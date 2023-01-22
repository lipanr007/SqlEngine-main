#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "Constants.h"
#include "Exception.h"

class CheckCommands {
public:
	static Constants getCommandType(string line) {
		for (size_t i = 0; i < line.size(); ++i) {
			line[i] = toupper(line[i]);
		}
		if (line.substr(0, 13) == "CREATE TABLE ") {
			return Constants::CREATE_TABLE;
		}
		if (line.substr(0, 11) == "DROP TABLE ") {
			return Constants::DROP_TABLE;
		}
		if (line.substr(0, 8) == " VALUES ") {
			return Constants::VALUES;
		}
		if (line.substr(0, 6) == " FROM ") {
			return Constants::FROM;
		}
		if (line.substr(0, 7) == " WHERE ") {
			return Constants::WHERE;
		}
		if (line.substr(0, 5) == " SET ") {
			return Constants::SET;
		}
		if (line.substr(0, 14) == "DISPLAY TABLE ") {
			return Constants::DISPLAY_TABLE;
		}
		if (line.substr(0, 12) == "INSERT INTO ") {
			return Constants::INSERT_INTO;
		}
		if (line.substr(0, 12) == "DELETE FROM ") {
			return Constants::DELETE_FROM;
		}
		if (line.substr(0, 7) == "SELECT ") {
			return Constants::SELECT;
		}
		if (line.substr(0, 7) == "UPDATE ") {
			return Constants::UPDATE;
		}
		return Constants::NONE;
	}
	
	static void parenthesesCheck(string line, int& i, int commasFound, string& fieldName, string& fieldType, string& defaultValue) {
		while (line.at(i - 1) != ')' && i <= line.length()) {
			while (line.at(i) != ',' && line.at(i) != ')') {
				switch (commasFound) {
				case 0:
				{
					fieldName += line.at(i);
					break;
				}
				case 1:
				{
					fieldType += line.at(i);
					break;
				}
				case 2:
				{
					defaultValue += line.at(i);
					break;
				}
				}
				i++;
			}
			if (line.at(i) != ',' && line.at(i) != ')') {
				throw SQLException("The length is bigger than expected");
			}
			else {
				commasFound++;
				i++;
			}
		}
	}

	static void trimmer(string& line) {
		for (int i = 0; i < line.length(); i++) {
			if (line.at(i) == ' ') {
				line = line.substr(0, i) + line.substr(i + 1);
			}
		}
	}

	static string getTableOrFieldName(string line) {
		int i = 0;
		string tableName = "";
		while (i < line.length() && line.at(i) != ' ') {
			tableName += line.at(i);
			i++;
		}
		return tableName;
	}

	static void processBeforeCommand(string& line, string& tableOrFieldName, Constants command) {
		removeCommandFromLine(line, command);

		switch (command) {
		case Constants::CREATE_TABLE:
		case Constants::DELETE_FROM:
		case Constants::DISPLAY_TABLE:
		case Constants::FROM:
		case Constants::UPDATE:
		case Constants::SET:
		case Constants::DROP_TABLE:
		case Constants::INSERT_INTO:
		case Constants::SELECT:
			try {
				tableOrFieldName = getTableOrFieldName(line);
				line = line.substr(tableOrFieldName.size());
			}
			catch (SQLException e) {
				cout << "-------------";
				cout << endl << e.what() << endl;
			}
			break;
		case Constants::WHERE:
		case Constants::VALUES:
			break;
		}
	}

	static void removeCommandFromLine(string& line, Constants command) {
		switch (command) {
		case Constants::CREATE_TABLE:
			line = line.substr(13);
			break;
		case Constants::DROP_TABLE:
			line = line.substr(11);
			break;
		case Constants::INSERT_INTO:
			line = line.substr(12);
			break;
		case Constants::UPDATE:
			line = line.substr(7);
			break;
		case Constants::DISPLAY_TABLE:
			line = line.substr(14);
			break;
		case Constants::DELETE_FROM:
			line = line.substr(12);
			break;
		case Constants::SELECT:
			line = line.substr(7);
			break;
		case Constants::VALUES:
			line = line.substr(8);
			break;
		case Constants::SET:
			line = line.substr(5);
			break;
		case Constants::FROM:
			line = line.substr(6);
			break;
		case Constants::WHERE:
			line = line.substr(7);
			break;
		}
	}


};
