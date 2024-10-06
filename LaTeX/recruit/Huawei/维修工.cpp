#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

struct Customer {
    int x;
    int y;
    int level;
};

// Function to calculate the Euclidean distance between two points
double distance(int x1, int y1, int x2, int y2) { 
    double res;
    res = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return res;
}

// Enumeration method to find the minimum total distance
double calculate_min_distance(int n, int k, int company_x, int company_y, vector<Customer>& customers) { 
    // Sort customers by priority (level)
    sort(customers.begin(), customers.end(), [](Customer a, Customer b) {
        return a.level < b.level;
    });

    // dp[i] represents the minimum total distance to serve the first i customers
    vector<double> dp(n + 1, numeric_limits<double>::max());
    dp[0] = 0.0; // Base case: nno customers served yet, distance is 0

    // Precompute distances from the company to each customer and between customers
    vector<vector<double>> dist(n + 1, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) { 
        dist[0][i + 1] = distance(company_x, company_y, customers[i].x, customers[i].y);
        dist[i + 1][0] = dist[0][i + 1]; // Return to company
        for (int j = 0; j < n; ++j) { 
            dist[i + 1][j + 1] = distance(customers[i].x, customers[i].y, customers[j].x, customers[j].y);
        }
    }

    // Enumerate all paths, trying every valid group of customers
    for (int i = 0; i < n; ++i) { 
        for (int j = i; j < n; ++j) { 
            int num_customers = j - i + 1;
            if (num_customers <= k) { 
                double current_dist = dp[i] + dist[0][i + 1];
                for (int p = i; p < j; ++p) { 
                    current_dist += dist[p + 1][p + 2];
                }
                current_dist += dist[j + 1][0];
                dp[j + 1] = min(dp[j + 1], current_dist);
            }
        }
    }

    return dp[n];
}

int main() {
    int n, k;
    cin >> n >> k;

    int company_x, company_y;
    cin >> company_x >> company_y;
    
    vector<Customer> customers(n);
    for (int i = 0; i < n; ++i) { 
        cin >> customers[i].x >> customers[i].y >> customers[i].level;
    }

    double result = calculate_min_distance(n, k, company_x, company_y, customers);
    printf("%.1f\n", result);

    return 0;
}