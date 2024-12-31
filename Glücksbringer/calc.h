#pragma once

#include <random>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <chrono>

#include "statistics.h"



class Random : public statistics {
	unsigned __int64 _rdrand, _rdseed = 0;
	bool rdrand_used = false;
	std::uniform_int_distribution<unsigned __int64> base_rand{ 1, std::numeric_limits<unsigned __int64>::max()};
	
	std::mt19937_64 gen;
	
	void gen_new_rseed();

public:
	Random();
	unsigned __int64 seed_feed();
	unsigned __int64 seed_feed_experimental();
	bool rdnr_used(unsigned __int64 nr);

	void casheurm(std::vector<int> base, std::vector<int> basestar, int c);

	void casheurm2(std::vector<int> base, std::vector<int> basestar, int c);

	void eurm_mod();
	


	void swiss();
	void eurodream();
	

	struct {
		bool operator()(double a, double b) const {
			double  a1 = abs(a - 0);
			double  b1 = abs(b - 0);
			return a1 < b1;
		}
	} custom_sort;

	void predic() const;
	



	
};

class calc {
	inline static bool isgerade(int a) {
		return a % 2;
	}
	inline static bool isgerade2(int a) {
		return !(a & 1);
	}
};