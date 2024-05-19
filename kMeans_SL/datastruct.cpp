#include "datastruct.hpp"

double Point::distance(const Point& other) const {
    double dist = 0.0;
    for (size_t i = 0; i < coords.size(); ++i) {
        dist += std::pow(coords[i] - other.coords[i], 2);
    }
    return std::sqrt(dist);
}

double Point::distance_sq(const Point& other) const {
    double dist = 0.0;
    for (size_t i = 0; i < coords.size(); ++i) {
        dist += std::pow(coords[i] - other.coords[i], 2);
    }
    return dist;
}


void swap(std::pair<double, std::pair<size_t, size_t>>& a, std::pair<double, std::pair<size_t, size_t>>& b) {
    auto temp = a;
    a = b;
    b = temp;
}

int partition(Distances& arr, int low, int high) {
    double pivot = arr.dist[high].first;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr.dist[j].first <= pivot) {
            i++;
            swap(arr.dist[i], arr.dist[j]);
        }
    }
    swap(arr.dist[i + 1], arr.dist[high]);
    return (i + 1);
}

void quickSort(Distances& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
