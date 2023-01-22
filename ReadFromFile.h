#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;



class ReadFromFile {
public:

	static void readStringFromFile(string str, ifstream& file) {
		file.read((char*)&str, sizeof(string));
	}

	static void readIntFromFile(int val, ifstream& file) {
		file.read((char*)&val, sizeof(int));
	}

	static void readFieldsFromFile(Field* fields, int numberOfFields, ifstream& file) {
		for (int i = 0; i < numberOfFields; i++) {
			readStringFromFile(fields[i].getName(), file);
			readStringFromFile(fields[i].getType(), file);
			readStringFromFile(fields[i].getDefaultValue(), file);
		}
	}

	static const void readRecordsFromFile(Record* records, int numberOfRecords, ifstream& file) {
		for (int i = 0; i < numberOfRecords; i++) {
			for (int j = 0; j < records[i].getNoValues(); i++) {
				readStringFromFile(records[i][j], file);
			}
		}
	}

};
