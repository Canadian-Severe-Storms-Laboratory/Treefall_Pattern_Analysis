#pragma once
#include <string>
#include "CPP_CS_Interop.h"

EXPORT class Monitor
{
public:
	double value;
	double min;
	double max;
	std::string title;
	std::string message;
	std::string warning;
	bool cancelled = false;

	Monitor() :  title(""), value(0), min(0), max(1), message(""), warning("") {};
};
