#ifndef UF_H
#define UF_H

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include"../datastruct.hpp"

class UnionFind
{
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;
    std::vector<Point> centroids;
    std::vector<std::pair<double, double>> costSize;
    size_t numRoots;

public:
    UnionFind(const std::vector<Point>& points);
    size_t find(size_t x);
    bool unionSets(size_t x, size_t y);
    bool isConnected(size_t x, size_t y);
    size_t getSize();
    size_t getDimension();
    Point getClusterCentroid(size_t i);
    double getClusterCost(size_t i);
    double getClusterSize(size_t i);
    size_t getNumRoots();
    std::vector<std::pair<Point, int>> extractCentroids();
    void writeToFile(std::ofstream& outFile);

};

#endif
