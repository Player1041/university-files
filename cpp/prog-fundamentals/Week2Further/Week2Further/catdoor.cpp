#include <iostream>

using namespace std;

int main() {

	bool innerDoor = true;
	bool outerDoor = true;
	bool continueProgram = true;
	cout << "Is the inner door closed? (1 for yes, 0 for no): ";
	cin >> innerDoor;

	cout << "Is the outer door closed? (1 for yes, 0 for no): ";
	cin >> outerDoor;

	while (continueProgram) {
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

		cout << "Would you like to change the door status? (1 for yes, 0 for no): ";
		bool changeStatus;
		cin >> changeStatus;
		if (changeStatus) {
			cout << "Which door would you like to change? (1 for inner, 2 for outer, 3 for both): ";
			int doorChoice;
			cin >> doorChoice;
			if (doorChoice == 1) {
				innerDoor = !innerDoor;
			}
			else if (doorChoice == 2) {
				outerDoor = !outerDoor;
			}
			else if (doorChoice == 3) {
				if (innerDoor && outerDoor) {
					cout << "Changing both doors will leave Garfield outside. Are you sure? (1 for yes, 0 for no): ";
					int confirm;
					cin >> confirm;
					if (!confirm) {
						cout << "No changes made." << endl;
						return 0;
					}
				}
				innerDoor = !innerDoor;
				outerDoor = !outerDoor;
			}
			else {
				cout << "Invalid choice. No changes made." << endl;
				return 0;
			}
			cout << "Door status changed." << endl;
		}
		else {
			cout << "Door status remains the same." << endl;
			continueProgram = false;
		}
	}

	return 0;
}


