#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "Constants.h"
#include "Exception.h"
#include "CheckCommands.h"
#include "Field.h"
#include "Table.h"

class CreateTable: public Table {
public:

	static void displayTable(string tableName, Table* tables, int numberOfTables) {
		bool found = false;
		for (int i = 0; i < numberOfTables; i++) {
			if (tables[i].getName() == tableName) {
				tables[i].displayTable();
				found = true;
				break;
			}
		}

		if (found == false) {
			throw SQLException("Table not found.");
		}
	}

	static Field* getTableFields(string line, int& numberOfFields) {
		if (line.at(0) == ' ') {
			Field* myFields = nullptr;
			int i = 0;
			CheckCommands::trimmer(line);
			if (line.at(i++) == '(') {
				numberOfFields = 1;
				string fieldName = "";
				string fieldType = "";
				string defaultValue = "";
				if (line.at(i++) == '(') {

					while (line.at(i) != ')') {
						fieldName = "";
						fieldType = "";
						defaultValue = "";
						CheckCommands::parenthesesCheck(line, i, 0, fieldName, fieldType, defaultValue);
						if ((line.at(i) != ',' && i < line.length() - 1) || (line.at(i) != ')' && i == line.length() - 1)) {
							throw SQLException("Invalid syntax.");
						}
						else {
							Field tempField(fieldName, fieldType, defaultValue);
							if (myFields != nullptr) {
								Field* tempFields = new Field[numberOfFields + 1];
								for (int j = 0; j < numberOfFields; j++) {
									tempFields[j] = myFields[j];
								}
								delete[] myFields;
								tempFields[numberOfFields] = tempField;
								numberOfFields++;
								myFields = new Field[numberOfFields];

								for (int j = 0; j < numberOfFields; j++) {
									myFields[j] = tempFields[j];
								}
							}
							else {
								myFields = new Field[1];
								myFields[0] = tempField;
							}

							if (line.at(i) == ',') {
								i += 2;
							}
						}
					}
					if (i != line.length() - 1) {
						throw SQLException("Invalid syntax, there are other charaters after the last ')'.");
					}
				}
				else {
					i--;
					myFields = new Field[numberOfFields];
					CheckCommands::parenthesesCheck(line, i, 0, fieldName, fieldType, defaultValue);
					if (line.at(i - 1) == ')' && i - 1 == line.length() - 1) {
						Field tempField(fieldName, fieldType, defaultValue);
						myFields[0] = tempField;
					}
					else {
						throw SQLException("Invalid syntax. Too many parameters.");
					}
				}
			}
			else {
				throw SQLException("Invalid syntax, missing parentheses");
			}
			return myFields;
		}
		else {
			throw SQLException("Invalid syntax, missing space between table name and columns.");
		}
	}

