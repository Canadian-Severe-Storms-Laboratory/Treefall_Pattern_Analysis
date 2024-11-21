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

	auto model = SullivanVortex();

	std::vector<double> rmaxPrecentages;

	for (int Vr = 20; Vr < 80; Vr++) {
		for (int Vt = 1; Vt < 60; Vt++) {
			for (int Vs = 5; Vs < 25; Vs++) {
				for (int Vc = 30; Vc < 60; Vc++) {

					model.Vr = (double)Vr / (double)Vc;
					model.Vt = (double)Vt / (double)Vc;
					model.Vs = (double)Vs / (double)Vc;

					if (!model.hasPattern() || model.isOuterType()) continue;

					model.solveAxesOfInterest();

					double rp = 1.0 / model.length();

					if (rp < 1.0) {
						rmaxPrecentages.push_back(rp);
					}
				}
			}
		}
	}

	std::ofstream myfile;
	myfile.open("rmax.csv");

	for (int i = 0; i < rmaxPrecentages.size(); i++) {
		myfile << rmaxPrecentages[i] << "\n";
	}
	myfile.close();

	//std::vector<std::array<double, 2>> pts;
	//pts.reserve(1000);

	//for (int i = 0; i < 1000; i++) {
	//	pts.push_back({ 10.0 * (double)rand() / RAND_MAX, 0.5 * (double)rand() / RAND_MAX + 0.5 });
	//}

	//timeIt([&]() {model.solveAxesOfInterest(); auto p = model.patternCurve(15); }, 10000);

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