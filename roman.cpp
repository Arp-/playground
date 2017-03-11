/**
 * History:
 * So i was interviewing for a job at a company and this task was the computer
 * test in the 4th round, i miserably failed to do this problem, and it looked
 * easy at the first glance, and i just wanted to prove myself that i can do it,
 * or at least find the reason why i could not do the first task in 2 hours.
 *
 * Problem:
 * Well i'm simplifiing it and there are probably flaws in my memory so i
 * cannot claim that these were the exact tasks, but they're pretty simmilar
 *
 * 1. Convert the roman number to arabian numbers according to the 'extended
 * roman number' specification, which was basicly the general rule except you
 * can use subtraction for letter AB, where A < B, this way the 'IM' roman
 * literal could translated to '999' in arabian, and 'IIM' was an error,
 * but then what was 'IMIM' ?
 *
 * 2. I forgot this one, i think it was to write -1 on error
 *
 * 3. Was to convert the arabian number to common_roman numbers
 *
 * 4. Was to convert the arabian_number to extended_roman_numbers
 *    NOTE actually this is a superset of the common_roman_numbers so generally
 *    it's okay to use the common one.
 *
 *
 *
 * Final Notes:
 * This is a really ugly task. That aside, if i'd have red the questions more
 * carefully i probably could have finished with one of the questions in time.
 *
 * To code and test this program it took me about 4-5 hours, in my usual linux &
 * vim environment, well i was commenting, and i was not in a
 * hurry, but i had searched a few clues beforehand on the internet,
 * and probably there are a few errors here, and there are a few things that 
 * i've forgot form the paper,
 * BUT it still took me more time to finish this than what was available, well
 * i'm not a competitive programmer :'D
 *
 * Well i'm a bit sad :(
 *
 * Sorry for my ramblings i just wanted to have this off my shoulders.
 */

#include <iostream>
#include <cstring>
#include <array>

static int letter_to_num(char letter) {
	switch (letter) {
		case 'I': return 1;
		case 'V': return 5;
		case 'X': return 10;
		case 'L': return 50;
		case 'C': return 100;
		case 'D': return 500;
		case 'M': return 1000;
	}
	return -1;
}

/**
 * NOTE:
 * These roman numbers are really in the gray area, i mean if i only use
 * subtraction then they can basically be okay with numbers like 
 * 'IMIMIM' -> 2997
 * 'IVM' -> 994
 * but then 'VIX' -> 14, and 'XIV' -> 14 ?
 * oh well let's go with this one
 * oh i had to make sure that there are no more than 3 letters next to each
 * other, but then what about numbers like 'IMIMIMIM' ???
 */
static int from_roman(const char* roman) {

	int max = 0;
	int prev = 0;
	int num = 0;
	int same_count = 0;
	for (int i = strlen(roman)-1; i >= 0; i--) {
		int val = letter_to_num(roman[i]);
		if (val == -1) { return -1; } // case of error return error
		if (val == prev) { 
			same_count++;
		} else { 
			same_count = 0;
		}
		if (val < max && val < prev) { // subtractive
			num -= prev;
			num += (prev - val);
		} else if (val >= max && val >= prev) { // additive??
			// because there is 3 equal sign between 4 numbers
			if (same_count >= 3) { return -1; }
			num += val;
			max = val;
		} else { 
			return -1; 
		}
		prev = val;
	}
	return num;
}

typedef std::array<int, 7> coin_list_t;
typedef std::array<char, 7> coin_list_repr_t;
constexpr coin_list_t coin_list = { 1000, 500, 100, 50, 10, 5, 1 };
constexpr coin_list_repr_t coin_list_repr = { 'M', 'D', 'C', 'L', 'X', 'V', 'I' };

static bool is_10_pow(int num) {
	return (num % 10 == 0) || num == 1;
}


/**
 * NOTE this is really ugly,
 */
static std::string process_number_list(const coin_list_t& num_list) {
	std::string repr = "";
	int i = 0;
	for (int i = 0; i < num_list.size(); i+=2) {
		int j = i-1;
		auto& num_i = num_list[i];

		if (num_i == 4 && i != 0) {
			auto& num_j = num_list[j];
			if (num_j == 1) {
				repr.push_back(coin_list_repr[i]);
				repr.push_back(coin_list_repr[i-2]);
			} else {
				repr.push_back(coin_list_repr[i]);
				repr.push_back(coin_list_repr[i-1]);
			}
		} else {
			repr.insert(repr.end(), num_i, coin_list_repr[i]);
		}
	}
	return repr;
}

static std::string to_roman(int num) {
	if (num <= 0) { return "-1"; }
	coin_list_t number_list = { 0,0,0,0,0,0,0 };
	int i = 0;
	while (num > 0) {
		int coin = coin_list[i];
		int div = num / coin;
		if (div > 0) {
			num -= coin * div;
			number_list[i] += div;
		} else {
			i++;
		}
	}
	return process_number_list(number_list);
}


int main(int argc, char* argv[]) {

	if (argc < 2) { 
		fprintf(stderr, "Usage: %s <arabian_or_roman_number1> <arabian_or_roman_number2>...\n", argv[0]);
		fprintf(stderr, "Example %s 1 489 IMCC MDC\n", argv[0]);
		return -1;
	}

	printf("arabian (-1 on error): ");
	for (int i = 1; i < argc; i++) {
		int num = from_roman(argv[i]);
		printf("%d ", num);
	}
	putc('\n', stdout);
	printf("roman (-1 on error): ");
	for (int i = 1; i < argc; i++) {
		std::cout << to_roman(atoi(argv[i])) << " ";
	}
	return 0;
}

