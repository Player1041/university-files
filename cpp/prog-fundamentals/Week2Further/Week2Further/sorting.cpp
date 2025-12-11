#include <iostream>

using namespace std;

int main4() {
	int first;
	int second;
	int third;

	cout << "Enter a number: ";
	cin >> first;

	cout << "Enter another number: ";
	cin >> second;

	cout << "Enter another another number: ";
	cin >> third;

	cout << "The numbers in ascending order are: ";

	if (first <= second && second <= third) {
		cout << first << ", " << second << ", " << third << endl;
	}
	else if (first <= third && third <= second) {
		cout << first << ", " << third << ", " << second << endl;
	}
	else if (second <= first && first <= third) {
		cout << second << ", " << first << ", " << third << endl;
	}
	else if (second <= third && third <= first) {
		cout << second << ", " << third << ", " << first << endl;
	}
	else if (third <= first && first <= second) {
		cout << third << ", " << first << ", " << second << endl;
	}
	else if (third <= second && second <= first) {
		cout << third << ", " << second << ", " << first << endl;
	}
	return 0;
}

