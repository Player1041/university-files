#include <iostream>

using namespace std;

int main3() {

	int ages[10] = { 29,34,21,19,16,45,34,57,99,40 };

	for (int i = 0; i < 10; i++) {
		cout << "Person " << i + 1 << " is " << ages[i] << " years old." << endl;
	}
	return 0;
}