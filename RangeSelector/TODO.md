## Work Flow:
1. Parse the CSV file.
~~1.1. Read CSV file.~~
~~1.2. Write CSV file.~~
~~2. Convert Timestamp to unix time~~
~~2.1. The timestamp values are already in unix time format.~~
3. Develop a method for selecting the ranges.
4. Make sure everything is working using simple console
4.1. Loading and parsing the csv file.
4.2. Determining the different columns in the csv file as sensor and timestamp
4.3. Convert the timestamp from String to Date.
4.4. Removing selected ranges.
4.5. Storing the new generated file.

### Info:
* RapidMiner date format MM/DD/YY HH:MM AM/PM
* RapidMiner give the value precision to the 9th decimal


## PSUDO CODE
DELETE UNWANTED RANGE:
1. Select unwanted range.
2. Determine the start and end of the unwanted range.
3. for point in DATA:
	if point not inside unwanted range:
		NORMAL_RANGE.append(point)
4. Return NORMAL_RANGE.

UNDO SELECTED RANGE:
1. Select the wanted range to undo.
2. The range has the start index and the end index.
3. Creat a loop that loop through the values form the start index to the end index
4. for(START_IDX; START_IDX <= END_IDX; START_IDX++){
	NORMAL_RANGE.insert(START_IDX, DATA.at(START_IDX));
}
5. Return NORMAL_RANGE