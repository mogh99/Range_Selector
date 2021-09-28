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

void deleteUnwantedRange(Data* normalData, Range* selectedRange) {
	// Update the numberOfRows by calculating the range [(endIdx - startIdx) + 1]
	// One is added because we remove also the endIdx
	normalData->numberOfRows -= (selectedRange->endIdx+1 - selectedRange->startIdx);

	// Loop through the data and remove the range using the startIdx and endIdx
	for (int i = 0; i < normalData->numberOfColumns; i++) {
		normalData->columns.at(i).values.erase(normalData->columns.at(i).values.begin() + selectedRange->startIdx, 
											   normalData->columns.at(i).values.begin() + selectedRange->endIdx + 1);
	}
}

void undoSelectedRange(Data* data, Data* normalData, std::vector<Range>* ranges, Range* undoRange) {
	// Remove the undoRange from the std::vector<Range>
	std::erase(*ranges, *undoRange);
	
	// Make the normalData equal the original data and remove the unwanted ranges
	*normalData = *data;

	// Remove the unwanted ranges
	for (int i = 0; i < ranges->size(); i++) {
		deleteUnwantedRange(normalData, &ranges->at(i));
	}
}

void main() {
	std::cout.precision(RAPIDMINER_DOUBLE_PRECISION);

	std::string fileName = "ExampleSet.csv";

	Data data = parseCSVFile(fileName);
	Data normalData = data;
	std::vector<Range> ranges;
	Range range1 = { 1.5673536E12 , 1.5673716E12 , 19, 24 };
	Range range2 = { 1.5674616E12 , 1.5674796E12 , 49, 54 };
	ranges.push_back(range1);
	deleteUnwantedRange(&normalData, &range1);

	ranges.push_back(range2);
	deleteUnwantedRange(&normalData, &range2);


	undoSelectedRange(&data, &normalData, &ranges, &range1);
	undoSelectedRange(&data, &normalData, &ranges, &range2);

	std::string newFileName = "ExampleSetNew.csv";
	writeCSVFile(newFileName, normalData);
}