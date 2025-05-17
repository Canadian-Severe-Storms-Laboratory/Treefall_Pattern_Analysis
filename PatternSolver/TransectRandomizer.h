#pragma once

#include "vec2.h"
#include "ConvergenceLine.h"
#include "Transect.h"
#include "ObservedPattern.h"
#include "VecHashGrid.h"
#include "Utils.h"

using namespace Utils;

EXPORT class TransectRandomizer
{
protected:
	VecHashGrid& vectorHashGrid;
	ConvergenceLine& convergenceLine;

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
	Range lengthAboveRange;
	Range lengthBelowRange;
	Range angleOffsetRange;
	Range heightOffsetRange;
	Range widthRange;
	Range spacingRange;

	TransectRandomizer(VecHashGrid& vectorHashGrid, ConvergenceLine& convergenceLine) : vectorHashGrid(vectorHashGrid), convergenceLine(convergenceLine) {}

	TransectRandomizer(Transect& transect, VecHashGrid& vectorHashGrid, ConvergenceLine& convergenceLine) : vectorHashGrid(vectorHashGrid), convergenceLine(convergenceLine) {
		positionOffsetRange = { std::max(0.0, transect.positionOffset - transect.width / 2.0), std::min(convergenceLine.length(), transect.positionOffset + transect.width / 2.0) };
		lengthAboveRange = { transect.lengthAbove * 0.9 , transect.lengthAbove * 1.1 };
		lengthBelowRange = { transect.lengthBelow * 0.9 , transect.lengthBelow * 1.1 };
		angleOffsetRange = { transect.angleOffset - 5.0, transect.angleOffset + 5.0 };
		heightOffsetRange = { -transect.spacing, transect.spacing };
		widthRange = { transect.width * 0.9, transect.width * 1.1 };
		spacingRange = { transect.spacing * 0.75, transect.spacing * 1.25 };
	}

	ObservedPattern rand(std::uniform_real_distribution<double>& dist, std::mt19937& gen) {
		int iters = 0;
		constexpr int maxIters = 1000;

		while (iters < maxIters) {

			iters++;

			Transect transect = randomTransect(dist, gen);

			if (transect.lengthAbove > transect.lengthBelow + transect.spacing) continue;

			double length = (transect.lengthAbove + transect.lengthBelow) / transect.spacing;

			if (10 > length || length > 20) continue;

			ObservedPattern observedPattern(transect.lengthAbove, transect.lengthBelow, transect.spacing, vectorHashGrid.query(transect));

			if (!observedPattern.isMissingVectors() && observedPattern.isConverging()) return observedPattern;
		}

		return ObservedPattern();
	}
};