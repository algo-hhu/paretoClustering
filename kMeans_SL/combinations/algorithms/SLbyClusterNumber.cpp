#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>


#include "../../singlelinkage/singlelinkage.hpp"
#include "../../datastruct.hpp"
#include "../../singlelinkage/unionfind.hpp"
#include "common.hpp"


int main(int argc, char **argv) 
{
    if(argc<5){
        std::cout << "Input must contain at least four arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of outputfile"<<"\n"<<"name of outputfile for assignment"<<"\n"<<"number of clusters"<<"\n"<<"name of file for sorted pairwise distances (optional)"<<"\n";
        return 1;
    }

    std::string input=argv[1];
    std::string output=argv[2];
    std::string outputassign=argv[3];

    if(std::stoi(argv[4])<=0){
        std::cout<< "Number of clusters must be a positive integer."<< std::endl;
        return 1;
    }

    size_t k=std::stoi(argv[4]);

    std::cout << "Perform one run of Single Linkage on Input: "<< input << "\n";
    std::cout << "With number of clusters: "<<k<< "\n";
    std::cout << "Clustering is stored in: " << output << std::endl;
    std::cout << "Assignment is stored in: " << outputassign << std::endl;
        
    bool success;
        
    std::vector<Point> points;
        
    success=readPointsFromFile(input, points);
        
    if(success==false){
        return 1;
    }

    UnionFind clusters(points);
    Distances distances;

    if(argc==6){
        std::string dist_file=argv[5];
        distances=readDistancesFromFile(dist_file);

    }
    else{
        distances=pairwise_dist_sorted(points);
    }
    
    singleLinkageClustering(clusters, k, distances);
    std::ofstream file(output);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << output << std::endl;
        return false;
    }
    writeClustersToFile(file, clusters, points);
    file.close();

    std::ofstream fileassign(outputassign);
    if (!fileassign.is_open()) {
        std::cerr << "Unable to open file: " << outputassign << std::endl;
        return false;
    }
    clusters.writeToFile(fileassign);
    fileassign.close();

    return 0;
}