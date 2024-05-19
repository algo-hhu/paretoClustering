#include "singlelinkage.hpp"

bool writeClustersToFile(std::ofstream& file, UnionFind& clusters, const std::vector<Point>& points) {
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << std::endl;
        return false;
    }
    size_t n=points.size();
    std::vector<std::vector<Point>> output(n);
    for(size_t i=0; i<n; i++){
        output[clusters.find(i)].push_back(points[i]);
    }

    size_t j=1;
    for (size_t i = 0; i < n; ++i) {
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

void singleLinkageThreshold(UnionFind& clusters, const double threshold, const std::vector<Point>& points){
    size_t num=clusters.getSize();
    double dist;

    //unite points if they are at distance at most threshold
    for(size_t i=0; i<num; i++){
        for(size_t j=i+1; j<num; j++){
            dist=points[i].distance(points[j]);
            if(dist<=threshold){
                clusters.unionSets(i,j); 
            }
        }
    }
}

std::pair<size_t, double> singleLinkageOptimized(const size_t start, UnionFind& clusters, const Distances &distances) {
    size_t p,q;
    size_t num=distances.dist.size();
    
    if(num-1<start){
        throw std::runtime_error("singleLinkageOptimized: start out of scope");
    }

    double threshold=distances.dist[start].first;

    //unite points if they are at distance at most threshold
    bool changed=false;
    for(size_t i=start; i<num; i++){
        if(distances.dist[i].first<=threshold){
            p=(distances.dist[i].second).first;
            q=(distances.dist[i].second).second;
            changed=clusters.unionSets(p,q); 
        }
        else{
            if(changed==true){
                return std::make_pair(i, threshold);
            }
            threshold=distances.dist[i].first;
        }
    }
    return std::make_pair(num, distances.dist[num-1].first);
}

void singleLinkageClustering(UnionFind& clusters, size_t k, const Distances &distances) {
    size_t p,q;
    //unite points as long as there are more then k clusters
    if(k==0){
        throw std::runtime_error("singleLinkageClsutering: invalid number of clusters");
    }
    size_t i=0;
    while(clusters.getNumRoots()>k){
        p=(distances.dist[i].second).first;
        q=(distances.dist[i].second).second;
        clusters.unionSets(p,q); 
        i++;
    }
}