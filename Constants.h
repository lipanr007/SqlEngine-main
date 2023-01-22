#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

using std::cout;
using std::endl;

enum class Constants {
	CREATE_TABLE,
	SELECT,
	DROP_TABLE,
	DISPLAY_TABLE,
	INSERT_INTO,
	DELETE_FROM,
	UPDATE,
	VALUES,
	SET,
	WHERE,
	FROM,
	NONE
};
