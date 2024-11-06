#pragma once
#include <thread>
#include <vector>
#include <array>
#include <algorithm>
#include "CPP_CS_Interop.h"
#include "vec2.h"
#include "ConvergenceLine.h"
#include "Transect.h"
#include "ObservedPattern.h"
#include "VecHashGrid.h"
#include "Utils.h"
#include "PatternMatcher.h"
#include "dbscan.h"
#include "Monitor.h"

using namespace Utils;

EXPORT class AutoTransectFitter
{
private:
	VecHashGrid& vectorHashGrid;
	ConvergenceLine& convergenceLine;

	std::vector<Transect> foundTransects;

	Transect randomTransect(std::uniform_real_distribution<double>& dist, std::mt19937& gen) {

		double positionOffset = positionOffsetRange.random(dist, gen);

		auto position = convergenceLine.query(positionOffset);

		Transect transect(position[0], position[1], lengthAboveRange.random(dist, gen), lengthBelowRange.random(dist, gen), widthRange.random(dist, gen));

		transect.positionOffset = positionOffset;
		transect.angleOffset = angleOffsetRange.random(dist, gen);
		transect.heightOffset = heightOffsetRange.random(dist, gen);
		transect.spacing = floor(spacingRange.random(dist, gen) + 0.5);

		transect.setPerpendicularAngle(position[2]);

		return transect;
	}

public:

	Range positionOffsetRange;
	Range lengthAboveRange{ 200.0, 400.0 };
	Range lengthBelowRange{ 200.0, 400.0 };
	Range angleOffsetRange{ -22.5, 22.5 };
	Range heightOffsetRange{ -50.0, 50.0 };
	Range widthRange{50.0, 100.0};
	Range spacingRange{30.0, 50.0};
	Monitor monitor;
	double matchThreshold = 0.1;
	int maxIters = 1000000;
	bool failed = false;

	AutoTransectFitter(VecHashGrid& vectorHashGrid, ConvergenceLine& convergenceLine) : vectorHashGrid(vectorHashGrid), convergenceLine(convergenceLine) {
		positionOffsetRange = Range{ 0.0, convergenceLine.length() };
	}

	int resultsSize() {
		return foundTransects.size();
	}

	Transect getResult(int index) {
		return foundTransects[index];
	}

	void randomClusterSearch() {

		constexpr int count = 1000;

		monitor.title = "Random Cluster Search";
		monitor.value = 0;
		monitor.max = count;
		monitor.message = "Finding Seed Transects...";

		foundTransects.clear();
		failed = false;

		std::vector<Transect> initialTransects;
		initialTransects.reserve(count);

		std::vector<Point> points;
		points.reserve(count);

		#pragma omp parallel for schedule(dynamic) num_threads((int)(std::thread::hardware_concurrency()*0.8))
		for (int i = 0; i < count; i++) {

			if (monitor.cancelled) continue;

			Transect transect = randomSearch();

			if (transect.width < 0) continue;
			
			#pragma omp critical
			{
				initialTransects.push_back(transect);
				points.emplace_back(transect.x, transect.y, 0.0, UNCLASSIFIED);
				monitor.value = std::max(monitor.value, (double)i);
			}
		}

		if (monitor.cancelled || initialTransects.empty()) {
			failed = true;
			return;
		}

		monitor.message = "Clustering...";

		DBSCAN ds(1, 2.0 * widthRange.max, points);
		const int numClusters = ds.run();

		points = ds.m_points;

		std::vector<std::vector<Transect>> clusters(numClusters);

		for (int i = 0; i < count; i++) {

			if (points[i].clusterID <= 0) continue;

			clusters[points[i].clusterID - 1].push_back(initialTransects[i]);
		}

		if (monitor.cancelled) return;

		monitor.value = 0;
		monitor.max = numClusters-1;
		monitor.message = "Matching...";

		PatternMatcher matcher({ 20.0, 80.0 }, { 1.0, 60.0 }, { 5.0, 25.0 }, { 30.0, 60.0 });

		for (auto& cluster : clusters) {
			monitor.value++;
			if (cluster.empty()) continue;

			Transect bestTransect = cluster[0];
			double minError = 1e100;

			for (auto& transect : cluster) {

				if (monitor.cancelled) return;

				ObservedPattern observedPattern(transect.lengthAbove, transect.lengthBelow, transect.spacing, vectorHashGrid.query(transect));
				double error = matcher.bestMatchError(observedPattern);

				if (error < minError) {
					minError = error;
					bestTransect = transect;
				}
			}

			if (minError < matchThreshold) {
				foundTransects.push_back(bestTransect);
			}			
		}

		failed = foundTransects.empty();
	}

	Transect randomSearch() {

		Transect transect;
		int iters = 0;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1.0);

		while (iters < maxIters) {

			iters++;

			transect = randomTransect(dist, gen);

			if (transect.lengthAbove > transect.lengthBelow + transect.spacing) continue;

			double length = (transect.lengthAbove + transect.lengthBelow) / transect.spacing;

			if (10 > length || length > 20) continue;

			ObservedPattern observedPattern(transect.lengthAbove, transect.lengthBelow, transect.spacing, vectorHashGrid.query(transect));

			if (observedPattern.isValid()) break;
		}

		return transect;
	}

};