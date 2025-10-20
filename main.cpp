// Compile with g++ -O3 -fopenmp main.cpp
// The reason -O0 is so much slower than -O3 is probably because
// the loop logic gets bloated.

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

std::vector<int> random_numbers(int howMany) {
	std::vector<int> out;
	for (int i = 0; i < howMany; i++) {
		out.push_back((std::rand()&1) * 69);
	}

	return out;
}

std::vector<int> predictable_numbers(int howMany) {
	std::vector<int> out;
	for (int i = 0; i < howMany; i++) {
		int num = 0;
		if (i > howMany / 2) num = 69;
		out.push_back(num);
	}
	return out;
}

// Needs optimize("O2") to remove calling vec index twice
unsigned long long __attribute__ ((optimize("O2"))) normal_branch(std::vector<int> &vec, std::string label) {
	auto start = std::chrono::high_resolution_clock::now();

	unsigned long long sum = 0;

	// A normal "for i = 0..." loop would increment two registers.
	// This for-each avoids that.
	// Uncomment the pragma line to multi-thread it.
//#pragma omp parallel for reduction(+:sum)
	for (int &e : vec) {
		if (e == 69) {
			sum += e;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

	std::cout << label << ": " << duration.count() << " microseconds\n";
	std::cerr << sum << '\n';
	return sum;
}

unsigned long long __attribute__ ((optimize("O1"))) conditional_move(std::vector<int> &vec, std::string label) {
	auto start = std::chrono::high_resolution_clock::now();

	unsigned long long sum = 0;

	// A normal "for i = 0..." loop would increment two registers.
	// This for-each avoids that.
	// Uncomment the pragma line to multi-thread it.
//#pragma omp parallel for reduction(+:sum)
	for (int &e : vec) {
		asm(
			"leaq 69(%[sum]), %%rdx;" // rdx = sum + 69
			"cmpl $69, %[vec_i];" // if (vec[i] == 69)
			"cmove %%rdx, %[sum];" // sum = rdx;

			: [sum] "+r" (sum)
			: [vec_i] "m" (e)
			: "rbx", "edx", "rdx"
		);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

	std::cout << label << ": " << duration.count() << " microseconds\n";
	std::cerr << sum << '\n';
	return sum;
}

int main() {
	// Changing the size of these seem to do nothing to the relative speeds
	std::vector<int> vecRandom = random_numbers(10000); // 40 kB
	std::vector<int> vecPredictable = predictable_numbers(10000); // 40 kB

	unsigned long long sum1 = normal_branch(vecRandom, "[Normal branch] random");
	unsigned long long sum2 = normal_branch(vecPredictable, "[Normal branch] predictable");

	unsigned long long sum3 = conditional_move(vecRandom, "[Conditional move] random");
	unsigned long long sum4 = conditional_move(vecPredictable, "[Conditional move] predictable");
}
