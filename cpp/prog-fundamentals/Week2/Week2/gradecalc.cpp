#include <iostream>

using namespace std;

int main() {
	int mod1;
	int mod2;
	int mod3;
	int mod4;
	int mod5;
	int mod6;

	cout << "Enter the percent for each module: \n";
	cin >> mod1 >> mod2 >> mod3 >> mod4 >> mod5 >> mod6;

	if (mod1 < 0 || mod1 > 100 || mod2 < 0 || mod2 > 100 || mod3 < 0 || mod3 > 100 || mod4 < 0 || mod4 > 100 || mod5 < 0 || mod5 > 100 || mod6 < 0 || mod6 > 100) {
		cout << "Invalid input. Please enter a number between 0 and 100." << endl;
		return 1;
	}
	else {
		int average = (mod1 + mod2 + mod3 + mod4 + mod5 + mod6) / 6;
		if (average >= 85) {
			cout << "Your average is " << average << "%. A*. You passed." << endl;
		}
		else if (average >= 70  && average <= 84) {
			cout << "Your average is " << average << "%. A. You passed." << endl;
		}
		else if (average >= 60 && average <= 69) {
			cout << "Your average is " << average << "%. B. You passed." << endl;
		}
		else if (average >= 50 && average <= 59) {
			cout << "Your average is " << average << "%. C. You passed." << endl;
		}
		else if (average >= 40 && average <= 49) {
			cout << "Your average is " << average << "%. D. You passed." << endl;
		}
		else if (average >= 30 && average <= 39) {
			cout << "Your average is " << average << "%. E. You failed." << endl;
		}
		else if (average <= 29) {
			cout << "Your average is " << average << "%. F. You failed." << endl;
		}
	}
	return 0;
}



