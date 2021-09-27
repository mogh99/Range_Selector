#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

#include "Data.cpp"
#include "Range.cpp"

const int RAPIDMINER_DOUBLE_PRECISION = 9;

Data parseCSVFile(std::string fileName) {

	Data data;
	data.fileName = fileName;
	std::ifstream file(fileName);

	// Check if the file was opened correctly
	if (!file.is_open()) {
		throw std::runtime_error("ERROR: Failed To Open The File" + fileName);
	}

	// Helper variables to parse the date.
	std::string line, colname;
	double val;
	const std::string TIMESTAMP = "Timestamp";

	// Read the column names
	if (file.good()) {
		std::getline(file, line);
		std::stringstream ss(line);

		int idx = 0;
		while (std::getline(ss, colname, ',')) {
			data.columns.push_back({ TIMESTAMP.compare(colname) ? false : true, idx, colname, std::vector<double> {} });
			idx += 1;
		}
		data.numberOfColumns = idx;
	}

	// Read the data row by row
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		int colIdx = 0;

		while (ss >> val) {
			data.columns.at(colIdx).values.push_back(val);

			if (ss.peek() == ',') ss.ignore();

			colIdx++;
		}
	}

	data.numberOfRows = data.columns.at(0).values.size();

	// Close file
	file.close();

	return data;
}

void writeCSVFile(std::string filePath, Data data) {
	std::ofstream file(filePath);

	// Write the sensor names and the timestamp.
	for (int i = 0; i < data.numberOfColumns; i++) {
		file << data.columns.at(i).name;
		if (i != data.numberOfColumns - 1) file << ',';
	}
	file << "\n";


	for (int i = 0; i < data.numberOfRows; ++i) {
		for (int j = 0; j < data.numberOfColumns; ++j){
			file << std::setprecision(RAPIDMINER_DOUBLE_PRECISION) << data.columns.at(j).values.at(i);
			if (j != data.numberOfColumns - 1) file << ","; // No comma at end of line
		}
		file << "\n";
	}

	file.close();
}

void deleteUnwantedRange(Data* normalData, Range* range) {
	normalData->numberOfRows -= (range->endIdx+1 - range->startIdx);

	for (int i = 0; i < normalData->numberOfColumns; i++) {
		normalData->columns.at(i).values.erase(normalData->columns.at(i).values.begin() + range->startIdx, normalData->columns.at(i).values.begin() + range->endIdx + 1);
	}

}

void undoSelectedRange() {

}

void main() {
	std::cout.precision(RAPIDMINER_DOUBLE_PRECISION);

	std::string fileName = "ExampleSet.csv";

	Data data = parseCSVFile(fileName);
	Data normalData = data;
	std::vector<Range> ranges;
	ranges.push_back({ 0.0 , 0.0 , 5, 6 });

	deleteUnwantedRange(&normalData, &ranges.at(0));

	std::string newFileName = "ExampleSetNew.csv";
	writeCSVFile(newFileName, normalData);
}