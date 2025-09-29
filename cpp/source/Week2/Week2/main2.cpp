#include <iostream>

using namespace std;

int main2() {
	int a;
	cout << "Enter an integer: ";
	cin >> a;

	if (a % 2 == 0) {
		cout << a << " is even" << endl;
	}
	else {
		cout << a << " is odd" << endl;
	}
	return 0;
}