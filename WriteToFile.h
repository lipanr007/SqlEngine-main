#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using std::string;
using std::ofstream;
using std::istream;
using std::ifstream;
using std::ios;


class WriteToFile {
public:

	static void writeRecordsToBinaryFile(Record* records, int numberOfRecords, ofstream& file) {
		for (int i = 0; i < numberOfRecords; i++) {
			string* tempValues = new string[records[i].getNoValues()];
			for (int j = 0; j < records[i].getNoValues(); j++) {
				file.write(tempValues[j].c_str(), sizeof(string));
			}
			delete[] tempValues;
			file.write((char*)records[i].getNoValues(), sizeof(int));
		}
	}

	static void writeFieldsToBinaryFile(Field* fields, int numberOfFields, ofstream& file) {
		for (int i = 0; i < numberOfFields; i++) {
			file.write(fields[i].getName().c_str(), sizeof(string));
			file.write(fields[i].getType().c_str(), sizeof(string));
			file.write(fields[i].getDefaultValue().c_str(), sizeof(string));
		}
	}

	static void writeTablesToBinaryFile(Table* tables, int numberOfTables, ifstream& binaryTables) {
		ofstream tableStructure("tables.bin", ios::out | ios::binary);
		for (int i = 0; i < numberOfTables; i++) {
			tableStructure.write((char*)&tables[i], sizeof(tables[i]));
		}
		tableStructure.close();
		remove("binaryTables.bin");
		rename("tables.bin", "binaryTables.bin");
	}

	static void writeRecordsToBinaryFile(Record* records, int numberOfRecords, ifstream& binaryRecords) {
		ofstream recordStructure("records.bin", ios::out | ios::binary);
		for (int i = 0; i < numberOfRecords; i++) {
			recordStructure.write((char*)&records[i], sizeof(records[i]));
		}
		recordStructure.close();
		remove("binaryRecords.bin");
		rename("records.bin", "binaryRecords.bin");
	}

	static void allocSpaceTable(Table*& tables, int& numberOfTables, int spaces) {
		if (tables != nullptr) {
			Table* tempTables = new Table[numberOfTables + spaces];
			for (int i = 0; i < numberOfTables; i++) {
				tempTables[i] = tables[i];
			}
			delete[] tables;

			numberOfTables += spaces;
			tables = new Table[numberOfTables];
			for (int i = 0; i < numberOfTables; i++) {
				tables[i] = tempTables[i];
			}
			delete tempTables;
		}
		else {
			tables = new Table[spaces];
			numberOfTables += spaces;
		}
	}

	static void allocSpaceField(Field*& fields, int& numberOfFields, int spaces) {
		if (fields != nullptr) {
			Field* tempFields = new Field[numberOfFields + spaces];
			for (int i = 0; i < numberOfFields; i++) {
				tempFields[i] = fields[i];
			}
			delete[] fields;

			numberOfFields += spaces;
			fields = new Field[numberOfFields];
			for (int i = 0; i < numberOfFields; i++) {
				fields[i] = tempFields[i];
			}
			delete[] tempFields;
		}
		else {
			fields = new Field[spaces];
			numberOfFields += spaces;
		}
	}

	static void allocSpaceRecord(Record*& records, int& numberOfRecords, int spaces) {
		if (records != nullptr) {
			Record* tempRecords = new Record[numberOfRecords + spaces];
			for (int i = 0; i < numberOfRecords; i++) {
				tempRecords[i] = records[i];
			}
			delete[] records;

			numberOfRecords += spaces;
			records = new Record[numberOfRecords];
			for (int i = 0; i < numberOfRecords; i++) {
				records[i] = tempRecords[i];
			}
			delete[] tempRecords;
		}
		else {
			records = new Record[spaces];
			numberOfRecords += spaces;
		}
	}
};
