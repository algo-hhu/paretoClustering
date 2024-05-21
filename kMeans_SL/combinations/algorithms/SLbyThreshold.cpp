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
        std::cout << "Input must contain four arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of outputfile"<<"\n"<<"name of outputfile for assignment"<<"\n"<<"threshold for Single Linkage"<<"\n";
        return 1;
    }

    std::string input=argv[1];
    std::string output=argv[2];
    std::string outputassign=argv[3];

    double threshold=std::stod(argv[4]);
   
    std::cout << "Perform one run of Single Linkage on Input: "<< input << "\n";
    std::cout << "With threshold: "<<threshold<< "\n";
    std::cout << "Clustering is stored in: " << output << std::endl;
    std::cout << "Assignment is stored in: " << outputassign << std::endl;
        
    bool success;
        
    std::vector<Point> points;
        
    success=readPointsFromFile(input, points);
        
    if(success==false){
        return 1;
    }

    UnionFind clusters(points);
    singleLinkageThreshold(clusters, threshold, points);

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