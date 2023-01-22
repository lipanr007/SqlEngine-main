#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using std::exception;
using std::string;

class SQLException : public exception {
protected:
	string message = "";
public:
	SQLException(string message) {
		this->message = message;
	}
	string what() {
		return this->message;
	}
};