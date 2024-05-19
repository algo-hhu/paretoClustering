#include "kmeanspp.hpp"

std::vector<Point> initializeCentroids(const std::vector<Point>& points, size_t k, std::optional<unsigned int> seed) {
    std::vector<Point> centroids;
    std::mt19937 gen;
    if (seed.has_value()) {
        gen.seed(seed.value());
    } else {
        std::random_device rd;
        gen = std::mt19937(rd());
    }
    std::uniform_int_distribution<> dis(0, points.size() - 1);

    if (k <= 0) {
        throw std::invalid_argument("Number of clusters must be positive.");
    }
	
	// Choose the first centroid randomly
	centroids.push_back(points[dis(gen)]);

    std::vector<double> distances(points.size(),std::numeric_limits<double>::infinity());

    // Choose remaining centroids
	while (centroids.size() < k) {
        
        double sum = 0.0;

        // Calculate distances from the nearest centroid
        for (size_t j = 0; j < points.size(); ++j) {
            double curr_dist = points[j].distance_sq(centroids[centroids.size()-1]);
			if (distances[j] > curr_dist){
				distances[j]=curr_dist;
			}			
            sum += distances[j];
        }

        // Choose the next center
        std::uniform_real_distribution<> dist(0, sum);
        double random_point = dist(gen);
        double cumulative_sum = 0.0;
        for (size_t j = 0; j < points.size(); ++j) {
            cumulative_sum += distances[j];
            if (cumulative_sum >= random_point) {
                centroids.push_back(points[j]);
                break;
            }
        }
    }

    return centroids;
}

std::pair<std::vector<Point>, std::vector<double>> initializeCentroidsWeighted(const std::vector<std::pair<Point, int>>& points, size_t k, std::optional<unsigned int> seed){
    std::vector<Point> centroids;
    std::mt19937 gen;
    if (seed.has_value()) {
        gen.seed(seed.value());
    } else {
        std::random_device rd;
        gen = std::mt19937(rd());
    }


    int totalweight=0;

    for(size_t i=0; i< points.size(); i++){
        totalweight+=points[i].second;
    }

    std::uniform_int_distribution<> dis(0, totalweight - 1);
	std::vector<double> costs(k);
	for (unsigned int i=0;i<k;i++){
		costs[i] = 0;
	}

    if (k <= 0) {
        throw std::invalid_argument("Number of clusters must be positive.");
    }
	

	// Choose the first centroid randomly
	int random_point = dis(gen);
    int cumulative_sum = 0;
    for (size_t j = 0; j < points.size(); ++j) {
        cumulative_sum += points[j].second;
        if (cumulative_sum >= random_point) {
            centroids.push_back(points[j].first);
            break;
        }
    }

    std::vector<double> distances(points.size(),std::numeric_limits<double>::infinity());
    // Choose remaining centroids
	while (centroids.size() < k) {

        double sum = 0.0;

        // Calculate distances from the nearest centroid
        for (size_t j = 0; j < points.size(); ++j) {
            double curr_dist = points[j].first.distance_sq(centroids[centroids.size()-1]);
			if (distances[j] > curr_dist){
				distances[j]=curr_dist;
			}			
            sum += distances[j]*(double)points[j].second;
        }

        // Choose the next center
        std::uniform_real_distribution<> dist(0, sum);
        double random_point = dist(gen);
        double cumulative_sum = 0.0;
        for (size_t j = 0; j < points.size(); ++j) {
            cumulative_sum += distances[j]*(double)points[j].second;
            if (cumulative_sum >= random_point) {
                centroids.push_back(points[j].first);
                break;
            }
        }
    }

    return std::make_pair(centroids, costs);

}


int findClosestCentroid(const Point& point, const std::vector<Point>& centroids) {
    int closest = 0;
    double min_distance = std::numeric_limits<double>::max();

    for (unsigned int i = 0; i < centroids.size(); ++i) {
        double distance = point.distance(centroids[i]);
        if (distance < min_distance) {
            min_distance = distance;
            closest = i;
        }
    }

    return closest;
}


