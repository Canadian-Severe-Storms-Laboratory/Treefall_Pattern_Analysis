#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "CPP_CS_Interop.h"


namespace Utils
{
    EXPORT struct Range
    {
        double min;
        double max;

        double random(std::uniform_real_distribution<double>& dist, std::mt19937& gen) const {
            return min + dist(gen) * (max - min);
        }
    };

	static void writeBinaryFile(const std::vector<double>& data, const std::string& filename) {
        // Open a binary file for output
        std::ofstream outFile(filename, std::ios::binary);

        if (!outFile) {
            std::cerr << "Error opening file for writing." << std::endl;
            return;
        }

        // Write the size of the vector first
        size_t size = data.size();
        outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // Write the vector data
        outFile.write(reinterpret_cast<const char*>(data.data()), size * sizeof(double));

        outFile.close();
    }

    std::vector<double> readBinaryFile(const std::string& filename) {
        // Open the binary file for input
        std::ifstream inFile(filename, std::ios::binary);

        if (!inFile) {
            std::cerr << "Error opening file for reading." << std::endl;
            return {};
        }

        // Read the size of the vector
        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));

        // Read the vector data
        std::vector<double> data(size);
        inFile.read(reinterpret_cast<char*>(data.data()), size * sizeof(double));

        inFile.close();

        return data;
    }
}