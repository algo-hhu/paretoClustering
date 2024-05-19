#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>


#include "../../kmeanspp/kmeanspp.hpp"
#include "../../singlelinkage/singlelinkage.hpp"
#include "../../datastruct.hpp"
#include "../../singlelinkage/unionfind.hpp"
#include "common.hpp"
#include "../score/ARIAndNMI.hpp"

int main(int argc, char **argv) 
{
    if(argc<5){
        std::cout << "Input must contain at least four arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of outputfile"<<"\n"<<"number of clusters"<<"\n";
        std::cout << "0 no score somputation, 1 score computation\n" << "if 1 for score computation, name of ground truth\n";
        std::cout<< "if 1 for score computation, name of scores output\n";
        std::cout <<"seed for k-means++ (optional)" << std::endl;
        return 1;
    }

    std::string input=argv[1];
    std::string output=argv[2];
    if(std::stoi(argv[3])<=0){
        std::cout<< "Number of clusters must be a positive integer."<< std::endl;
        return 1;
    }

    size_t k=std::stoi(argv[3]);
    double cost;

    srand(static_cast<unsigned>(time(0)));
    int seed= rand();
    
    int scr=std::stoi(argv[4]);

    if(scr==0){
        std::cout << "No score computation"<<std::endl;
        if(argc>=6){
	        seed = std::atoi(argv[5]); 
	    } 
    
    
        bool success;

        std::vector<Point> points;
        success=readPointsFromFile(input, points);
        if(success==false){
            std::cout << "Error reading file" <<input <<"\n";
            return 1;
        }
    
        size_t num=points.size();
        size_t pairwise_dist=pow(num,2);

        UnionFind clusters(points);
    
        Distances distances(pairwise_dist);
        size_t index=0;
        for(size_t i=0; i<num; i++){
            for(size_t j=0; j<num; j++){
                index=i*num+j;
                distances.dist[index].first=points[i].distance(points[j]);
                distances.dist[index].second=std::make_pair(i,j);
            }
        }

        quickSort(distances, 0, pairwise_dist-1);

        std::ofstream file(output);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << output << std::endl;
            return 1;
        }
    
        std::pair<size_t, double> current;
        current.first=num-1;
    
    
        std::vector<std::pair<Point, int>> centroids=clusters.extractCentroids();
        std::vector<Point> kmeancenters= kmeansPPWeighted(centroids, k, 1 ,seed);
        std::vector<int> nearest=nesting(clusters, kmeancenters);
        cost=computeSolutionCost(points, nearest, kmeancenters);
       
        std::vector<std::pair<double, double>> paretoSet;

        paretoSet.push_back(std::make_pair(0.0, cost));

    
        while((current.first<pairwise_dist) && (clusters.getNumRoots()>k)){

            current=singleLinkageOptimized(current.first, clusters, distances);
            std::vector<std::pair<Point, int>> centroids=clusters.extractCentroids();
            kmeancenters= kmeansPPWeighted(centroids, k, 1 ,seed);
            std::vector<int> nearest=nesting(clusters, kmeancenters);
            cost=computeSolutionCost(points, nearest, kmeancenters);
            checkParetoOptimality(paretoSet, std::make_pair(distances.dist[current.first-1].first, cost));
        }

        file << std::fixed << std::setprecision(5);
        for(size_t i=0; i<paretoSet.size(); i++){
            file << "Pareto Point " << i << "\n";
            file << "Threshold "<<paretoSet[i].first<<"\n";
            file << "Cost "<<paretoSet[i].second<<"\n";
        }
        file.close();
    }
    if(scr!=1){
        std::cout<< "Prameter must have value 0 or 1"<<std::endl;
    }

    if(scr==1){
        std::string input_gt=argv[5];
        std::string output_scores=argv[6];

        if(argc>=7){
	        seed = std::atoi(argv[7]); 
	    }
    
        bool success;

        std::vector<Point> points;
        success=readPointsFromFile(input, points);
        if(success==false){
            std::cout << "Error reading file" <<input <<"\n";
            return 1;
        }
    
        size_t num=points.size();
        size_t pairwise_dist=pow(num,2);

        UnionFind clusters(points);    
        Distances distances(pairwise_dist);
        size_t index=0;
        for(size_t i=0; i<num; i++){
            for(size_t j=0; j<num; j++){
                index=i*num+j;
                distances.dist[index].first=points[i].distance(points[j]);
                distances.dist[index].second=std::make_pair(i,j);
            }
        }

        quickSort(distances, 0, pairwise_dist-1);
        std::pair<size_t, double> current;
        current.first=num-1;
    
    
        std::vector<std::pair<Point, int>> centroids=clusters.extractCentroids();
        std::vector<Point> kmeancenters= kmeansPPWeighted(centroids, k, 1 ,seed);
        std::vector<int> nearest=nesting(clusters, kmeancenters);
        cost=computeSolutionCost(points, nearest, kmeancenters);
       
        std::vector<std::pair<double, double>> paretoSet;
        std::vector<std::vector<double>> scores(0);

        paretoSet.push_back(std::make_pair(0.0, cost));

        std::vector<int> assign_gt=readFromFile(input_gt);

        double randindex= RandIndex(assign_gt,nearest);

        std::vector<double> NMIF_scores=normalizedMutalInformation_Fscores(assign_gt, nearest);
        std::vector<double> first_scores(5,0);
        first_scores[0]=randindex;
        first_scores[1]=NMIF_scores[0];
        first_scores[2]=NMIF_scores[1];
        first_scores[3]=NMIF_scores[2];
        first_scores[4]=NMIF_scores[3];

        scores.push_back(first_scores);
    
        while((current.first<pairwise_dist) && (clusters.getNumRoots()>k)){

            current=singleLinkageOptimized(current.first, clusters, distances);
            std::vector<std::pair<Point, int>> centroids=clusters.extractCentroids();
            kmeancenters= kmeansPPWeighted(centroids, k, 1 ,seed);
            std::vector<int> nearest=nesting(clusters, kmeancenters);
            cost=computeSolutionCost(points, nearest, kmeancenters);
            
            double curr_RI=RandIndex(assign_gt, nearest);
            std::vector <double> curr_NMIF_scores=normalizedMutalInformation_Fscores(assign_gt,nearest);

            std::vector<double> curr_scores(5,0);
            curr_scores[0]=curr_RI;
            curr_scores[1]=curr_NMIF_scores[0];
            curr_scores[2]=curr_NMIF_scores[1];
            curr_scores[3]=curr_NMIF_scores[2];
            curr_scores[4]=curr_NMIF_scores[3];

            checkParetoOptimalityScores(paretoSet, std::make_pair(distances.dist[current.first-1].first, cost), scores, curr_scores);
        }
        std::ofstream file(output);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << output << std::endl;
            return 1;
        }

        file << std::fixed << std::setprecision(5);
        for(size_t i=0; i<paretoSet.size(); i++){
            file << "Pareto Point " << i << "\n";
            file<<"Threshold "<<paretoSet[i].first<<"\n";
            file<<"Cost "<<paretoSet[i].second<<"\n";
        }
        file.close();

        std::ofstream file_scores(output_scores);
        if (!file_scores.is_open()) {
            std::cerr << "Unable to open file: " << output << std::endl;
            return 1;
        }
        if(scores.size()!=paretoSet.size()){
            std::cout<< "Size of pareto set does not mach number of scores"<<std::endl;
            std::cout<< "Pareto Set has size "<<paretoSet.size()<<std::endl;
            std::cout<< "Scores set has size "<<scores.size()<<std::endl;

            return 1;
        }

        for(size_t i=0; i<scores.size(); i++){
            file_scores<< "Threshold"<< "\n"<< paretoSet[i].first <<"\n";
            file_scores<< "Rand Index"<< "\n"<< scores[i][0]<<"\n";
            file_scores<< "F1"<< "\n"<< scores[i][1]<<"\n";
            file_scores<< "F2"<< "\n"<< scores[i][2]<<"\n";
            file_scores<< "F0.5"<< "\n"<< scores[i][3]<<"\n";
            file_scores<< "Normalized Mutal Information"<< "\n"<< scores[i][4]<<"\n";
        }
        file_scores.close();
    }
    return 0;
}