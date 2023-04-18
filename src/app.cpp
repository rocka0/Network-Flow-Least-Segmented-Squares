#include <iostream>

using namespace std;

typedef long double ld;

constexpr ld eps = 1e-9;

bool floatEq(ld x, ld y) {
    return abs(x - y) < eps;
}

bool floatLess(ld x, ld y) {
    return !floatEq(x, y) and x < y;
}

struct Point {
    ld x;
    ld y;

    Point() {}

    Point(ld x, ld y) : x(x), y(y) {}

    bool operator<(const Point &other) const {
        return floatLess(x, other.x) or (floatEq(x, other.x) and floatLess(y, other.y));
    }
};

int main() {
    int n;
    cin >> n;

    vector<Point> points(n);
    vector<vector<ld>> bestFitError(n, vector<ld>(n));

    for (int i = 0; i < n; ++i) {
        ld x, y;
        cin >> x >> y;
        points[i] = Point(x, y);
    }

    ld segmentCost;
    cin >> segmentCost;

    sort(points.begin(), points.end());

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
            ld bestFitSlope = (n * sum_xi_yi - sum_xi * sum_yi) / (n * sum_xi_square - sum_xi * sum_xi);
            ld bestFitIntercept = (sum_yi - bestFitSlope * sum_xi) / n;
            for (int k = i; k <= j; ++k) {
                ld err = points[k].y - (bestFitSlope * points[k].x) - bestFitIntercept;
                bestFitError[i][j] += err * err;
            }
        }
    }

    vector<ld> dp(n);
    // dp[i] is the minimum cost of segmented square fit for points 0 .. i

    for (int i = 2; i < n; ++i) {
        dp[i] = bestFitError[0][i] + segmentCost;
        for (int x = 1; x < i; ++x) {
            dp[i] = min(dp[i], bestFitError[x][i] + segmentCost + dp[x - 1]);
        }
    }

    int segmentEnd = n - 1;
    vector<pair<int, int>> segments;
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

    cout << segments.size() << endl;
    for (auto segment : segments) {
        cout << segment.first << " " << segment.second << endl;
    }
}
