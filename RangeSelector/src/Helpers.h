#pragma once

// CSV File Parsing
Data parseCSVFile(std::string fileName);
void writeCSVFile(std::string filePath, Data data);

// Managing ranges
void deleteUnwantedRange(Data* normalData, Range* selectedRange);
void undoSelectedRange(Data* data, Data* normalData, std::vector<Range>* ranges, Range* undoRange);