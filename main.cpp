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

void print_vec(std::vector<int> &vec) {
	for (int &e : vec) {
		std::cout << e << ',';
	}
	std::cout << '\n';
}

unsigned long long do_thing(std::vector<int> &vec, std::string label) {
	auto start = std::chrono::high_resolution_clock::now();

	unsigned long long sum = 0;

#if 1
	for (int i = 0; i < vec.size(); i++) {
		asm(
			"leaq 69(%[sum]), %%rdx;" // rdx = sum + 69
			"cmpl $69, %[vec_i];" // if (vec[i] == 69)
			"cmove %%rdx, %[sum];" // sum += 69;

			: [sum] "+r" (sum)
			: [vec_i] "m" (vec[i])
			: "rbx", "edx", "rdx"
		);

		/*if (vec[i] == 69) {
			sum += vec[i];
		}*/
	}
#endif

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

	std::cout << label << ": " << duration.count() << "ms\n";
	std::cerr << sum << '\n';
	return sum;
}

int main() {
	std::vector<int> vecRandom = random_numbers(100000000); // 400 MB
	std::vector<int> vecPredictable = predictable_numbers(100000000); // 400 MB

	//std::vector<int> vecRandom = random_numbers(1000000); // 4 MB
	//std::vector<int> vecPredictable = predictable_numbers(1000000); // 4 MB

	//std::vector<int> vecRandom = random_numbers(65536); // 256 KiB
	//std::vector<int> vecPredictable = predictable_numbers(65536); // 256 KiB

	unsigned long long sum1 = do_thing(vecRandom, "random");
	unsigned long long sum2 = do_thing(vecPredictable, "predictable");
}
