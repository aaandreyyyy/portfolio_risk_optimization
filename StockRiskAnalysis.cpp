
#include "StockRiskAnalysis.h"

#include <utility>
#include <iostream>
#include "StockData.h"


std::vector<std::vector<float>> CalculateInverseMatrix(const std::vector<std::vector<float>> &matrix) {
    size_t n = matrix.size();
    std::vector<std::vector<float>> augmentedMatrix(n, std::vector<float>(2 * n, 0.0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
            augmentedMatrix[i][j + n] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        if (augmentedMatrix[i][i] == 0.0) {
            int swapRow = -1;
            for (size_t j = i + 1; j < n; ++j) {
                if (augmentedMatrix[j][i] != 0.0) {
                    swapRow = j;
                    break;
                }
            }
            if (swapRow == -1) {
                std::cerr << "Convolution matrix is not invertible.\nTry again with different set of stocks/period." << std::endl;
            }

            std::swap(augmentedMatrix[i], augmentedMatrix[swapRow]);
        }

        float scale = augmentedMatrix[i][i];
        for (size_t j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] = augmentedMatrix[i][j] / scale;
        }

        for (size_t j = 0; j < n; ++j) {
            if (i != j) {
                float factor = augmentedMatrix[j][i];
                for (size_t k = 0; k < 2 * n; ++k) {
                    augmentedMatrix[j][k] = augmentedMatrix[j][k] - factor * augmentedMatrix[i][k];
                }
            }
        }
    }

    std::vector<std::vector<float>> inverseMatrix(n, std::vector<float>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverseMatrix[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return inverseMatrix;
}

std::vector<float> StockRiskAnalysis::CountWealthPortions() {
    size_t m = stocks.size();

    std::vector<std::vector<float>> covariance_matrix(m, std::vector<float>(m));

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            covariance_matrix[i][j] = CalculateCov(*stocks[i], *stocks[j]);
        }
    }

    std::vector<float> expected_values(m);
    for (size_t i = 0; i < m; ++i) {
        expected_values[i] = stocks[i]->ExpectedValue();
    }

    auto inverse_covariance_matrix = CalculateInverseMatrix(covariance_matrix);

    std::vector<float> weight_vector(m);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < m; ++j) {
            weight_vector[i] += inverse_covariance_matrix[i][j] * expected_values[j];
        }
    }

    float weight_sum = 0.0;
    for (size_t i = 0; i < m; ++i) {
        weight_sum += std::abs(weight_vector[i]);
    }

    for (size_t i = 0; i < m; ++i) {
        weight_vector[i] /= weight_sum;
    }
    return weight_vector;
}
