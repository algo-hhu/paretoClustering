#ifndef SL_H
#define SL_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "unionfind.hpp"
#include "../datastruct.hpp"

bool writeClustersToFile(std::ofstream& file, UnionFind& clusters, const std::vector<Point>& points);

void singleLinkageThreshold(UnionFind& clusters, const double threshold, const std::vector<Point>& points);

void singleLinkageClustering(UnionFind& clusters, size_t k, const Distances &distances);

std::pair<size_t, double> singleLinkageOptimized(const size_t start, UnionFind& clusters, const Distances &distances);


#endif