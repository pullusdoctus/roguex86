#include <iostream>

extern long int add_numbers(long int a, long int b);

int main() {
	int result = add_numbers(5, 7);
	std::cout << "5 + 7 = " << result << std::endl;
	return EXIT_SUCCESS;
}