#include <iostream>

using namespace std;

int main6() {
	char choice;
	int num1, num2, result;

	cout << "Enter two numbers: ";
	cin >> num1 >> num2;

	do {
		cout << "\n1 | Add\n2 | Multiply\n3 | Divide\nQ | Exit\nEnter choice: ";
		cin >> choice;
		switch (choice) {
		case '1':
			result = num1 + num2;
			cout << "The result is: " << result << endl;
			break;
		case '2':
			result = num1 * num2;
			cout << "The result is: " << result << endl;
			break;
		case '3':
			if (num2 == 0) {
				cout << "Error: Division by zero!" << endl;
			}
			else {
				result = num1 / num2;
				cout << "The result is: " << result << endl;
			}
			break;
		case 'q':
		case 'Q':
			cout << "Exiting the program." << endl;
			break;
		default:
			cout << "Invalid choice, please try again." << endl;
		}
	}
	
	while (choice != 'q' && choice != 'Q');
	return 0;
}