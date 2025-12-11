#include <iostream>

using namespace std;

int main4() {
	int seconds;
	cout << "Enter the number of seconds: ";
	cin >> seconds;

	int hours = seconds / 3600;
	int minutes = (seconds % 3600) / 60;
	int secs = seconds % 60;
	cout << hours << " hours, " << minutes << " minutes, " << secs << " seconds" << endl;

	return 0;
}