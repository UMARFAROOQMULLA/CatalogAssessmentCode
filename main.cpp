#include<bits/stdc++.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

int convertToDecimal(const std::string& valueStr, int base) {
    std::cout << "Converting '" << valueStr << "' from base " << base << " to decimal" << std::endl;
    int result = 0;
    for (char digit : valueStr) {
        int digitValue;
        if (digit >= '0' && digit <= '9') {
            digitValue = digit - '0';
        } else if (digit >= 'A' && digit <= 'F') {
            digitValue = 10 + (digit - 'A');
        } else if (digit >= 'a' && digit <= 'f') {
            digitValue = 10 + (digit - 'a');
        } else {
            throw std::invalid_argument("Invalid character in input: " + std::string(1, digit));
        }
        
        if (digitValue >= base) {
            throw std::invalid_argument("Digit " + std::to_string(digitValue) + " is not valid in base " + std::to_string(base));
        }
        
        result = result * base + digitValue;
    }
    std::cout << "Conversion result: " << result << std::endl;
    std::cout<<std::endl<<std::endl;
    return result;
}

std::string trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return start == std::string::npos ? "" : str.substr(start, end - start + 1);
}

double lagrangeInterpolation(const std::vector<int>& x, const std::vector<int>& y) {
    double c = 0.0;
    int n = x.size();

    for (int i = 0; i < n; ++i) {
        double term = y[i];
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= static_cast<double>(0 - x[j]) / (x[i] - x[j]);
            }
        }
        c += term; // Add the contribution of each term to c
    }
    
    return std::round(c); // Round the result to get an integer value for c
}

void processJson(const std::string& jsonInput) {
    int n = 0, k = 0;
    std::vector<int> xValues;
    std::vector<int> yValues;

    std::istringstream stream(jsonInput);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find("\"n\":") != std::string::npos) {
            std::string nStr = line.substr(line.find(':') + 1);
            nStr = trim(nStr);
            n = std::stoi(nStr);
        }
        if (line.find("\"k\":") != std::string::npos) {
            std::string kStr = line.substr(line.find(':') + 1);
            kStr = trim(kStr);
            k = std::stoi(kStr);
        }

        // Process base and value
        if (line.find("base") != std::string::npos) {
            std::string baseStr = line.substr(line.find(':') + 1);
            baseStr = trim(baseStr);
            baseStr.erase(std::remove(baseStr.begin(), baseStr.end(), '\"'), baseStr.end());
            baseStr.erase(std::remove(baseStr.begin(), baseStr.end(), ','), baseStr.end());

            try {
                int base = std::stoi(baseStr);

                // Read the next line for the value
                std::getline(stream, line);
                std::string valueStr = line.substr(line.find(':') + 1);
                valueStr = trim(valueStr);
                valueStr.erase(std::remove(valueStr.begin(), valueStr.end(), '\"'), valueStr.end());

                std::cout << "Base String: '" << baseStr << "', Value String: '" << valueStr << "'" << std::endl;

                int y = convertToDecimal(valueStr, base);
                
                // Use the current key number for x values (1, 2, 3, 4)
                int x = xValues.size() + 1; // Adjust the way x is assigned
                xValues.push_back(x);
                yValues.push_back(y);

                std::cout << "Extracted x: " << x << ", y: " << y << " (base: " << base << ", value: " << valueStr << ")" << std::endl;
                std::cout<<std::endl<<std::endl<<std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error processing input: " << e.what() << std::endl;
            }
        }
    }

    std::cout << "n: " << n << ", k: " << k << ", degree of polynomial is: " << (k - 1) << std::endl;

    if (!xValues.empty() && !yValues.empty()) {
        double constantTerm = lagrangeInterpolation(xValues, yValues);
        std::cout << "Constant term (c) of the polynomial: " << constantTerm << std::endl;
    } else {
        std::cout << "Not enough valid points to perform interpolation." << std::endl;
    }
}

int main() {
    std::string jsonInput1 = R"(
    {
        "keys": {
            "n": 4,
            "k": 3
        },
        "1": {
            "base": "10",
            "value": "4"
        },
        "2": {
            "base": "2",
            "value": "111"
        },
        "3": {
            "base": "10",
            "value": "12"
        },
        "4": {
            "base": "4",
            "value": "213"
        }
    })";

    std::string jsonInput2 = R"(
    {
        "keys": {
        "n": 9,
        "k": 6
    },
    "1": {
        "base": "10",
        "value": "28735619723837"
    },
    "2": {
        "base": "16",
        "value": "1A228867F0CA"
    },
    "3": {
        "base": "12",
        "value": "32811A4AA0B7B"
    },
    "4": {
        "base": "11",
        "value": "917978721331A"
    },
    "5": {
        "base": "16",
        "value": "1A22886782E1"
    },
    "6": {
        "base": "10",
        "value": "28735619654702"
    },
    "7": {
        "base": "14",
        "value": "71AB5070CC4B"
    },
    "8": {
        "base": "9",
        "value": "122662581541670"
    },
    "9": {
        "base": "8",
        "value": "642121030037605"
    }
    })";

    std::cout << "Processing first JSON input:" << std::endl;
    processJson(jsonInput1);
    std::cout<<"---------------------------------------------------------------------------"<<std::endl;
    std::cout << "\nProcessing second JSON input:" << std::endl;
    processJson(jsonInput2);

    return 0;
}
