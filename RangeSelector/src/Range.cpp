#ifndef RANGE
#define RANGE

#include <string>
#include <vector>

class Range {
public:
	int startDate;
	int endDate;
	int startIdx;
	int endIdx;

	bool operator == (Range range) {
		if (startDate == range.startDate && endDate == range.endDate && startIdx == range.startIdx && endIdx == range.endIdx) {
			return true;
		}
		return false;
	}
};

#endif