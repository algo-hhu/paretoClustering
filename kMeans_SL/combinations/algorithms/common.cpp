#include "common.hpp"

bool readPointsFromFile(const std::string& filename, std::vector<Point>& points) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        Point point;
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            std::istringstream coord_iss(token);
            double coord;
            if (coord_iss >> coord) {
                point.coords.push_back(coord);
            }
        }
        if (!point.coords.empty()) {
            points.push_back(point);
        }
    }

    file.close();
    return true;
}

std::vector<int> nesting(UnionFind& clusters, const std::vector<Point>& kmeancenters){
    size_t k= kmeancenters.size();
    size_t num=clusters.getSize();
    size_t dim=clusters.getDimension();
    
    size_t index=0;
    Point p(dim);
    double dist=0;
    double distmin=0;
    std::vector<int> nearest(num,-1);

    for(size_t i=0; i<num; i++){

        index=clusters.find(i);

        if(nearest[index]==-1){
            p=clusters.getClusterCentroid(index);
            distmin=std::numeric_limits<double>::max();

            for(size_t j=0;j<k; j++){
                dist=0;
                for(size_t l=0; l<dim; l++){
                    dist+=pow(p.coords[l]-kmeancenters[j].coords[l], 2); 
                }
                if(dist<=distmin){
                    nearest[index]=j;
                    distmin=dist;
                }
            }
            
        }
        nearest[i]=nearest[index];
    }

    return nearest;
}

bool writeClustersToFilewrtNesting(std::ofstream& file, const std::vector<Point>& points, const std::vector<int>& nearest, const std::vector<Point>& kmeanscenters) {
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << std::endl;
        return false;
    }
    size_t n=points.size();
    size_t k=kmeanscenters.size();
    std::vector<std::vector<Point>> output(k);
    for(size_t i=0; i<n; i++){
        if(nearest[i]>=(int) k || nearest[i]<0){
            std::cerr << "Invalid assignement" << std::endl;
            return false;
        }
        output[nearest[i]].push_back(points[i]);
    }

    size_t j=1;
    for (size_t i = 0; i < k; ++i) {
        if(output[i].size()!=0){
            file << "Cluster " << j << ":\n";

            for (const Point& point : output[i]) {
                file << "(";
                for (size_t j = 0; j < point.coords.size(); ++j) {
                    file << point.coords[j];
                    if (j < point.coords.size() - 1) {
                        file << ", ";
                    }
                }
                file << ")\n";
            }
            file << "\n";
        }
        j++;
    }
    return true;
}


bool writeNestingToFile(std::ofstream& file, const std::vector<int>& nearest) {
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << std::endl;
        return false;
    }
    size_t n=nearest.size();

    for (size_t i =0; i<n; i++){
            file<< i << " " <<nearest[i]<<"\n"; 
    }
    return true;
}

double computeSolutionCost(const std::vector <Point>& points, const std::vector<int>& nearest, const std::vector<Point>& kmeancenters){
    size_t num=points.size();
    size_t dim=points.at(0).coords.size();
    double cost=0;
    for(size_t i=0; i<num; i++){
        for(size_t l=0; l<dim; l++){
            cost+=pow(points[i].coords[l]-kmeancenters[nearest[i]].coords[l], 2); 
        }   
    }
    return cost;
}

void checkParetoOptimality(std::vector<std::pair<double,double>>& paretoSet, const std::pair<double, double>& newPoint){
    size_t n=paretoSet.size();
    double cost_old=paretoSet[n-1].second;

    if(cost_old>=newPoint.second){
        paretoSet[n-1].first=newPoint.first;
        paretoSet[n-1].second=newPoint.second;
    }
    paretoSet.push_back(newPoint);

}

void checkParetoOptimalityScores(std::vector<std::pair<double,double>>& paretoSet, const std::pair<double, double>& newPoint, std::vector<std::vector<double>>& scores, std::vector<double>& curr_scores){
    size_t n=paretoSet.size();
    double cost_old=paretoSet[n-1].second;
    size_t i=n-1;
    while(cost_old>=newPoint.second){
        paretoSet.pop_back();
        scores.pop_back();
        i--;
        cost_old=paretoSet[i].second;
        
    }
    paretoSet.push_back(newPoint);
    scores.push_back(curr_scores);
}

Distances pairwise_dist_sorted(std::vector <Point>& points){
    size_t num=points.size();
    size_t pairwise_dist=pow(num,2);

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
    return distances;
}

bool writeDistancesToFile(const Distances& distances, std::ofstream& outFile) {
    if (!outFile.is_open()) {
        return false;
    }

    for (const auto& entry : distances.dist) {
        outFile << entry.second.first << " " << entry.second.second << " "
                << std::fixed << std::setprecision(5) << entry.first << std::endl;
    }

    return true;
}

Distances readDistancesFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    Distances distances;

    if (!inFile.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return distances; // Return empty Distances object
    }

    double distance;
    size_t index1, index2;

    while (inFile >> index1 >> index2 >> distance) {
        distances.dist.push_back({distance, {index1, index2}});
    }

    inFile.close();
    return distances;
}
