#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "Exception.h"
#include "Record.h"
#include "Field.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

class Table: public Field, public Record {
protected:
	string name = "";
	Record* records = nullptr;
	int noOfRecords = 0;
	Field* fields = nullptr;
	int noOfFields = 0;

public:
	Table() {}

	Table(string name, int noOfRecords, int noOfFields, Record* records, Field* fields)
	{
		this->name = name;
		this->noOfRecords = noOfRecords;
		this->noOfFields = noOfFields;

		this->records = new Record[noOfRecords];
		for (int i = 0; i < this->noOfRecords; i++)
			this->records[i] = records[i];

		this->fields = new Field[noOfFields];
		for (int i = 0; i < this->noOfFields; i++)
			this->fields[i] = fields[i];
	}

	Table(string name, int noOfFields, Field* fields)
	{
		this->name = name;
		this->noOfRecords = noOfRecords;
		this->noOfFields = noOfFields;

		this->fields = new Field[noOfFields];
		for (int i = 0; i < this->noOfFields; i++)
			this->fields[i] = fields[i];
	}

	Table(const Table& table) {

		this->name = table.name;
		this->noOfRecords = table.noOfRecords;
		this->noOfFields = table.noOfFields;

		if (table.records != nullptr) {
			this->records = new Record[table.noOfRecords];
			for (int i = 0; i < table.noOfRecords; i++)
				this->records[i] = table.records[i];
		}

		if (table.fields != nullptr) {
			this->fields = new Field[table.noOfFields];
			for (int i = 0; i < table.noOfFields; i++)
				this->fields[i] = table.fields[i];
		}
	}
	~Table()
	{
		if (this->records != nullptr)
			delete[] this->records;
		if (this->fields != nullptr)
			delete[] this->fields;
	}

	Table& operator=(const Table& table)
	{
		this->name = table.name;
		this->noOfRecords = table.noOfRecords;
		this->noOfFields = table.noOfFields;

		if (this->records != nullptr)
			delete[] this->records;
		if (this->fields != nullptr)
			delete[] this->fields;

		this->records = new Record[table.noOfRecords];
		for (int i = 0; i < table.noOfRecords; i++)
			this->records[i] = table.records[i];

		this->fields = new Field[table.noOfFields];
		for (int i = 0; i < table.noOfFields; i++)
			this->fields[i] = table.fields[i];
		return *this;
	}


	void displayTable()
	{
		cout << endl << endl;
		cout << "Table name: " << this->name << endl;
		cout << "Number of fields: " << this->noOfFields << endl;
		cout << "Fields:" << endl;
		for (int i = 0; i < this->noOfFields; i++)
		{
			cout << this->fields[i] << endl;
		}
		cout << "Number of records: " << this->noOfRecords << endl;
		cout << "Record:" << endl;
		for (int j = 0; j < this->noOfRecords; j++)
		{
			cout << this->records[j] << endl;
		}
	}

	string getName()
	{
		return this->name;
	}

	void setName(string name)
	{
		if (name.length() >= 0)
			this->name = name;
	}

	int getNoOfFields()
	{
		return this->noOfFields;
	}

	void setNoOfFields(int noOfFields)
	{
		if (noOfFields >= 0)
			this->noOfFields = noOfFields;
	}

	int getNoOfRecords()
	{
		return this->noOfRecords;
	}

	void setNoOfRecords(int noOfRecords)
	{
		if (noOfRecords >= 0)
			this->noOfRecords = noOfRecords;
	}

	void addRecord(Record records)
	{
		Record* aux = new Record[this->noOfRecords];
		for (int i = 0; i < this->noOfRecords; i++)
			aux[i] = this->records[i];

		if (this->records != nullptr)
			delete[] this->records;

		int newNoOfRecords = this->noOfRecords + 1;
		this->records = new Record[newNoOfRecords];
		for (int i = 0; i < this->noOfRecords; i++)
			this->records[i] = aux[i];

		this->records[newNoOfRecords - 1] = records;

		if (aux != nullptr)
			delete[] aux;

		this->noOfRecords = newNoOfRecords;
	}

	void setFields(Field* fields, int numberOfFields) {
		if (this->fields != nullptr) {
			delete[] this->fields;
		}
		this->noOfFields = numberOfFields;
		this->fields = new Field[numberOfFields];
		for (int i = 0; i < this->noOfFields; i++) {
			this->fields[i] = fields[i];
		}
	}

	void setRecords(Record* records, int noRecords) {
		if (this->records != nullptr) {
			delete[] this->records;
		}
		this->noOfRecords = noRecords;
		this->records = new Record[noRecords];
		for (int i = 0; i < this->noOfRecords; i++) {
			this->records[i] = records[i];
		}
	}

