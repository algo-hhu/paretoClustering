#ifndef Datastruct_H
#define Datastruct_H

#include <vector>
#include <cstddef>
#include <cmath>

struct Point {
    std::vector<double> coords;

    Point(): coords(){}
    Point(std::vector<double> coordinates) : coords(coordinates) {}

    Point(size_t dim) : coords(dim,0) {}

    double distance(const Point& other) const;
    double distance_sq(const Point& other) const; 
};

struct Distances {
    std::vector<std::pair<double, std::pair<size_t,size_t>>> dist;

    Distances(size_t n): dist(n){}
    Distances() = default;
    Distances(std::vector<std::pair<double, std::pair<size_t,size_t>>> d): dist(d){}
};

void swap(std::pair<double, std::pair<size_t, size_t>>& a, std::pair<double, std::pair<size_t, size_t>>& b);

int partition(Distances& arr, int low, int high);

void quickSort(Distances& arr, int low, int high);
#endif