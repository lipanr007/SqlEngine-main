#include <iostream>
#include "Menu.h"

int main(int argc, char* argv[]) {
	try {
		Menu::run(argc, argv);
	}
	catch (SQLException e) {
		cout << endl << e.what() << endl;
	}
}