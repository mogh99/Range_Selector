#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Data.cpp"

Data parseCSVFile(std::string filename) {
	//std::vector<std::pair<std::string, std::vector<float>>> data;
	Data data;
	std::ifstream file(filename);

	if (!file.is_open()) {
		throw std::runtime_error("ERROR: Failed To Open The File" + filename);
	}

	std::string line, colname;
	int timestampIdx;
	const std::string TIMESTAMP = "Timestamp";

	// Read the column names
	if (file.good()) {
		std::getline(file, line);

		std::stringstream ss(line);

		while (std::getline(ss, colname, ',')) {
			data.sensors.push_back({ colname, std::vector<float> {} });
		}
	}
}

void main() {
	std::string filename = "ExampleSet.csv";

	parseCSVFile(filename);
}