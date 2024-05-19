#include "ARIAndNMI.hpp"

std::vector<int> readFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;
    std::vector<int> data;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return std::vector<int>(0);
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string part;
        int second;
            
        if (std::getline(iss, part, ' ')) {
            if (std::getline(iss, part)) {
                second = std::stoi(part);
                data.push_back(second);
            }
        }
    }

    file.close();
    return data;
}

//computes the rand index
double RandIndex(const std::vector<int>& groundTruth, const std::vector<int>& computation){
     if(groundTruth.size()!=computation.size()){
        std::cout<< "Different number of points in ground truth and computed solution!"<<std::endl;
        std::cout<<"Ground Truth:"<<groundTruth.size()<<std::endl;
        std::cout<<"Computed Solution:"<< computation.size()<<std::endl;
        return 0;
    }
    
    int TP=0,TN=0, FP=0, FN=0;

    for(size_t i=0; i<groundTruth.size(); i++){
        for(size_t j=i+1; j<groundTruth.size(); j++){
            if(groundTruth[i]!=groundTruth[j]){
                if(computation[i]!=computation[j]){
                    TN++;
                }
                else{
                    FP++;
                }
            }
            else{
                if(computation[i]!=computation[j]){
                    FN++;
                }
                else{
                    TP++;
                }

            }
        }
    }
    int sum= TN+TP+FP+FN;
    int correct= TN+TP;
    double score= (double) correct/double(sum);
    return score;
}

//computes the normalized mutal information and F scores
std::vector<double> normalizedMutalInformation_Fscores(const std::vector<int>& groundTruth, const std::vector<int>& computation){
    
    //check if ground truth and solution have same number of points
     if(groundTruth.size()!=computation.size()){
        std::cout<< "Different number of points in ground truth and computed solution!"<<std::endl;
        std::cout<<"Ground Truth:"<<groundTruth.size()<<std::endl;
        std::cout<<"Computed Solution:"<< computation.size()<<std::endl;
        return std::vector<double>(0);
    }

    double maxGT=0;
    double maxC=0;
    const int n=groundTruth.size();


    //compute number of labels for ground truth and solution
    for(size_t i=0; i<groundTruth.size(); i++){
        if(groundTruth[i]<0 || computation[i]<0){
            throw std::runtime_error("invalid label for points");
        }
        if(groundTruth[i]>maxGT){
            maxGT=groundTruth[i];
        }
        if(computation[i]>maxC){
            maxC=computation[i];
        }
    }

    std::vector<std::vector<int>> similarity(maxGT+1);
    for(size_t i=0; i< maxGT+1; i++){
        similarity[i]=std::vector(maxC+1,0);
    }
    
    std::vector<int> sumGT(maxGT+1,0);
    std::vector<int> sumC(maxC+1,0);

    //compute the size of clusters in ground truth and solution
    //compute the size of the intersection of a cluster in the ground truth and a cluster in the solution
    for(size_t i=0; i< groundTruth.size(); i++){
        int a=groundTruth[i];
        int b=computation[i];
        similarity[a][b]++;
        sumGT[a]++;
        sumC[b]++;
    }

    //compute entropy of ground truth
    double entropyGT=0;
    for(size_t i=0; i<maxGT+1; i++){
        if(sumGT[i]>0){
            double ent=(double)sumGT[i]/(double)n;
            ent=log2(ent);
            ent*=(double)sumGT[i]/(double)n;
            entropyGT-=ent;
        }
    }
    
    //compute entropy of solution
    double entropyC=0;
    for(size_t i=0; i<maxC+1; i++){
        if(sumC[i]>0){
            double ent=(double)sumC[i]/(double)n;
            ent=log2(ent);
            ent*=(double)sumC[i]/(double)n;
            entropyC-=ent;
        }
    }

    //compute NMI
    double score=0;
    for(size_t i=0; i<maxGT+1; i++){
        for(size_t j=0; j<maxC+1; j++){
            if(sumGT[i]>0 && sumC[j]>0 && similarity[i][j]>0){
                double sim=(double) similarity[i][j]* (double) n;
                sim/=(double) sumGT[i]*(double) sumC[j];
                sim=log2(sim);
                sim*=similarity[i][j];
                sim/=(double) n;
                score+=sim;
            }
        }
    }
    score/=entropyC+entropyGT;
    score=2*score;
    std::vector<double> scores(4,0);
    scores[3]=score;

    //compute F_b-scores for b=1, 2, 0.5 
    double f1,f2,f05;

    for(size_t i=0; i<maxGT+1; i++){
        f1=0;
        f2=0;
        f05=0;
        for(size_t j=0; j<maxC+1; j++){
            if(sumGT[i]>0 && sumC[j]>0 && similarity[i][j]>0){
                double recall=similarity[i][j]/ (double) sumGT[i];
                double prec=similarity[i][j]/(double) sumC[j];
                double f1_tmp= 2 * (prec * recall)/(prec + recall);
                double f2_tmp= 5 * (prec * recall)/(4*prec + recall);
                double f05_tmp= 1.25 * (prec * recall)/(0.25*prec + recall);
                if(f1<f1_tmp){
                    f1=f1_tmp;
                }
                if(f2<f2_tmp){
                    f2=f2_tmp;
                }
                if(f05<f05_tmp){
                    f05=f05_tmp;
                }
            }
        }
        scores[0]+=f1*sumGT[i];
        scores[1]+=f2*sumGT[i];
        scores[2]+=f05*sumGT[i];
    }

    scores[0]/=n;
    scores[1]/=n;
    scores[2]/=n;

    return scores;
}

