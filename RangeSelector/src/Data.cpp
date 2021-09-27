#ifndef DATA
#define DATA

#include <string>
#include <vector>

#include "Column.cpp"

class Data {
public:
	int numberOfRows;
	int numberOfColumns;
	std::string fileName;
	std::vector<Column> columns;
};

#endif