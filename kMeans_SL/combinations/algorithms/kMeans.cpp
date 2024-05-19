#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>


#include "../../kmeanspp/kmeanspp.hpp"
#include "../../datastruct.hpp"
#include "common.hpp"


std::vector <int> assignment(const std::vector <Point>& points, const std::vector <Point>& centers){
    size_t n=points.size();
    std::vector <int> assign(n);
    for(size_t i=0; i<n ; i++) {
        int t=findClosestCentroid(points[i], centers);
        assign[i]=t;
    }
    return assign;
}

int main(int argc, char **argv) 
{
    if(argc<4){
        std::cout << "Input must contain at least three arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of outputfile\n"<<"number of clusters\n"<<"seed (optional)"<<"\n";
        return 1;
    }

    std::string input=argv[1];
    std::string output=argv[2];
    int k=std::stoi(argv[3]);

    srand(static_cast<unsigned>(time(0)));
    int seed= rand();

    if(argc>=5){
	    seed = std::atoi(argv[4]); 
	} 
    bool success;
        
    std::vector<Point> points;
        
    success=readPointsFromFile(input, points);
        
    if(success==false){
        return 1;
    }
    std::vector<Point> centers;

    std::ofstream file(output);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << output << std::endl;
        return 1;
    }
    centers=kmeansPP(points, k, 1, seed);

    
    std::vector<int> assign= assignment(points, centers);
    writeClustersToFilewrtNesting(file, points, assign, centers);

    file.close();
    return 0;
}