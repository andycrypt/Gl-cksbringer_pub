#pragma once
#include <vector>
#include <chrono>

class statistics {
	struct Tip {
		std::vector<int> dig, star;
	};
	struct Dig {
		std::vector<int> dig;
		int cnt;
	};
	std::vector<Dig> spv, digp, sv, dig;
	std::vector<Tip> tips;
public:
	void count_digits(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);

	void count_digitpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);

	void count_star(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);

	void count_starpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print = false);

	void optimize_play_eurmil(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to);

	//optional print avalible, maybe impl
	void gen_statistic(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to);

};

