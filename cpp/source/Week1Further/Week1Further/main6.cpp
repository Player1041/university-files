#include <iostream>

using namespace std;

int main() {
	int pointa;
	int pointb;

	cout << "Enter first coordinate: ";
	cin >> pointa;
	cout << "Enter second coordinate: ";
	cin >> pointb;

	cout << "The distance between the two points is: " << abs(pointa - pointb) << endl;
	return 0;
}