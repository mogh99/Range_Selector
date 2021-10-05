#pragma once

#include <string>
#include <vector>

#include "Column.cpp"

class Data {
public:
	int numberOfRows;
	int numberOfColumns;
	int timestampIdx;
	std::string fileName;
	std::vector<Column> columns;
};