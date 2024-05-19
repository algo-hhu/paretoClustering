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

int main(int argc, char **argv) 
{
    if(argc<6){
        std::cout << "Input must contain at least five arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of outputfile for clustering"<<"\n"<<"name of outputfile for assignment"<<"\n";
        std::cout << "number of clusters"<<"\n"<< "threshold for Single Linkage"<<"\n";
        std::cout << "seed for k-means++ (optional)" << std::endl;
        return 1;
    }

    std::string input=argv[1];
    std::string output=argv[2];
    std::string outputassign = argv[3]; 

    if(std::stoi(argv[4])<=0){
        std::cout<< "Number of clusters must be a positive integer."<< std::endl;
        return 1;
    }
    size_t k=std::stoi(argv[4]);
    double threshold=std::stod(argv[5]);


    srand(static_cast<unsigned>(time(0)));
    int seed= rand();
    
    if(argc>=7){
	    seed = std::atoi(argv[6]); 
    } 

    bool success;
    
    std::vector<Point> points;
    success=readPointsFromFile(input, points);

        
    if(success==false){
        return 1;
    }

    UnionFind clusters(points);
        
    singleLinkageThreshold(clusters, threshold, points);
    std::vector<std::pair<Point, int>> centroids=clusters.extractCentroids();
    std::vector<Point> kmeancenters= kmeansPPWeighted(centroids, k, 1 ,seed);
    std::vector<int> nearest=nesting(clusters, kmeancenters);

    std::ofstream file(output);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << output << std::endl;
        return 1;
    }
    
    writeClustersToFilewrtNesting(file, points, nearest, kmeancenters);
    file.close();
        
    std::ofstream fileassign(outputassign);
    if (!fileassign.is_open()) {
        std::cerr << "Unable to open file: " << outputassign << std::endl;
        return 1;
    }

    writeNestingToFile(fileassign, nearest);
    fileassign.close();
    return 0; 
}