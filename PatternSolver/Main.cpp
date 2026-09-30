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

template <typename F>
static void timeIt(F f, int n=1) {
	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < n; i++) {
		f();
	}
	
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << elapsed.count() << " ms\n";
}

void show(double result, double expected)
{
	std::cout << "Numerical: " << std::setprecision(20) << std::fixed << result
		<< "\tExpected: " << expected << "\tDiff: "
		<< std::scientific << std::setprecision(6) << std::setw(15)
		<< std::abs((result - expected) / expected) << '\n';
}

int main(){

	//double Vc = 30.0;

	//auto model = BakerSterlingVortex(60.0 / Vc, 35.0 / Vc, 25.0 / Vc);

	//timeIt([&]() {model.solveAxesOfInterest(); auto p = model.pattern(15); }, 100000);

	//double y = model.patternLocation(-2.0);

	//std::cout << y << "\n";

	auto model = ModifiedRankineVortex(0.5, 1.3333333, 0.9375, 0.16501);
	model.solveAxesOfInterest();

	double x = model.vecAt(model.Xc, model.patternLocation(model.Xc)).x;

	std::cout << model.Xc << ", " << x << "\n";
}

#endif