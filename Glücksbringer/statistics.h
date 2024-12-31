#pragma once
#include <vector>
#include <chrono>

class statistics {
public:
	void count_digits(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);
	void count_digitpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);
	struct Dig {
		int dig1;
		int dig2;
		int cnt;
	};
	struct Star {
		int dig1;
		int cnt;
	};
	struct Tip {
		int dig1, dig2, dig3, dig4, dig5, star1, star2;
	};
	std::vector<Dig> spv, digp;
	std::vector<Star> sv, dig;
	//std::vector<int> dig;
	void count_star(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);
	void count_starpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);
	void optimize_play_eurmil(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to);
	void gen_statistic(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to);

};

