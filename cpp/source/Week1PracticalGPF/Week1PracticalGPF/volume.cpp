#include <iostream>
#include <string>
using namespace std;

int mainvolume() {
	float length, width, height, volume;
	cout << "Enter length: ";
	cin >> length;
	cout << "Enter width: ";
	cin >> width;
	cout << "Enter height: ";
	cin >> height;
	volume = length * width * height;
	cout << "The volume of the box is: " << volume << endl;
	return 0;
}