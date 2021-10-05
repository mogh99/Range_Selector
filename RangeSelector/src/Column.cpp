#pragma once

#include <string>
#include <vector>

class Column {
public:
	bool isTimestamp = false;
	int colIdx;
	std::string name;
	std::vector<double> values;
};