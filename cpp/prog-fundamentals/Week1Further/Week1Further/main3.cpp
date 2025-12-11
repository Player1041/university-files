#include <iostream>

using namespace std;

int main3() {
	int total = 0;
	int input;

	cout << "Enter a number: ";
	cin >> input;
	total = total + input;

	cout << "Enter a number: ";
	cin >> input;
	total = total + input;

	cout << "Enter a number: ";
	cin >> input;
	total = total + input;

	cout << "Enter a number: ";
	cin >> input;
	total = total + input;

	cout << "The average of the four integers is: " << total / 4 << endl;
	return 0;
}