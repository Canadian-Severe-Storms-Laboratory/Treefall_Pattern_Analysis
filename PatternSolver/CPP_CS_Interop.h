#pragma once
#include <vector>
#include <string>
#include <span>

#if _NOEXPORT 
#define EXPORT 
#else
#define EXPORT __declspec(dllexport)
#endif

EXPORT std::span<double> createSpan(double* spanArray, int size) {
	return std::span<double>(spanArray, size);
}

EXPORT int getSpanSize(std::span<double> span) {
	return span.size();
}

EXPORT double* getSpanPtr(std::span<double> span) {
	return span.data();
}

EXPORT void freeSpan(std::span<double> span) {
	delete[] span.data();
}

EXPORT std::vector<double> createVector(double* sourceArray, int size) {
	std::vector<double> result(sourceArray, sourceArray + size);

	return result;
}

EXPORT double* getVector(std::vector<double>& sourceVector) {
	return sourceVector.data();
}