#include <iostream>

using namespace std;

int main4() {
	int numbers[20];
	for (int i = 0; i < 20; i++) {
		numbers[i] = i * i;
		cout << "The square of " << i << " is " << numbers[i] << "." << endl;
	}
	return 0;
}