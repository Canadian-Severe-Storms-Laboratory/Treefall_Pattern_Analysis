#pragma once
#include <vector>
#include <array>
#include <string>

struct LineNode {
    double maxR;
    double ar;
    double br;
    double at;
    double bt;
};

struct SplineTable {
    std::vector<LineNode> table;

    SplineTable() {};

    SplineTable(std::vector<LineNode> table_) : table(std::move(table_)) {
        table.emplace_back(1e100, 0.0, 0.0, 0.0, 0.0);
    }

    static const bool compareR(const LineNode& node, const double value) {
        return node.maxR < value;
    }

    LineNode& operator[](const int i) {
        return table[i];
    }

    LineNode& operator[](const double x) {
        return *std::lower_bound(table.begin(), table.end(), x, &SplineTable::compareR); //binary search
    }

    constexpr size_t size() const {
        return table.size();
    }

    constexpr std::vector<LineNode>::iterator begin() {
        return table.begin();
    }

    constexpr std::vector<LineNode>::iterator end() {
        return table.end();
    }

    void emplace_back(const double maxR, const double ar, const double br, const double at, const double bt) {
        table.emplace_back(maxR, ar, br, at, bt);
    }

    void reserve(const size_t n) {
        table.reserve(n);
    }

    std::string toString() const {
		std::string str = "{ ";

		for (const auto& node : table) {
			str += "{ " + std::to_string(node.maxR) + ", " + std::to_string(node.ar) + ", " + std::to_string(node.br) + ", " + std::to_string(node.at) + ", " + std::to_string(node.bt) + " },\n";
		}

        str.erase(str.length() - 2);

		str += " }";

		return str;
	}

};