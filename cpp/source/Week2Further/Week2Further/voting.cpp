#include <iostream>
#include <string>
#include <cctype>


using namespace std;

int main3() {
	string name;
	int age;
	string country;
	string country_lower;

	cout << "Enter your name: ";
	getline(cin >> ws, name); // use getline to allow for spaces in name

	cout << "Enter your age: ";
	cin >> age;

	cout << "Enter your country of citizenship: ";
	getline(cin >> ws, country); // use getline to allow for spaces in country name

	for (char c : country) {
		country_lower += tolower(c);
	}

	if (age < 18) {
		cout << name << " cannot vote for another " << 18 - age << " years." << endl;
		return 1;
	}
	else {
		if (age >= 18 && country_lower == "uk" || country_lower == "england" || country_lower == "gb" || country_lower == "great britain" || country_lower == "britain" || country_lower == "united kingdom") {
			cout << name << " can vote in the UK." << endl;
			return 0;
		}
		else {
			cout << name << " cannot vote in the UK as they aren't a citizen of the UK." << endl;
			return 2;
		}
	}
	return 0;
}


