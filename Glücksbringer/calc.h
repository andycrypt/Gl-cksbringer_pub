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
	//std::uniform_int_distribution<> start_rand{ 1, 201 };
	std::mt19937_64 gen;
	//std::seed_seq seq;
	void gen_new_rseed();

public:
	Random();
	unsigned __int64 seed_feed();
	unsigned __int64 seed_feed_experimental();
	bool rdnr_used(unsigned __int64 nr);
	//unsigned __int64 gen_seed();
	//unsigned __int64 gen_rdrand();
	void casheurm(std::vector<int> base, std::vector<int> basestar, int c);

	void casheurm2(std::vector<int> base, std::vector<int> basestar, int c);
	/*
	void lcnteurm(vector<int> a, vector<int> b) {
		int lcnt = 1;
		random_device rd;
		mt19937_64 gen(rd());
		uniform_int_distribution<> rng(1, 50);
		uniform_int_distribution<> star(1, 12);

		vector<int> winnr;
		vector<int> starnr;

		while(true) {
			for (auto x = 0; x < 5; x++) {
				auto a = rng(gen);
				while (auto it = ranges::find(winnr.begin(), winnr.end(), a) != winnr.end()) {
					a = rng(gen);
				}
				winnr.push_back(a);
			}

			for (auto x = 0; x < 2; x++) {
				auto a = star(gen);
				while (auto it = ranges::find(starnr.begin(), starnr.end(), a) != starnr.end()) {
					a = star(gen);
				}
				starnr.push_back(a);
			}

			sort(winnr.begin(), winnr.end(), less<int>());
			sort(starnr.begin(), starnr.end(), less<int>());

			if (a == winnr && b == starnr) { break; }
			lcnt++;
		}
		cout << "loopcnt: " << lcnt << endl;
	}*/
	//void eurm();
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