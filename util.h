#ifndef UTIL_H
#define UTIL_H
#include "taxonomy.h"
#include <vector>

// Function to calculate the centroid of a set of points
point3 centroid(const std::vector<point3>& points);

// Function to calculate the angle from the centroid to a point
double angle_from_centroid(const point3& c, const point3& p);

// Main function to remove duplicates and sort points counterclockwise
std::vector<point3> process_points(std::vector<point3> points);

// Function to print points
void print_points(const std::vector<point3>& points);


#endif //UTIL_H
