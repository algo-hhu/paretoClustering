#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <random>
#include <ctime>
#include <iomanip>
#include <string>
#include <filesystem>
#include <optional>
#include "../../datastruct.hpp"
#include "../../singlelinkage/unionfind.hpp"

bool readPointsFromFile(const std::string& filename, std::vector<Point>& points);

std::vector<int> nesting(UnionFind& clusters, const std::vector<Point>& kmeancenters);

bool writeClustersToFilewrtNesting(std::ofstream& file, const std::vector<Point>& points, const std::vector<int>& nearest, const std::vector<Point>& kmeanscenters);

bool writeNestingToFile(std::ofstream& file, const std::vector<int>& nearest);

double computeSolutionCost(const std::vector <Point>& points, const std::vector<int>& nearest, const std::vector<Point>& kmeancenters);

void checkParetoOptimality(std::vector<std::pair<double,double>>& paretoSet, const std::pair<double, double>& newPoint);

void checkParetoOptimalityScores(std::vector<std::pair<double,double>>& paretoSet, const std::pair<double, double>& newPoint, std::vector<std::vector<double>>& scores, std::vector<double>& curr_scores);


Distances pairwise_dist_sorted(std::vector <Point>& points);

bool writeDistancesToFile(const Distances& distances, std::ofstream& outFile);

Distances readDistancesFromFile(const std::string& filename);

#endif