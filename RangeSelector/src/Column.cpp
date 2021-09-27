#ifndef COLUMN
#define COLUMN

#include <string>
#include <vector>

class Column {
public:
	bool isTimeStamp = false;
	int colIdx;
	std::string name;
	std::vector<double> values;
};

#endif