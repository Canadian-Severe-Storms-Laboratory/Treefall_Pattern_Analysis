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

#define MINIMUM_POINTS 4     // minimum number of cluster
#define EPSILON (1)  // distance for clustering, metre^2

void printResults(vector<Point>& points, int num_points)
{
    int i = 0;
    printf("Number of points: %u\n"
        " x     y     z     cluster_id\n"
        "-----------------------------\n"
        , num_points);
    while (i < num_points)
    {
        printf("%5.2lf %5.2lf %5.2lf: %d\n",
            points[i].x,
            points[i].y, points[i].z,
            points[i].clusterID);
        ++i;
    }
}

int main(){

	//LinearPiecewiseVortex lpv = LinearPiecewiseVortex({ 1.0, 1.0, 0.0, 10.0, -1.0/9.0, 1.0 + 1.0/9.0 }, { 1.0, 1.0, 0.0, 10.0, -1.0 / 9.0, 1.0 + 1.0 / 9.0 });

	/*VecHashGrid vhg = VecHashGrid({ 0, 0, 1, 1,   2, 3, 2, 4,   1, 2, 0, 1 }, 10);

	auto vecs = vhg.query(RotatedRect(0, 0, 2, 2, PI/4.0));

	for (int i = 0; i < vecs.size(); i+=4) {
		printf("%.2f, %.2f, %.2f, %.2f\n", vecs[i], vecs[i + 1], vecs[i + 2], vecs[i + 3]);
	}*/

	/*timeIt([&] {
		BakerSterlingVortex model(70.0 / 40.0, 30.0 / 40.0, 15.0 / 40.0);
		model.solveAxesOfInterest();
	}, 32768);*/

	//timeIt([&] {
	//	auto model2 = VortexFactory::BakerSterlingLPVortex(70.0 / 40.0, 30.0 / 40.0, 15.0 / 40.0);
	//	model2.solveAxesOfInterest();
	//}, 32768);

    vector<Point> points;

// generate random data
	for (int i = 0; i < 1000; ++i)
	{
		Point p;
		p.x = 10.0 * (double)rand() / RAND_MAX;
		p.y = 10.0 * (double)rand() / RAND_MAX;
		p.z = 0.0;
		p.clusterID = UNCLASSIFIED;

		points.push_back(p);
	}

	timeIt([&] {
		// constructor
		DBSCAN ds(MINIMUM_POINTS, EPSILON, points);

		// main loop
		ds.run();
	}, 100);

    

    // result of DBSCAN algorithm
   // printResults(ds.m_points, ds.getTotalPointSize());

}

#endif