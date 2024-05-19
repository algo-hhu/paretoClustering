#include "unionfind.hpp"

UnionFind::UnionFind(const std::vector<Point>& points) {
    size_t n=points.size();
    numRoots=n;
    parent=std::vector<size_t> (n, 0);
    rank=std::vector<size_t> (n, 0);
    centroids=std::vector<Point> (n);
    size_t dim=points.at(0).coords.size();
    for(size_t i=0; i<n; i++){
        if(dim!=points[i].coords.size()){
            std::cerr << "Points of different dimension." << std::endl;
            throw std::runtime_error("Points of different dimension");

        }
        centroids[i]=points[i].coords;
    }
    costSize=std::vector<std::pair<double, double>> (n, std::make_pair(0,1));
    
    for (size_t i = 0; i < n; ++i)
        parent[i] = i;
}

size_t UnionFind::find(size_t x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]); // Path Compression
    return parent[x];
}

bool UnionFind::unionSets(size_t x, size_t y) {
    size_t rootX = find(x);
    size_t rootY = find(y);
    if (rootX == rootY) return false;

    numRoots--;
    size_t dim=centroids.at(0).coords.size();
    Point centroidNew(dim);
    
    double sizeNew=costSize[rootX].second+costSize[rootY].second;

    for(size_t i=0; i<dim; i++){
        centroidNew.coords[i]=centroids[rootX].coords[i]*costSize[rootX].second;
        centroidNew.coords[i]+=centroids[rootY].coords[i]*costSize[rootY].second;
        centroidNew.coords[i]/=sizeNew;
    }
    double costNew=costSize[rootX].second*centroids[rootX].distance(centroidNew)
    +costSize[rootY].second*centroids[rootY].distance(centroidNew);
    costNew+=costSize[rootX].first;
    costNew+=costSize[rootY].first;

    // Union by Rank
    if (rank[rootX] < rank[rootY]){
        parent[rootX] = rootY;
        centroids[rootY]=centroidNew;
        costSize[rootY].first=costNew;
        costSize[rootY].second=sizeNew;
    }
    else if (rank[rootX] > rank[rootY]){
        parent[rootY] = rootX;
        centroids[rootX]=centroidNew;
        costSize[rootX].first=costNew;
        costSize[rootX].second=sizeNew;
    }
    else {
        parent[rootY] = rootX;
        rank[rootX]++;
        centroids[rootX]=centroidNew;
        costSize[rootX].first=costNew;
        costSize[rootX].second=sizeNew;
    }
    return true;
}

bool UnionFind::isConnected(size_t x, size_t y) {
    return find(x) == find(y);
}

size_t UnionFind::getSize(){
    return parent.size();
}

size_t UnionFind::getDimension(){
    if(centroids.size()==0){
        return 0;
    }
    else{
        return centroids[0].coords.size();
    }
}

size_t UnionFind::getNumRoots(){
    return numRoots;
}

Point UnionFind::getClusterCentroid(size_t i){
    if(i>parent.size()-1){
        std::cerr << "Centroid request out of scope." << std::endl;
        throw std::runtime_error("Centroid request out of scope.");
    }
    return centroids[i];

}

double UnionFind::getClusterCost(size_t i){
    if(i>parent.size()-1){
        std::cerr << "Cluster cost request out of scope." << std::endl;
        throw std::runtime_error("Cluster cost request out of scope.");
    }
    return costSize[i].first;
}

double UnionFind::getClusterSize(size_t i){
    if(i>parent.size()-1){
        std::cerr << "Cluster size request out of scope." << std::endl;
        throw std::runtime_error("Cluster size request out of scope.");
    }
    return costSize[i].second;
}

void UnionFind::writeToFile(std::ofstream& outFile) {

    for (size_t i = 0; i < parent.size(); ++i) {
        size_t root = find(i); // Find the root of element i
        outFile << i << " " << root << std::endl;
    }
}

std::vector<std::pair<Point, int>> UnionFind::extractCentroids(){
    size_t num=parent.size();

    std::vector<bool> usedAsCenter(num, false);

    for(size_t i=0; i<num; i++){
        size_t root= find(i);
        usedAsCenter[root]=true;
    }

    std::vector<std::pair<Point, int>> centroids(numRoots);
    size_t curr_num_roots=0;
    for(size_t i=0; i<num; i++){
        if(usedAsCenter[i]==true){
            centroids[curr_num_roots]=std::make_pair(getClusterCentroid(i), getClusterSize(i));
            curr_num_roots++;
        }
    }

    return centroids;

}