	static void trimmer(string& line) {
		for (int i = 0; i < line.length(); i++) {
			if (line.at(i) == ' ') {
				line = line.substr(0, i) + line.substr(i + 1);
			}
		}
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

	static void removeTable(Table*& tables, string tableName, int& numberOfTables) {
		bool exists = false;
		if (tables != nullptr) {
			Table* tempTables = new Table[numberOfTables];
			int k = 0;
			for (int i = 0; i < numberOfTables; i++) {
				if (tables[i].getName() != tableName) {
					tempTables[k++] = tables[i];
				}
				else {
					exists = true;
				}
			}

			if (exists == true) {
				numberOfTables = k;
				delete[] tables;
				tables = new Table[numberOfTables];

				for (int i = 0; i < numberOfTables; i++) {
					tables[i] = tempTables[i];
				}
				delete[] tempTables;
			}
			else {
				delete[] tempTables;
				throw SQLException("Table not found in database.");
			}
		}
	}

	static void dropTable(Table*& tables, int& numberOfTables, string tableName) {
		bool found = false;
		if (tables != nullptr) {
			for (int i = 0; i < numberOfTables; i++) {
				if (tables[i].getName() == tableName) {
					found = true;
					removeTable(tables, tableName, numberOfTables);
				}
			}
		}
		else {
			throw SQLException("Empty database.");
		}

		if (found == false) {
			throw SQLException("Table not found.");
		}
	}

	static Table createTable(Table* tables, int numberOfTables, string tableName, string line) {
		if (tables != nullptr) {
			for (int i = 0; i < numberOfTables; i++) {
				if (tables[i].getName() == tableName) {
					throw SQLException("This table already exists in the database.");
				}
			}
		}

		int numberOfFields = 0;
		Field* myFields = getTableFields(line, numberOfFields);
		Table table(tableName, numberOfFields, myFields);

		return table;
	}


	static void addTable(Table*& tables, Table currentTable, int& numberOfTables) {
		if (tables != nullptr) {
			Table* tempTables = new Table[numberOfTables + 1];
			for (int i = 0; i < numberOfTables; i++) {
				tempTables[i] = tables[i];
			}
			delete[] tables;
			tempTables[numberOfTables] = currentTable;

			numberOfTables++;
			tables = new Table[numberOfTables];

			for (int i = 0; i < numberOfTables; i++) {
				tables[i] = tempTables[i];
			}
			delete[] tempTables;
		}
		else {
			numberOfTables++;
			tables = new Table[numberOfTables];
			tables[0] = currentTable;
		}
	}

	static void processAfterWhereClause(string& line, string& fieldName, string& fieldValue) {
		int i = 0;
		while (line.at(i) != '=') {
			fieldName += line.at(i++);
		}
		i++;
		while (i < line.length() && line.at(i) != ' ') {
			fieldValue += line.at(i++);
		}
		if (i < line.length()) {
			throw SQLException("Invalid syntax for 'WHERE' clause.");
		}
	}

	static Record getRecordFromParenthese(string line, Table& table) {
		trimmer(line);
		if (line.at(0) == '(') {
			Record myRecords;
			string* values = new string[table.getNoOfFields()];
			int numberOfValues = 0;
			int i = 1;

			string tempValue = "";
			while (line.at(i) != ')' && i <= line.length()) {
				if (line.at(i) != ',') {
					tempValue += line.at(i);
				}
				else {
					values[numberOfValues++] = tempValue;
					if (numberOfValues == table.getNoOfFields()) {
						delete[] values;
						throw SQLException("Too many parameters. ");
					}
					tempValue = "";
				}
				i++;
			}

			if (numberOfValues != table.getNoOfFields() - 1) {
				throw SQLException("Invalid number of fields.");
			}

			values[numberOfValues] = tempValue;
			myRecords.setValues(values, numberOfValues + 1);
			delete[] values;
			return myRecords;
		}
		else {
			throw SQLException("Invalid syntax, missing '(' from command.");
		}
	}


	static void insertIntoTables(string tableName, Table* tables, int numberOfTables, string line) {
		bool found = false;
		for (int i = 0; i < numberOfTables; i++) {
			if (tables[i].getName() == tableName) {
				found = true;

				tables[i].addRecord(getRecordFromParenthese(line, tables[i]));
			}
		}

		if (found == false) {
			throw SQLException("Table not found in database.");
		}
	}

	Record& operator[](int i)
	{
		if (i < 0 || i > this->noOfRecords)
		{
			throw SQLException("Index out of bounds");
		}
		else
		{
			return this->records[i];
		}
	}

	static Record* getRecordWithValueForField(Table table, string fieldName, string fieldValue, int& numberOfRecords) {
		bool fieldFound = false;
		bool valueFound = false;
		Record* records = nullptr;


		for (int i = 0; i < table.getNoOfFields(); i++)
		{
			Field field = table.getFieldAtASpecifiedPosition(i);
			if (field.getName() == fieldName)
			{
				fieldFound = true;
				for (int j = 0; j < table.getNoOfRecords(); j++)
				{
					if (table[j][i] == fieldValue)
					{
						valueFound = true;
						allocSpaceRecord(records, numberOfRecords, 1);
						records[numberOfRecords - 1] = table[j];
					}
				}
				if (valueFound == false) {
					throw SQLException("Record not found.");
				}
				break;
			}
		}

		if (fieldFound == false) {
			throw SQLException("Invalid field.");
		}

		return records;
	}

	static void processBeforeFrom(string*& fieldsInput, string fieldsName, int& numberOfFields) {
		int i = 0;
		while (i < fieldsName.length()) {
			string tempString = "";
			while (i < fieldsName.length() && fieldsName.at(i) != ',') {
				tempString += fieldsName.at(i++);
			}

			if (i < fieldsName.length()) {
				if (fieldsName.at(i) != ',') {
					throw SQLException("Invalid input syntax.");
				}
			}
			numberOfFields++;
			i++;
			fieldsInput[numberOfFields - 1] = tempString;
		}
	}


	static void printIndividualRecord(Record record, int position) {
		string* values = record.getValues();
		cout << values[position] << " ";
	}

	static void selectTable(string tableName, Table*& tables, int numberOfTables, string line, string fieldsName) {
		bool found = false;

		for (int i = 0; i < numberOfTables && found == false; i++) {
			if (tables[i].getName() == tableName) {
				found = true;
				if (fieldsName != "ALL") {
					if (line.length() == 0) {
						string* fieldsInput = new string[tables[i].getNoOfFields()];
						int numberOfFields = 0;
						processBeforeFrom(fieldsInput, fieldsName, numberOfFields);
						if (numberOfFields > tables[i].getNoOfFields()) {
							throw SQLException("Command has too many fields.");
						}

						Field* fields = tables[i].getFields();

						int k = 0, l = 0;
						while (k < numberOfFields && l < tables[i].getNoOfFields()) {
							if (fieldsInput[k] == fields[l].getName()) {
								cout << "The values for field: '" << fieldsInput[k] << "' are: " << endl;
								for (int m = 0; m < tables[i].getNoOfRecords(); m++) {
									printIndividualRecord(tables[i][m], l);
								}
								k++;
								l = -1;
								cout << endl;
							}
							l++;
						}
						if (k == 0) {
							throw SQLException("Field not found.");
						}
						delete[] fieldsInput;
					}
					else {
						string fieldName = "";
						string fieldValue = "";
						string* fieldsInput = new string[tables[i].getNoOfFields()];
						Field* fields = tables[i].getFields();
						int numberOfFields = 0;
						processBeforeFrom(fieldsInput, fieldsName, numberOfFields);
						processAfterWhereClause(line, fieldName, fieldValue);
						int numberOfRecords = 0;
						Record* record = getRecordWithValueForField(tables[i], fieldName, fieldValue, numberOfRecords);
						int k = 0, l = 0;

						while (k < numberOfFields && l < tables[i].getNoOfFields()) {
							if (fieldsInput[k] == fields[l].getName()) {
								cout << "The values for field: '" << fieldsInput[k] << "' are: " << endl;
								for (int m = 0; m < numberOfRecords; m++) {
									printIndividualRecord(record[m], l);
								}
								k++;
								l = -1;
								cout << endl;
							}
							l++;
						}
						if (k == 0) {
							throw SQLException("Field not found.");
						}

						delete[] record;
						delete[] fieldsInput;
					}
				}
				else {
					if (line.length() > 0)
					{
						string fieldName = "";
						string fieldValue = "";
						processAfterWhereClause(line, fieldName, fieldValue);
						int numberOfRecords = 0;
						Record* record = getRecordWithValueForField(tables[i], fieldName, fieldValue, numberOfRecords);

						for (int k = 0; k < numberOfRecords; k++) {
							cout << record[k];
						}

						delete[] record;
					}
				}
			}
		}
	}

	static void updateTable(Table* tables, int numberOfTables, string tableName, string fieldName, string fieldValue, string newFieldName, string newFieldValue) {
		bool found = false;
		for (int i = 0; i < numberOfTables && found == false; i++) {
			if (tables[i].getName() == tableName) {
				found = true;
				int posToUpdate = -1;
				int posToBeUpdated = -1;
				Field* fields = tables[i].getFields();
				for (int j = 0; j < tables[i].getNoOfFields(); j++) {
					if (fields[j].getName() == fieldName) {
						posToUpdate = j;
					}
					if (fields[j].getName() == newFieldName) {
						posToBeUpdated = j;
					}
				}

				if (posToUpdate == -1 && posToBeUpdated == -1) {
					throw SQLException("Fields not found.");
				}

				for (int j = 0; j < tables[i].getNoOfRecords(); j++) {
					if (tables[i][j][posToUpdate] == fieldValue && fields[posToUpdate].getName() == fieldName) {
						tables[i][j][posToBeUpdated] = newFieldValue;
					}
				}
			}
		}
	}

	static void deleteFromTable(string line, Table* tables, string tableName, int numberOfTables) {
		string fieldName = "";
		string fieldValue = "";
		processAfterWhereClause(line, fieldName, fieldValue);
		bool found = false;
		for (int i = 0; i < numberOfTables && found == false; i++) {
			if (tables[i].getName() == tableName) {
				found = true;
				int numberOfRecords = 0;
				Record* record = getRecordWithValueForField(tables[i], fieldName, fieldValue, numberOfRecords);

				for (int j = 0; j < numberOfRecords; j++) {
					tables[i].deleteRecord(record[j]);
				}
			}
		}

	}

	static int* processFieldName(string fieldName, Table table) {
		int* positionsForFields = new int[table.getNoOfFields()];
		int k = 0;
		trimmer(fieldName);
		string* fieldsName = new string[table.getNoOfFields()];
		int numberOfFields = 0;
		int i = 1;

		string tempValue = "";
		while (i < fieldName.length()) {
			if (fieldName.at(i) != ',') {
				tempValue += fieldName.at(i);
			}
			else {
				bool found = false;
				for (int i = 0; i < table.getNoOfFields(); i++)
				{
					Field field = table.getFieldAtASpecifiedPosition(i);
					if (field.getName() == tempValue)
					{
						positionsForFields[k++] = i;
						found = true;
						break;
					}
				}

				if (found == false) {
					delete[] fieldsName;
					throw SQLException("Field not found in table.");
				}

				fieldsName[numberOfFields++] = tempValue;
				if (numberOfFields == table.getNoOfFields()) {
					delete[] fieldsName;
					throw SQLException("Too many parameters.");
				}
				tempValue = "";
			}
			i++;
		}
		return positionsForFields;
	}




	static void setRecordWithValueForField(Table table, string fieldName, string fieldValue, int& numberOfRecords) {
		bool fieldFound = false;
		bool valueFound = false;
		Record* records = nullptr;


		for (int i = 0; i < table.getNoOfFields(); i++)
		{
			Field field = table.getFieldAtASpecifiedPosition(i);
			if (field.getName() == fieldName)
			{
				fieldFound = true;
				for (int j = 0; j < table.getNoOfRecords(); j++)
				{					
					if (table[i][j] == fieldValue)
					{
						valueFound = true;
						allocSpaceRecord(records, numberOfRecords, 1);
						table[j][i] = fieldValue;
					}
				}
				if (valueFound == false) {
					throw SQLException("Record not found.");
				}
				break;
			}
		}

		if (fieldFound == false) {
			throw SQLException("Invalid field.");
		}
	}

	
};

