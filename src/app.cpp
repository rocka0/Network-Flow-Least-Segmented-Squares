/**
 * @file app.cpp
 * @brief Computes the optimal segmented least squares
 */

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief A type definition for long double
 */
typedef long double ld;

/**
 * @brief The epsilon value to be used for comparing two floating-point numbers
 */
constexpr ld eps = 1e-9;

/**
 * @brief Check if two floating-point numbers are equal within an epsilon value
 * @param x The first floating-point number
 * @param y The second floating-point number
 * @return True if the two numbers are equal within an epsilon value, false otherwise
 */
bool floatEq(ld x, ld y) {
    return abs(x - y) < eps;
}

/**
 * @brief Check if the first floating-point number is less than the second one within an epsilon value
 * @param x The first floating-point number
 * @param y The second floating-point number
 * @return True if the first number is less than the second one within an epsilon value, false otherwise
 */
bool floatLess(ld x, ld y) {
    return !floatEq(x, y) and x < y;
}

/**
 * @brief A 2D point with floating-point x and y coordinates
 */
struct Point {
    ld x;    ///< The x-coordinate of the point
    ld y;    ///< The y-coordinate of the point

    /**
     * @brief Default constructor
     */
    Point() {}

    /**
     * @brief Constructor that sets the x and y coordinates of the point
     * @param x The x-coordinate of the point
     * @param y The y-coordinate of the point
     */
    Point(ld x, ld y) : x(x), y(y) {}

    /**
     * @brief Compare two points based on their x-coordinates and y-coordinates
     * @param other The other point to compare with
     * @return True if this point is less than the other point, false otherwise
     */
    bool operator<(const Point &other) const {
        return floatLess(x, other.x) or (floatEq(x, other.x) and floatLess(y, other.y));
    }
};

/**
 * @brief Main function to find the segmented least-squares regression line for given set of points
 *
 * @return int The exit status of the program
 */
int main() {
    int n;
    cin >> n;

    /**
     * @brief Vector to store the points
     */
    vector<Point> points(n);

    /**
     * @brief A matrix to store the best fit error for every subsegment
     */
    vector<vector<ld>> bestFitError(n, vector<ld>(n));

    /**
     * @brief Read the points from the standard input
     */
    for (int i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        points[i] = Point(x, y);
    }

    /**
     * @brief Read the cost of inserting a segment
     */
    ld segmentCost;
    cin >> segmentCost;

    /**
     * @brief Sort the points in increasing order of x-coordinates
     */
    sort(points.begin(), points.end());

    /**
     * @brief Compute the best fit error for every subsegment of points
     */
    for (int i = 0; i < n; ++i) {
        ld sum_xi = points[i].x;
        ld sum_yi = points[i].y;
        ld sum_xi_yi = points[i].x * points[i].y;
        ld sum_xi_square = sum_xi * sum_xi;
        for (int j = i + 1; j < n; ++j) {
            sum_xi += points[j].x;
            sum_yi += points[j].y;
            sum_xi_yi += points[j].x * points[j].y;
            sum_xi_square += points[j].x * points[j].x;

            /**
             * @brief The slope of the best fit line for the current subsegment
             */
            ld bestFitSlope = (n * sum_xi_yi - sum_xi * sum_yi) / (n * sum_xi_square - sum_xi * sum_xi);

            /**
             * @brief The intercept of the best fit line for the current subsegment
             */
            ld bestFitIntercept = (sum_yi - bestFitSlope * sum_xi) / n;

            for (int k = i; k <= j; ++k) {
                /**
                 * @brief Update the best fit error for the current subsegment
                 */
                ld err = points[k].y - (bestFitSlope * points[k].x) - bestFitIntercept;
                bestFitError[i][j] += err * err;
            }
        }
    }

    /**
     * @brief A vector to store the minimum cost of segmented square fit for points 0..i
     */
    vector<ld> dp(n);

    /**
     * @brief The minimum cost of segmented square fit for points 0..0 is segmentCost
     */
    dp[0] = segmentCost;

    /**
     * @brief Compute the minimum cost for segmented square fit for each point
     */
    for (int i = 1; i < n; ++i) {
        dp[i] = bestFitError[0][i] + segmentCost;
        for (int x = 1; x < i; ++x) {
            dp[i] = min(dp[i], bestFitError[x][i] + segmentCost + dp[x - 1]);
        }
    }

    /**
     * @brief Pointer used to reconstruct the segments
     */
    int segmentEnd = n - 1;

    /**
     * @brief A vector used in the reconstruction of the segments
     */
    vector<pair<int, int>> segments;

    /**
     * @brief Reconstruct the segments from the computed minimum cost
     */
    while (segmentEnd >= 0) {
        int segmentStart = 0;
        ld minCost = bestFitError[0][segmentEnd] + segmentCost;
        for (int x = 1; x <= segmentEnd; ++x) {
            ld cost = bestFitError[x][segmentEnd] + segmentCost + dp[x - 1];
            if (floatLess(cost, minCost)) {
                minCost = cost;
                segmentStart = x;
            }
        }
        segments.push_back(make_pair(segmentStart, segmentEnd));
        segmentEnd = segmentStart - 1;
    }
    reverse(segments.begin(), segments.end());

    /**
     * @brief Print the number of segments and the starting and ending points of each segment
     */
    cout << segments.size() << endl;
    for (auto segment : segments) {
        cout << segment.first << " " << segment.second << endl;
    }
}
