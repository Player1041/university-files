
#include <iostream>

using namespace std;

int maina() {
	float f;
    float c;

	cout << "Input fahrenheit" << endl;
	cin >> f;

	c = (f - 32) * 5 / 9;
	cout << f << " Fahrenheit in Celsius is " << c << endl;
	return 0;
}