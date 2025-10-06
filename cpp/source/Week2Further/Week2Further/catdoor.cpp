#include <iostream>

using namespace std;

int main() {
	bool innerDoor = true;
	bool outerDoor = true;

	cout << "Is the inner door closed? (1 for yes, 0 for no): ";
	cin >> innerDoor;

	cout << "Is the outer door closed? (1 for yes, 0 for no): ";
	cin >> outerDoor;


	if (innerDoor && outerDoor) {
		cout << "Garfield is secure inside the house." << endl; // 4
	}
	else if (!innerDoor && outerDoor) {
		cout << "Garfield is not able to get out." << endl; // 3
	}
	else if (innerDoor && !outerDoor) {
		cout << "Garfield is in the porch." << endl; // 2
	}
	else {
		cout << "Garfield is avenging himself." << endl; // 1
	}
	cout << "Door status: Inner door: " << innerDoor << ", Outer door: " << outerDoor << endl;


	return 0;
}


