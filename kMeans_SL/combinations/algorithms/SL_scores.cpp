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


std::vector <int> assignment(UnionFind& clusters){
    size_t n=clusters.getSize();
    std::vector <int> assign(n);
    for(size_t i=0; i<n ; i++) {
        assign[i]=clusters.find(i);
    }
    return assign;
}

int main(int argc, char **argv) 
{
    if(argc<4){
        std::cout << "Input must contain at least three arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of inputfile for ground truth"<<"\n"<<"name of outputfile for scores"<<"\n"<<"name of file for sorted pairwise distances (optional)"<<"\n";
        return 1;
    }

    std::string input=argv[1];
    std::string gt=argv[2];
    std::string output=argv[3];

        
    bool success;
        
    std::vector<Point> points;
        
    success=readPointsFromFile(input, points);
        
    if(success==false){
        return 1;
    }

    UnionFind clusters(points);
    Distances distances;
    size_t num=points.size();
    size_t pairwise_dist=pow(num,2);

    if(argc==5){
        std::string dist_file=argv[4];
        distances=readDistancesFromFile(dist_file);

    }
    else{
        
        distances=pairwise_dist_sorted(points);
    }

    std::pair<size_t, double> current;
    current.first=num-1;

    std::vector<int> assign_gt=readFromFile(gt);
    
    std::ofstream file(output);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << output << std::endl;
        return 1;
    }

    while((current.first<pairwise_dist) && (clusters.getNumRoots()>1)){
        current=singleLinkageOptimized(current.first, clusters, distances);
        std::vector<int> assign= assignment(clusters);
        file<< "Number of Clusters"<<"\n"<<clusters.getNumRoots()<<"\n";
        file<< "Threshold"<< "\n"<< current.second <<"\n";

        double RI=RandIndex(assign_gt,assign);
        std::vector<double> scores=normalizedMutalInformation_Fscores(assign_gt, assign);
        file<< "Rand Index"<< "\n"<< RI<<"\n";
        file<< "F1"<< "\n"<< scores[0]<<"\n";
        file<< "F2"<< "\n"<< scores[1]<<"\n";
        file<< "F0.5"<< "\n"<< scores[2]<<"\n";
        file<< "Normalized Mutal Information"<< "\n"<< scores[3]<<"\n";

    }

    file.close();
    return 0;
}