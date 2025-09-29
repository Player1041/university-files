#include <iostream>

using namespace std;

int main4() {
	int a;
	int b;

	int c;

	cout << "Enter first integer: ";
	cin >> a;

	cout << "Enter second integer: ";
	cin >> b;

	c = a;
	a = b;
	b = c;

	cout << "After swapping, first integer is: " << a << endl;
	cout << "After swapping, second integer is: " << b << endl;

	return 0;
}