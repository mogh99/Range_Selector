#include <fstream>
#include <sstream>
#include <iomanip>

#include "Data.cpp"
#include "Range.cpp"

const int DOUBLE_PRECISION = 9;

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
			// Divide the timestamp date by 1000 to be compatible with ImPlot
			if (data.columns.at(colIdx).isTimeStamp) {
				data.columns.at(colIdx).values.push_back(val / 1000);
			}
			else {
				data.columns.at(colIdx).values.push_back(val);
			}

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
		for (int j = 0; j < data.numberOfColumns; ++j) {
			// Multiply the timestamp date by 1000 to be compatible with RapidMiner
			if (data.columns.at(j).isTimeStamp) {
				file << std::setprecision(DOUBLE_PRECISION) << data.columns.at(j).values.at(i) * 1000;
			}
			else {
				file << std::setprecision(DOUBLE_PRECISION) << data.columns.at(j).values.at(i);
			}

			if (j != data.numberOfColumns - 1) file << ","; // No comma at end of line
		}
		file << "\n";
	}

	file.close();
}

void deleteUnwantedRange(Data* normalData, Range* selectedRange) {
	// Update the numberOfRows by calculating the range [(endIdx - startIdx) + 1]
	// One is added because we remove also the endIdx
	normalData->numberOfRows -= (selectedRange->endIdx + 1 - selectedRange->startIdx);

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