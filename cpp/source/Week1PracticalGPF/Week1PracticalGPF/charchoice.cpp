#include <iostream>
#include <string>
using namespace std;

int main() {

	char choice;

	cout << "Choose an option:" << endl;
	cout << "a. Hat\nb. Shirt\nc. Pants\n" << endl;
	cin >> choice;
	switch (choice) {
	case 'a':
		cout << "You chose Hat" << endl;
		break;
	case 'b':
		cout << "You chose Shirt" << endl;
		break;
	case 'c':
		cout << "You chose Pants" << endl;
		break;
	}
	return 0;
}