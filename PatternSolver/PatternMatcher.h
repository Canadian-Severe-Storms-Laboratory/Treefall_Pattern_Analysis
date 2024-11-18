#pragma once
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <thread>
#include "CPP_CS_Interop.h"
#include "VortexFactory.h"
#include "ObservedPattern.h"
#include "Utils.h"
#include "Monitor.h"

using namespace Utils;

EXPORT class PatternMatcher
{
private:

	struct RatioRange
	{
		double min;
		double max;
		double step;

		RatioRange(Range range, double divisor) {
		
			min = range.min / divisor;
			max = range.max / divisor;
			step = (max - min) / 31; //32 iterations	
		}

		RatioRange(Range range, Range divisor) {

			min = range.min / divisor.max;
			max = range.max / divisor.min;
			step = (max - min) / 31; //32 iterations	
		}
	};

	double patternError(ObservedPattern& obsPattern, VortexModel& model) {
	
		double errorSum = 0.0;
		double weightSum = 0.0;
		int centerIdx = obsPattern.centerIdx();
		double dx = obsPattern.spacing * model.length() / obsPattern.length();

		// left
		double d = 1.0 / ((obsPattern.lengthAbove + obsPattern.spacing) * model.length() / obsPattern.length());
		double x = model.Xc - dx;

		for (int i = centerIdx - 1; i > 0; i--) {
			Vec2 simVec = model.patternVec(x).rot90CW();

			const double w = 1.0 - (model.Xc - x) * d;

			errorSum += w * simVec.angleBetweenUnit(obsPattern.vecs[i]);

			weightSum += w;
			x -= dx;
		}

		// center
		weightSum += 1.0;
		Vec2 simVec1 = model.patternVec(model.Xc - 0.001).rot90CW();
		Vec2 simVec2 = model.patternVec(model.Xc + 0.001).rot90CW();

		errorSum += std::min(simVec1.angleBetweenUnit(obsPattern.vecs[centerIdx]), simVec2.angleBetweenUnit(obsPattern.vecs[centerIdx]));

		// right
		d = 1.0 / ((obsPattern.lengthBelow + obsPattern.spacing) * model.length() / obsPattern.length());
		x = model.Xc + dx;
		
		for (int i = centerIdx + 1; i < obsPattern.size() - 1; i++) {
			Vec2 simVec = model.patternVec(x).rot90CW();

			const double w = 1.0 - (x - model.Xc) * d;

			errorSum += w * simVec.angleBetweenUnit(obsPattern.vecs[i]);

			weightSum += w;
			x += dx;
		}

		return (errorSum / weightSum) / 3.14159265358979323;
	}

	bool isCorrectType(VortexModel& model) {
		if (patternType == 0) return true;

		if (patternType == 1) return model.isOuterType();

		return !model.isOuterType();
	}


public:
	std::vector<double> models;
	Range VrRange;
	Range VtRange;
	Range VsRange;
	Range VcRange;
	Monitor monitor;
	double matchThreshold = 0.1;
	int patternType = 0;
	int numSimulations = 10000;

	double bestMatchScale = 1.0;

	std::vector<double> minVelResults;
	std::vector<double> bestSwirlResults;

	PatternMatcher(Range VrRange, Range VtRange, Range VsRange, Range VcRange) : VrRange(VrRange), VtRange(VtRange), VsRange(VsRange), VcRange(VcRange) {}


	double bestMatchError(ObservedPattern obsPattern) {
		const RatioRange VrRatioRange(VrRange, VcRange);
		const RatioRange VtRatioRange(VtRange, VcRange);
		const RatioRange VsRatioRange(VsRange, VcRange);
		double minError = 1E100;

		#pragma omp parallel for schedule(dynamic) num_threads((int)(std::thread::hardware_concurrency()*0.8))
		for (int i = 0; i < 32; i++) {

			const double Vr = VrRatioRange.min + VrRatioRange.step * i;

			for (double Vt = VtRatioRange.min; Vt <= VtRatioRange.max; Vt += VtRatioRange.step) {
				for (double Vs = VsRatioRange.min; Vs <= VsRatioRange.max; Vs += VsRatioRange.step) {

					for (int j = 0; j < models.size(); j++) {
						auto model = VortexFactory::fromIndex(models[j], Vr, Vt, Vs);

						if (!model->hasPattern() || !isCorrectType(*model)) continue;

						model->solveAxesOfInterest();

						const double Rmax = obsPattern.length() / model->length();

						if (0.1 * obsPattern.length() > Rmax || Rmax > 0.5 * obsPattern.length()) continue; //Rmax must be between 10% and 50% of the observed pattern's length

						if (fabs(obsPattern.lengthAbove - model->lengthAbove() * Rmax) > obsPattern.spacing ||
							fabs(obsPattern.lengthBelow - model->lengthBelow() * Rmax) > obsPattern.spacing) continue;

						const double error = patternError(obsPattern, *model);

						#pragma omp critical
						{
							minError = std::min(minError, error);
						}
					}
				}
			}
		}
	
		return minError;
	}

	Pattern bestMatch(ObservedPattern obsPattern) {

		const RatioRange VrRatioRange(VrRange, VcRange);
		const RatioRange VtRatioRange(VtRange, VcRange);
		const RatioRange VsRatioRange(VsRange, VcRange);

		double minError = 1E100;

		std::unique_ptr<VortexModel> bestModel;

		#pragma omp parallel for schedule(dynamic) num_threads((int)(std::thread::hardware_concurrency()*0.8))
		for (int i = 0; i < 32; i++) {

			const double Vr = VrRatioRange.min + VrRatioRange.step * i;

			for (double Vt = VtRatioRange.min; Vt <= VtRatioRange.max; Vt += VtRatioRange.step) {
				for (double Vs = VsRatioRange.min; Vs <= VsRatioRange.max; Vs += VsRatioRange.step) {

					for (int j = 0; j < models.size(); j++) {
						auto model = VortexFactory::fromIndex(models[j], Vr, Vt, Vs);

						if (!model->hasPattern() || !isCorrectType(*model)) continue;

						model->solveAxesOfInterest();

						const double Rmax = obsPattern.length() / model->length();

						if (0.1 * obsPattern.length() > Rmax || Rmax > 0.5 * obsPattern.length()) continue; //Rmax must be between 10% and 50% of the observed pattern's length

						if (fabs(obsPattern.lengthAbove - model->lengthAbove() * Rmax) > obsPattern.spacing ||
							fabs(obsPattern.lengthBelow - model->lengthBelow() * Rmax) > obsPattern.spacing) continue;

						const double error = patternError(obsPattern, *model);

						#pragma omp critical
						{
							if (error < minError) {
								minError = error;
								bestModel = std::move(model); //cannot copy unique_ptr
							}
						}
					}
				}
			}
		}

		if (!bestModel->hasPattern()) return Pattern();

		bestMatchScale = obsPattern.length() / bestModel->length();

		return bestModel->pattern(obsPattern.spacing / bestMatchScale);
	}


	void monteCarloMatching(ObservedPattern obsPattern) {

		const int ITERS = numSimulations;

		monitor.max = ITERS;
		monitor.title = "Monte Carlo Simulation";
		monitor.message = "Matching...";
		monitor.value = 0;

		minVelResults.clear();
		bestSwirlResults.clear();

		minVelResults.reserve(ITERS);
		bestSwirlResults.reserve(ITERS);

		#pragma omp parallel num_threads((int)(std::thread::hardware_concurrency()*0.8))
		while (minVelResults.size() < ITERS && !monitor.cancelled) {

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<double> dist(0.0, 1.0);

			const double Vc = VcRange.random(dist, gen);
			const double Vs = VsRange.random(dist, gen) / Vc;

			const RatioRange VrRatioRange(VrRange, Vc);
			const RatioRange VtRatioRange(VtRange, Vc);

			double minVel = 1E308;
			double bestSwirl = 1E308;
			double minError = 1E308;

			auto model = VortexFactory::randomModel(models, dist, gen);
			model->Vs = Vs;

			for (double Vr = VrRatioRange.min; Vr <= VrRatioRange.max; Vr += VrRatioRange.step) {
				for (double Vt = VtRatioRange.min; Vt <= VtRatioRange.max; Vt += VtRatioRange.step) {

					//BakerSterlingVortex model(Vr, Vt, Vs);
					model->Vr = Vr;
					model->Vt = Vt;

					if (!model->hasPattern() || !isCorrectType(*model)) continue;

					model->solveAxesOfInterest();

					const double Rmax = obsPattern.length() / model->length();

					if (0.1 * obsPattern.length() > Rmax || Rmax > 0.5 * obsPattern.length()) continue; //Rmax must be between 10% and 50% of the observed pattern's length

					if (fabs(obsPattern.lengthAbove - model->lengthAbove() * Rmax) > obsPattern.spacing ||
						fabs(obsPattern.lengthBelow - model->lengthBelow() * Rmax) > obsPattern.spacing) continue;

					const double error = patternError(obsPattern, *model);

					if (error > matchThreshold) continue;

					const double Vmax = model->vmax() * Vc;

					#pragma omp critical 
					{	
						minVel = std::min(Vmax, minVel);

						if (error < minError) {
							minError = error;
							bestSwirl = model->swirlRatio();
						}
					}
				}
			}

			model.reset();

			if (minError > matchThreshold) continue; 

			#pragma omp critical
			{
				minVelResults.push_back(minVel);
				bestSwirlResults.push_back(bestSwirl);
				monitor.value++;
			}
		}

		std::sort(minVelResults.begin(), minVelResults.end());
		std::sort(bestSwirlResults.begin(), bestSwirlResults.end());
	}

};