#include <iostream>

using namespace std;

int main5() {

	float potatoPrice;
	float kilos;
	float taxPercent;

	cout << "Enter the price of potatoes per kilo without tax: ";
	cin >> potatoPrice;
	cout << "Enter the number of kilos: ";
	cin >> kilos;
	cout << "Enter the tax percent without the %: ";
	cin >> taxPercent;

	float totalPrice = potatoPrice * kilos;
	totalPrice = totalPrice + (totalPrice * taxPercent / 100);
	cout << "The total price including tax is: " << round(totalPrice * 100) / 100 << endl;


	return 0;
}