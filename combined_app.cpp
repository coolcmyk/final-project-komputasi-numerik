#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <random>

/**
 * @file combined_app.cpp
 * @brief Generates synthetic spring data, saves it to CSV, and immediately analyzes it.
 * @author Ryan Adidaru
 * @NPM 2306266994
 */

// --- Data Generation Function ---
void generate_data_to_file(const std::string& filename, double k_true, int num_points, double noise_level) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, noise_level);
    std::ofstream file(filename);
    file << "displacement_m,force_N\n";
    std::cout << "Generating " << num_points << " data points..." << std::endl;
    for (int i = 1; i <= num_points; ++i) {
        double x = i * 0.025;
        double y_perfect = k_true * x;
        double y_noisy = y_perfect + d(gen);
        file << std::fixed << std::setprecision(4) << x << "," << y_noisy << "\n";
    }
    file.close();
    std::cout << "Data successfully generated and saved to " << filename << std::endl;
}

// --- Analysis Function ---
struct RegressionResult { double slope, intercept, r_squared; };
RegressionResult linearRegression(const std::vector<double>& x, const std::vector<double>& y) {
    int n = x.size();
    if (n == 0) return {0, 0, 0};
    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    double sum_x_sq = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x_sq - sum_x * sum_x);
    double mean_x = sum_x / n;
    double mean_y = sum_y / n;
    double intercept = mean_y - slope * mean_x;
    double ss_tot = 0.0, ss_res = 0.0;
    for (int i = 0; i < n; ++i) {
        ss_res += pow(y[i] - (intercept + slope * x[i]), 2);
        ss_tot += pow(y[i] - mean_y, 2);
    }
    double r_squared = (ss_tot > 0) ? (1.0 - (ss_res / ss_tot)) : 1.0;
    return {slope, intercept, r_squared};
}

int main() {
    // --- Part 1: Generate Data ---
    const std::string filename = "data.csv";
    generate_data_to_file(filename, 200.0, 20, 1.5);

    // --- Part 2: Read and Analyze Data ---
    std::cout << "\n--- Starting Analysis ---\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return 1;
    }
    std::vector<double> x_data, y_data;
    std::string line, word;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, word, ',');
        x_data.push_back(std::stod(word));
        std::getline(ss, word, ',');
        y_data.push_back(std::stod(word));
    }
    file.close();

    RegressionResult result = linearRegression(x_data, y_data);
    
    // --- Part 3: Print Results ---
    std::cout << "==========================================================" << std::endl;
    std::cout << "      Combined C++ App: Analysis Results                  " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Data Points (n)           : " << x_data.size() << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Konstanta Pegas (k) [Slope]: " << result.slope << " N/m" << std::endl;
    std::cout << "Intercept (a0)              : " << result.intercept << " N" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Koefisien Determinasi (R^2): " << result.r_squared << std::endl;
    std::cout << "==========================================================" << std::endl;

    return 0;
}
