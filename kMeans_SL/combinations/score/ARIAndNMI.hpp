#ifndef SCORE_H
#define SCORE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>


std::vector<int> readFromFile(const std::string& filePath);

//computes the adjusted rand index
double RandIndex(const std::vector<int>& groundTruth, const std::vector<int>& computation);


//computes the normalized mutal information
std::vector<double> normalizedMutalInformation_Fscores(const std::vector<int>& groundTruth, const std::vector<int>& computation);
#endif