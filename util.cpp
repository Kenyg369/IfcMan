#include "util.h"
#include <iostream>
#include <unordered_set>
#include <cmath>
#include <algorithm>

point3 centroid(const std::vector<point3> &points) {
    point3 c = {0, 0, 0};
    for (const auto& p : points) {
        c.x += p.x;
        c.y += p.y;
        c.z += p.z;

    }
    c.x /= points.size();
    c.y /= points.size();
    c.z /= points.size();

    return c;
}

// not sure about manifold 3d direction? regradless of z axis???
double angle_from_centroid(const point3& c, const point3& p) {
    return std::atan2(p.y - c.y, p.x - c.x);
}

std::vector<point3> process_points(std::vector<point3> points) {
    std::unordered_set<point3> unique_points(points.begin(), points.end());
    points.assign(unique_points.begin(), unique_points.end());

    point3 c = centroid(points);

    // Sort the vector with lambda
    std::sort(points.begin(), points.end(), [&c](const point3& a, const point3& b) {
        return angle_from_centroid(c, a) < angle_from_centroid(c, b);
    });

    return points;
}

void print_points(const std::vector<point3>& points) {
    for (const auto& p : points) {
        std::cout << "[" << p.x << ", " << p.y << "] ";
    }
    std::cout << std::endl;
}