	void deleteRecord(Record record)
	{
		Record* aux = new Record[this->noOfRecords - 1];
		bool found = false;
		for (int i = 0; i < this->noOfRecords; i++)
		{

			if (i == this->noOfRecords && found == false && this->records[i] != record)
				break;

			if (this->records[i] != record)
			{
				aux[i] = this->records[i];
			}
			else {
				found = true;
			}
		}
		if (found == false && aux != nullptr)
		{
			delete[] aux;

			cout << "Record not found.";
		}
		else
		{
			if (this->records != nullptr)
				delete[] this->records;

			this->records = new Record[this->noOfRecords - 1];
			for (int i = 0; i < this->noOfRecords - 1; i++)
				this->records[i] = aux[i];

			if (aux != nullptr)
				delete[] aux;

			this->noOfRecords--;

			cout << endl << "Record Deleted." << endl;
		}
	}

	Record& operator[](int i)
	{
		if (i < 0 || i > this->noOfRecords)
		{
			throw SQLException("Index out of bounds.");
		}
		else
		{
			return this->records[i];
		}
	}

	Field getFieldAtASpecifiedPosition(int position)
	{
		if (position < 0 || position > this->noOfFields)
		{
			throw SQLException("Position out of bounds.");
		}
		else
		{
			return this->fields[position];
		}
	}

	Field* getFields() {
		return this->fields;
	}

	Record* getRecords() {
		return this->records;
	}

	bool operator==(Table table)
	{
		if (this->name == table.name && this->noOfFields == table.noOfFields && this->noOfRecords == table.noOfRecords)
		{
			bool field = true;
			for (int i = 0; i < this->noOfFields; i++)
			{
				if (this->fields[i] != table.fields[i])
				{
					field = false;
				}
			}

			bool record = true;
			for (int i = 0; i < this->noOfRecords; i++)
			{
				if (this->records[i] != table.records[i])
				{
					record = false;
				}
			}
			if (record && field)
				return true;
			else return false;
		}
		else
			return false;
	}

	bool operator!()
	{
		if (this->fields != nullptr) {
			return false;
		}
		else
			return true;
	}

	friend ifstream& operator>>(ifstream& in, Table& table);
	friend ofstream& operator<<(ofstream& out, Table& table);
	friend istream& operator>>(istream& in, Table& table);
	friend ostream& operator<<(ostream& out, Table& table);
};

ifstream& operator>>(ifstream& in, Table& table)
{
	cout << "Table name: " << endl;
	in >> table.name;
	cout << "Number of Fields: " << endl;
	in >> table.noOfFields;
	cout << "Number of Record: " << endl;
	in >> table.noOfRecords;

	for (int i = 0; i < table.noOfRecords; i++) {
		cout << "Record number: " << i + 1 << " is:" << endl;
		in >> table.records[i];
	}
	for (int i = 0; i < table.noOfFields; i++) {
		cout << "Record number: " << i + 1 << " is: " << endl;
		in >> table.fields[i];
	}
	return in;
}

ofstream& operator<<(ofstream& out, Table& table)
{

	out << "Table name: " << table.name << endl;
	out << "Number of fields: " << table.noOfFields << endl;
	out << "Fields:" << endl;
	for (int i = 0; i < table.noOfFields; i++)
	{
		out << table.fields[i] << endl;
	}
	out << "Number of records: " << table.noOfRecords << endl;
	out << "Record:" << endl;
	for (int j = 0; j < table.noOfRecords; j++)
	{
		out << table.records[j] << endl;
	}

	return out;

}

ostream& operator<<(ostream& out, Table& table)
{
	out << "Table name: " << table.name << endl;
	out << "Number of fields: " << table.noOfFields << endl;
	out << "Fields:" << endl;
	for (int i = 0; i < table.noOfFields; i++)
	{
		out << table.fields[i] << endl;
	}
	out << "Number of records: " << table.noOfRecords << endl;
	out << "Record:" << endl;
	for (int j = 0; j < table.noOfRecords; j++)
	{
		out << table.records[j] << endl;
	}

	return out;
}

istream& operator>>(istream& in, Table& table)
{
	cout << "Table name: " << endl;
	in >> table.name;
	cout << "Number of Fields: " << endl;
	in >> table.noOfFields;
	cout << "Number of Record: " << endl;
	in >> table.noOfRecords;

	for (int i = 0; i < table.noOfRecords; i++) {
		cout << "Record number: " << i + 1 << " is: " << endl;
		in >> table.records[i];
	}
	for (int i = 0; i < table.noOfFields; i++) {
		cout << "Record number: " << i + 1 << " is: " << endl;
		in >> table.fields[i];
	}
	return in;
}

