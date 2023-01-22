#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using std::cout;
using std::endl;
using std::ostream;
using std::istream;

class Record {
protected:
	string* records = nullptr;
	int noRecords = 0;

public:
	Record() {

	}

	Record(string* newValues, int newNoValues) {
		this->noRecords = newNoValues;
		this->records = new string[newNoValues];
		for (int i = 0; i < newNoValues; i++) {
			this->records[i] = newValues[i];
		}
	}

	Record(const Record& record) {
		this->noRecords = record.noRecords;
		this->records = new string[record.noRecords];
		for (int i = 0; i < record.noRecords; i++) {
			this->records[i] = record.records[i];
		}
	}

	~Record() {
		if (this->records != nullptr) {
			delete[] this->records;
		}
	}



	Record& operator=(const Record& record) {
		this->noRecords = record.noRecords;
		if (this->records != nullptr) {
			delete[] this->records;
		}

		this->records = new string[record.noRecords];
		for (int i = 0; i < record.noRecords; i++) {
			this->records[i] = record.records[i];
		}
		return *this;
	}

	string* getValues() {
		return this->records;
	}

	void setValues(string* newValues, int newNoValues) {
		if (this->records != nullptr) {
			delete[] this->records;
		}
		this->noRecords = newNoValues;
		this->records = new string[newNoValues];
		for (int i = 0; i < newNoValues; i++) {
			this->records[i] = newValues[i];
		}
	}

	int getNoValues() {
		return this->noRecords;
	}

	void setNoValues(int value) {
		if (value >= 0) {
			this->noRecords = value;
		}
	}

	bool operator!=(Record record) {
		if (this->noRecords == record.noRecords) {
			bool found = false;
			for (int i = 0; i < this->noRecords; i++)
			{
				if (this->records[i] != record.records[i])
				{
					found = true;
				}
			}
			if (found == true)
				return true;
			else
				return false;
		}
		else
			return true;
	}

	string& operator[](int i)
	{
		if (i < 0 || i > this->noRecords)
		{
			throw SQLException("Index out of bounds");
		}
		else
		{
			return this->records[i];
		}
	}

	friend ostream& operator<<(ostream& console, Record& record);
	friend istream& operator>>(istream& input, Record& record);

};

ostream& operator<<(ostream& console, Record& record) {
	console << "Number of Field Records: " << record.noRecords << endl;
	console << "Field Values: ";
	for (int i = 0; i < record.noRecords; i++) {
		console << endl << "The field number: " << i << " is " << record.records[i];
	}
	cout << endl;
	cout << endl;

	return console;
}

istream& operator>>(istream& input, Record& record)
{
	cout << "Number Of Values: " << endl;
	input >> record.noRecords;
	cout << "Values: " << endl;
	for (int i = 0; i < record.noRecords; i++)
		input >> record.records[i];
	return input;
}