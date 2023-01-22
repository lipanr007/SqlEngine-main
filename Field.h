#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include "Exception.h"

using std::cout;
using std::endl;
using std::ostream;
using std::istream;


class Field {
protected:
	string name = "";
	string type = "";
	string value = "";
public:
	Field() {

	}

	Field(string name, string type, string val)
	{
		this->name = name;
		this->type = type;
		this->value = val;
	}

	Field(const Field& field)
	{
		this->name = field.name;
		this->type = field.type;
		this->value = field.value;
	}

	~Field() {

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

	string getType()
	{
		return this->type;
	}

	void setType(string type)
	{
		if (type.length() >= 0)
			this->type = type;
	}

	string getDefaultValue()
	{
		return this->value;
	}

	void setDefaultValue(string value)
	{
		if (value.length() >= 0)
			this->value = value;
	}

	Field& operator=(const Field& field)
	{
		this->name = field.name;
		this->type = field.type;
		this->value = field.value;
		return *this;
	}

	bool operator!=(Field field)
	{
		if (this->name != field.name) {
			return true;
		}
		else
			return true;
	}

	friend ostream& operator<<(ostream& console, Field& field);
	friend istream& operator>>(istream& input, Field& field);
};

istream& operator>>(istream& input, Field& field)
{
	cout << "Field Name: " << endl;
	input >> field.name;
	cout << "Field Type: " << endl;
	input >> field.type;
	cout << "Field Value: " << endl;
	input >> field.value;

	return input;
}

ostream& operator<<(ostream& console, Field& field)
{
	console << "Field Name: " << field.name << endl;
	console << "Field Type: " << field.type << endl;
	console << "Field Value:" << field.value << endl;

	return console;
}