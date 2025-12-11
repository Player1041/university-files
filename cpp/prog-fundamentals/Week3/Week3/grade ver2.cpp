#include <iostream>

using namespace std;

int main2() {

	int module[6];
	int average = 0;
	bool passedModule[6] = { 0,0,0,0,0,0 };

	for (int i = 0; i < 6;) {
		cout << "Enter grade for module " << i + 1 << ": ";
		cin >> module[i];

		if (module[i] < 0 || module[i] > 100) {
			cout << "Invalid grade, try again.\n";
		}
		else {
			if(module[i] >= 40) {
				passedModule[i] = true;
			}
			else {
				passedModule[i] = false;
			}
			i++;
		}
	}

	for (int i = 0; i < 6; i++) {
		average += module[i];
	}

	average /= 6;

	if (average >= 85) {
		cout << "Your average is " << average << "%. A*.\nYou passed." << endl;
	}
	else if (average >= 70 && average <= 84) {
		cout << "Your average is " << average << "%. A.\nYou passed." << endl;
	}
	else if (average >= 60 && average <= 69) {
		cout << "Your average is " << average << "%. B.\nYou passed." << endl;
	}
	else if (average >= 50 && average <= 59) {
		cout << "Your average is " << average << "%. C.\nYou passed." << endl;
	}
	else if (average >= 40 && average <= 49) {
		cout << "Your average is " << average << "%. D.\nYou passed." << endl;
	}
	else if (average >= 30 && average <= 39) {
		cout << "Your average is " << average << "%. E.\nYou failed." << endl;
	}
	else if (average <= 29) {
		cout << "Your average is " << average << "%. F.\nYou failed." << endl;
	}

	int totalPassedModules = 0;
	for (int i = 0; i < 6; i++) {
		if (passedModule[i]) {
			cout << "\nModule " << i + 1 << ": passed." << endl;
			totalPassedModules++;
		}
		else {
			cout << "\nModule " << i + 1 << ": failed." << endl;
		}
	}

	cout << "\nYou passed " << totalPassedModules << " modules." << endl;
	return 0;
}