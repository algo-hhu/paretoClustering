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
#include "../../singlelinkage/singlelinkage.hpp"
#include "../../datastruct.hpp"
#include "../../singlelinkage/unionfind.hpp"
#include "common.hpp"
#include "../score/ARIAndNMI.hpp"

void compute_index_version2(const std::vector <Point>& points, const std::vector<int>& assign_gt, std::ofstream& file, std::optional<unsigned int> seed = std::nullopt){

    size_t num=points.size();
    std::pair<int, double> a{0,std::numeric_limits<double>::infinity()};
    
    std::vector<int> assign_comp(num);
    double curr_dist; 

    size_t max=100;
    if(num<max){ 
        max=num;
    }
    for(size_t i=1; i<max; i++){
        std::vector centroids=kmeansPP(points, i, 1, seed);
        
        std::vector<std::pair<int, double>> nearest_dist(num, a);
        for(size_t j=0; j<num; j++){
            for(size_t l=0; l<i; l++){
                curr_dist=points[j].distance(centroids[l]);
                if(nearest_dist[j].second>=curr_dist){
                    nearest_dist[j].first=l;
                    nearest_dist[j].second=curr_dist;
                }
            }
        }
        for(size_t j=0; j<num; j++){
            assign_comp[j]=nearest_dist[j].first;
        }

        file<< "Number of Clusters"<< "\n"<< i <<"\n";
        file<< "Rand Index"<< "\n"<< RandIndex(assign_gt, assign_comp)<<"\n";
        std::vector<double> scores= normalizedMutalInformation_Fscores(assign_gt, assign_comp);
        file<< "F1"<< "\n"<< scores[0]<<"\n";
        file<< "F2"<< "\n"<< scores[1]<<"\n";
        file<< "F0.5"<< "\n"<< scores[2]<<"\n";
        file<< "Normalized Mutal Information"<<"\n"<<scores[3]<< "\n";
    }

}

int main(int argc, char **argv) 
{
    if(argc<4){
        std::cout << "Input must contain at least three arguments:" <<"\n";
        std::cout << "name of inputfile"<<"\n"<<"name of inputfile for ground truth"<<"seed (optional)"<<"\n";
        return 1;
    }

    std::string input=argv[1];
    std::string gt=argv[2];
    std::string output=argv[3];

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
    std::vector<int> assign_gt=readFromFile(gt);

    std::ofstream file(output);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << output << std::endl;
        return 1;
    }
    compute_index_version2(points, assign_gt, file, seed);

    file.close();

    return 0;
}