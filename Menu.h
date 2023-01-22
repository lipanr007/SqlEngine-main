#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "Constants.h"
#include "Exception.h"
#include "Table.h"
#include "CreateTable.h"
#include "WriteToFile.h"
#include "ReadFromFile.h"
#include "Record.h"
#include "Constants.h"
#include "Field.h"

using std::getline;
using std::cin;

class Menu : 
			public CreateTable, 
			public CheckCommands, 
			public WriteToFile, 
			public ReadFromFile,
			public Table,
			public Record,
			public Field
			
{
public:

	static void commandsFromFile(Table*& tables, int& numberOfTables, string line) {

		string tableName = "";
		Constants command = CheckCommands::getCommandType(line);
		switch (command) {
		case Constants::CREATE_TABLE:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				Table myTable = CreateTable::createTable(tables, numberOfTables, tableName, line);
				CreateTable::addTable(tables, myTable, numberOfTables);
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::DROP_TABLE:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				CreateTable::dropTable(tables, numberOfTables, tableName);
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::INSERT_INTO:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				command = CheckCommands::getCommandType(line);
				if (command != Constants::VALUES) {
					throw SQLException("Syntax error. 'VALUES' keyword not found.");
				}
				CheckCommands::processBeforeCommand(line, tableName, command);
				CreateTable::insertIntoTables(tableName, tables, numberOfTables, line);

			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::DISPLAY_TABLE:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				CreateTable::displayTable(tableName, tables, numberOfTables);
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::SELECT:
		{
			try {
				string fieldName = "";
				CheckCommands::processBeforeCommand(line, fieldName, command);
				command = CheckCommands::getCommandType(line);
				if (command != Constants::FROM) {
					throw SQLException("Syntaxt error. 'FROM' keyword not found.");
				}
				CheckCommands::processBeforeCommand(line, tableName, command);
				command = CheckCommands::getCommandType(line);
				if (command != Constants::WHERE && line.length() > 0) {
					throw SQLException("Syntaxt error. 'WHERE' keyword not found. ");
				}
				else {
					CheckCommands::processBeforeCommand(line, tableName, command);
					CreateTable::selectTable(tableName, tables, numberOfTables, line, fieldName);
				}
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::DELETE_FROM:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				command = CheckCommands::getCommandType(line);
				if (command != Constants::WHERE && line.length() > 0) {
					throw SQLException("Syntaxt error. 'WHERE' keyword not found.");
				}
				else {
					if (command == Constants::WHERE && line.length() == 0) {
						throw SQLException("Missing parameters.");
					}
					else {
						if (command == Constants::WHERE && line.length() > 0) {
							CheckCommands::processBeforeCommand(line, tableName, command);
							CreateTable::deleteFromTable(line, tables, tableName, numberOfTables);
						}
					}
				}
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::UPDATE:
		{
			try {
				CheckCommands::processBeforeCommand(line, tableName, command);
				command = CheckCommands::getCommandType(line);
				if (command != Constants::SET) {
					throw SQLException("Invalid syntax. Missing 'SET' clause.");
				}
				else {
					if (line.length() == 0) {
						throw SQLException("Invalid syntax or missing 'SET' clause.");
					}
					else {
						string fieldAndValueToBeUpdated = "";
						CheckCommands::processBeforeCommand(line, fieldAndValueToBeUpdated, command);
						string fieldName = "";
						string fieldValue = "";
						processAfterWhereClause(fieldAndValueToBeUpdated, fieldName, fieldValue);
						command = CheckCommands::getCommandType(line);
						if (command == Constants::WHERE && line.length() > 0) {
							string newFieldAndValue = "";
							string newFieldName = "";
							string newFieldValue = "";
							CheckCommands::processBeforeCommand(line, newFieldAndValue, command);
							processAfterWhereClause(line, newFieldName, newFieldValue);
							CreateTable::updateTable(tables, numberOfTables, tableName, newFieldName, newFieldValue, fieldName, fieldValue);
						}
						else {
							throw SQLException("Invalid syntax or missing 'WHERE.' ");
						}
					}
				}

			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			break;
		}
		case Constants::NONE:
		{
			cout << endl << "Command not found.";
			break;
		}
		}
	}

	static void menuNumber1(Table*& tables, int& numberOfTables, ifstream& binaryTables, ifstream& binaryRecords, int argc, char* argv[]) {
		cout << endl;
		cout << "This is your file output: ";
		if (argc == 0) {
			ifstream file("clienti.csv", ios::in);
			try {
				if (file.is_open()) {
					string line;
					while (getline(file, line)) {
						commandsFromFile(tables, numberOfTables, line);
					}
				}
				else {
					throw SQLException("File not found.");
				}
			}
			catch (SQLException e) {
				cout << endl << e.what() << endl;
			}
			WriteToFile::writeTablesToBinaryFile(tables, numberOfTables, binaryTables);
			for (int k = 0; k < numberOfTables; k++) {
				WriteToFile::writeRecordsToBinaryFile(tables[k].getRecords(), tables[k].getNoOfRecords(), binaryRecords);
			}
			file.close();
		}
		else {
			for (int i = 0; i < argc; i++) {
				ifstream file(argv[i], ios::in);
				try {
					if (file.is_open()) {
						string line;
						while (getline(file, line)) {
							commandsFromFile(tables, numberOfTables, line);
						}
					}
					else {
						throw SQLException("File not found");
					}
				}
				catch (SQLException e) {
					cout << endl << e.what() << endl;
				}
				WriteToFile::writeTablesToBinaryFile(tables, numberOfTables, binaryTables);
				for (int k = 0; k < numberOfTables; k++) {
					WriteToFile::writeRecordsToBinaryFile(tables[k].getRecords(), tables[k].getNoOfRecords(), binaryRecords);
				}
				file.close();
			}
		}
		binaryTables.close();
	}

	static void menuNumber2(Table*& tables, int& numberOfTables) {
			string line;
			getline(cin, line);
			getline(cin, line);
			commandsFromFile(tables, numberOfTables, line);
	}

	static void run(int argc, char* argv[]) {
		ifstream binaryTables("binaryTables.bin", ios::in | ios::binary);
		ifstream binaryRecords("binaryRecords.bin", ios::in | ios::binary);
		Table* tables = nullptr;
		Field* fields = nullptr;
		int numberOfTables = 0;
		int numberOfFields = 0;

		int menuNumber = 0;
		int oldMenuNumber = 0;
		cout << "SQL Project" << endl;
		if (argc == 1) {
			while (true) {
				cout << endl << "Select a command or press any character to exit:" << endl;
				if (oldMenuNumber == 0) {
					cout << "1. Read commands from a file:" << endl;
					cout << "2. Write your own query:" << endl;
				}
				else {
					cout << "1. Write your own query:" << endl;
				}

				cin >> menuNumber;

				switch (menuNumber) {
				case 1:
				{
					if (oldMenuNumber == 1) {
						menuNumber2(tables, numberOfTables);
					}
					else {
						menuNumber1(tables, numberOfTables, binaryTables, binaryRecords, 0, nullptr);
						oldMenuNumber = 1;
					}
					break;
				}
				case 2:
				{
					if (oldMenuNumber == 1 && menuNumber == 2) {
						cout << endl << "Invalid menu number." << endl;
					}
					else {
						menuNumber2(tables, numberOfTables);
					}
					break;
				}
				case 3:
				{

				}
				default:
					return;
				}
			}
		}
		else {
			menuNumber1(tables, numberOfTables, binaryTables, binaryRecords, argc, argv);
		}
	}
};
