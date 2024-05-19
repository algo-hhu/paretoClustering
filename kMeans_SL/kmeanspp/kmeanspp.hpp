#ifndef KM_H
#define KM_H

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
#include "../datastruct.hpp"

namespace fs = std::filesystem;

//k-means++ sampling of initial k centers
std::vector<Point> initializeCentroids(const std::vector<Point>& points, size_t k, std::optional<unsigned int> seed = std::nullopt);

//k-means++ sampling of initial k centers, can treat weighted points
std::pair<std::vector<Point>, std::vector<double>> initializeCentroidsWeighted(const std::vector<std::pair<Point, int>>& points,  size_t k, std::optional<unsigned int> seed = std::nullopt);


int findClosestCentroid(const Point& point, const std::vector<Point>& centroids);

// kmeans++ sampling and, optional: followed by Lloyd
std::vector<Point> kmeansPP(const std::vector<Point>& points, size_t k, bool lloyds,  std::optional<unsigned int> seed = std::nullopt);

// kmeans++ sampling and, optional: followed by Lloyd, can treat weighted points
std::vector<Point> kmeansPPWeighted(const std::vector<std::pair<Point, int>>& points, size_t k, bool lloyds, std::optional<unsigned int> seed= std::nullopt);

std::vector<Point> Lloyds(const std::vector<Point>& points, std::vector<Point>& centroids);

//computes k-means cost of points wrt. to centroids
std::vector<double> clustercosts(const std::vector<Point>& centroids, const std::vector<Point>& points);

#endif