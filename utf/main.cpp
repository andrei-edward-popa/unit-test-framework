#include <iostream>

int multiply(int a, int b) {
	return a * b;
}

struct Test {

	int tmp;
	float ftmp;
	void print() {
		std::cout << "Inside method 'print' from class 'Test'\n";
	}
	int sum(int a, int b) {
		return a + b + tmp;
	}
	Test() : tmp(10), ftmp(12.45f) { };
	Test(int a, float b) : tmp(a), ftmp(b) { }
	
	Test operator+(const Test& test) {
		return Test(tmp + test.tmp, ftmp + test.ftmp);
	}

};

int main() {
	int a;
	for (uint64_t i = 0; i < 1000000000; i++) {
	
	}
	std::cin >> a;
	std::cout << a;
	return 0;
}