std::vector<Point> kmeansPPWeighted(const std::vector<std::pair<Point, int>>& points, size_t k, bool lloyds, std::optional<unsigned int> seed) {
     // Initialize centroids
	std::pair<std::vector<Point>, std::vector<double>>  initResult;
    if (seed.has_value()) {
        initResult = initializeCentroidsWeighted(points, k, seed);
    } 
    else {
        initResult = initializeCentroidsWeighted(points, k);
    }
    std::vector<Point> centroids = initResult.first;

	//return the centroids immediately if no lloyd's is required
	if (!lloyds){
		return centroids;
	}

    std::vector<int> assignments(points.size(),0);
    bool changed;

    size_t maxIterations=100;
    for(size_t iteration = 0; iteration < maxIterations; ++iteration) {
        changed = false;

        // Assign points to the closest centroid
        for (size_t i = 0; i < points.size(); ++i) {
            int closest = findClosestCentroid(points[i].first, centroids);
            if (assignments[i] != closest) {
                assignments[i] = closest;
                changed = true;
            }
        }

        // Recalculate centroids
        std::vector<Point> newCentroids(k, Point(centroids[0].coords.size()));
        std::vector<int> counts(k, 0);

        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = 0; j < points[i].first.coords.size(); ++j) {
                newCentroids[assignments[i]].coords[j] += (double) points[i].second * points[i].first.coords[j];
            }
            counts[assignments[i]]+= points[i].second;
        }

        for (size_t i = 0; i < k; ++i) {
            if (counts[i] > 0) {
                for (size_t j = 0; j < centroids[i].coords.size(); ++j) {
                    newCentroids[i].coords[j] /= (double)counts[i];
                }
            }
        }

        centroids = newCentroids;

        // Check for convergence
        if (!changed) {
            break;
        }
    }
    return centroids;
}

std::vector<Point> kmeansPP(const std::vector<Point>& points, size_t k, bool lloyds, std::optional<unsigned int> seed) {
    // Initialize centroids
	std::vector<Point>  centroids;
    if (seed.has_value()) {
        centroids = initializeCentroids(points, k, seed);
    } 
    else {
        centroids = initializeCentroids(points, k);
    }

	//we return the centroids immediately if no lloyd's is required
	if (!lloyds){
		return centroids;
	}

    centroids=Lloyds(points, centroids);

    return centroids;
}

std::vector<Point> Lloyds(const std::vector<Point>& points, std::vector<Point>& centroids) {
    std::vector<int> assignments(points.size(),0);
    size_t k=centroids.size();
    bool changed;

    size_t maxIterations=100;
    for(size_t iteration = 0; iteration < maxIterations; ++iteration) {
        changed = false;

        // Assign points to the closest centroid
        for (size_t i = 0; i < points.size(); ++i) {
            int closest = findClosestCentroid(points[i], centroids);
            if (assignments[i] != closest) {
                assignments[i] = closest;
                changed = true;
            }
        }

        // Recalculate centroids
        std::vector<Point> newCentroids(k, Point(centroids[0].coords.size()));
        std::vector<int> counts(k, 0);

        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t j = 0; j < points[i].coords.size(); ++j) {
                newCentroids[assignments[i]].coords[j] += points[i].coords[j];
            }
            counts[assignments[i]]++;
        }

        for (size_t i = 0; i < k; ++i) {
            if (counts[i] > 0) {
                for (size_t j = 0; j < centroids[i].coords.size(); ++j) {
                    newCentroids[i].coords[j] /= (double)counts[i];
                }
            }
        }

        centroids = newCentroids;

        // Check for convergence
        if (!changed) {
            break;
        }
    }
    return centroids;
}


std::vector<double> clustercosts(const std::vector<Point>& centroids, const std::vector<Point>& points) {
    std::vector<double> costs(centroids.size(), 0.0);

    for (const auto& point : points) {
        double minDistance = std::numeric_limits<double>::max();
        size_t closestCentroidIndex = 0;

        for (size_t i = 0; i < centroids.size(); ++i) {
            double distance = point.distance_sq(centroids[i]);
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroidIndex = i;
            }
        }

        costs[closestCentroidIndex] += minDistance;
    }

    return costs;
}