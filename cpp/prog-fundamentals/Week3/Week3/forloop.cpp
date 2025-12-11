#include <iostream>

using namespace std;

int main1() {

	int mode;

	cout << "Enter mode (1, 2 or 3): ";
	cin >> mode;

	if(mode > 3 || mode < 1) {
		cout << "Invalid mode!" << endl;
		return 1;
	}
	else if (mode == 1) {
		for (int i = 0; i <= 10; i++) {
			cout << i << " ";
		}
	}

	else if (mode == 2) {
		for (int i = 4; i <= 8; i++) {
			cout << i << " ";
		}
	}
	else if (mode == 3) {
		for (int i = 10; i >= 2; i--) {
			cout << i << " ";
		}
	}

	return 0;
}