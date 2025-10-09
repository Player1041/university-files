#include <iostream>

using namespace std;

int main() {
	int num[5] = { 1,2,3,4,5 };
	int mirror[10];

	for (int i = 0; i < 10; i++) {
		if (i < 5) {
			mirror[i] = num[i];
		}
		else {
			mirror[i] = num[10 - 1 - i];
		}
			cout << "The mirrored array is: " << mirror[i] << endl;
	}

}