#if _NOEXPORT

#include <string>
#include <iostream>
#include "LinearPiecewiseVortex.h"
#include "BakerSterlingVortex.h"
#include "Transect.h"
#include "VecHashGrid.h"
#include "ConvergenceLine.h"
#include "ObservedPattern.h"
#include "AutoTransectFitter.h"
#include "VortexFactory.h"
#include <algorithm>
#include <vector>
#include <array>
#include <chrono>
#include <functional>
#include <thread>
#include <fstream>
#include "dbscan.h"


#define PI 3.14159265358979

static void timeIt(std::function<void()> f, int n=1) {
	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < n; i++) {
		f();
	}
	
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << elapsed.count() << " ms\n";
}




int main(){

	auto model = ModifiedRankineVortex(0.95, 0.9, 0.5, 0.25);

	//std::vector<std::array<double, 2>> pts;
	//pts.reserve(1000);

	//for (int i = 0; i < 1000; i++) {
	//	pts.push_back({ 10.0 * (double)rand() / RAND_MAX, 0.5 * (double)rand() / RAND_MAX + 0.5 });
	//}

	timeIt([&]() {model.solveAxesOfInterest(); auto p = model.patternCurve(15); }, 10000);

	//for (int i = 0; i < 1000; i++) {
	//	printf("Actual:%f  Aprox:%f\n", pow(pts[i][0], pts[i][1]), fastPow(pts[i][0], pts[i][1]));
	//}

	//timeIt([&]() {
	//	
	//	for (int i = 0; i < 1000; i++) {
	//		auto p = model.vecAt(pts[i][0], pts[i][1]);
	//	}
	//
	//}, 10000);
}

#endif