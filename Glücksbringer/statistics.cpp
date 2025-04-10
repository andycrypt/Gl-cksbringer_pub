#include "statistics.h"
#include "datastore.h"
#include "log_msg.h"
#include "conv.h"
#include <ranges>

using namespace std;
using namespace chrono;

void statistics::count_digits(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print) {
	for (auto x = 1; x <= 50; x++) {
		for (const auto& y : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
			bool brk = true;
			if (ranges::contains(y.wnr, x)) {
				//if (dig.size() < x) { dig.push_back(Dig({ x }, 1)); } else { dig[dig.size()-1].cnt++; } // <-- TRY
				for (auto& z : dig) {
					if (z.dig.front() == x) { z.cnt++; brk = false; break; }
				}
				if (brk) { dig.push_back(Dig({ x }, 1)); }
			}
		}
	}
}

void statistics::count_digitpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print) {
	for (auto x = 1; x <= 50; x++) {
		for (auto y = 1; y <= 50; y++) {
			if (x == y || x > y) { continue; }
			const auto tmp = { x,y };
			for (const auto& z : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
				bool brk = true;
				if (ranges::includes(z.wnr, tmp)) {
					for (auto& zz : digp) {
						if (ranges::includes(zz.dig, tmp)) {
							zz.cnt++; brk = false; break;
						}
					}
					if (brk) { digp.push_back(Dig({ x,y }, 1)); }
				}
			}
		}
	}
}

void statistics::count_star(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print) {
	for (auto x = 1; x <= 12; x++) {
		for (const auto& y : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
			bool brk = true;
			if (ranges::contains(y.star, x)) {
				for (auto& z : sv) {
					if (z.dig.front() == x) { z.cnt++; brk = false; break; }
				}
				if (brk) { sv.push_back(Dig({ x }, 1)); }
			}
		}
	}
}

void statistics::count_starpair(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to, bool print) {
	for (auto x = 1; x <= 12; x++) {
		for (auto y = 1; y <= 12; y++) {
			if (x == y || x > y) { continue; }
			const auto tmp = { x,y };
			for (const auto& z : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
				bool brk = true;
				if (ranges::includes(z.star, tmp)) {
					for (auto& zz : spv) {
						if (ranges::includes(zz.dig, tmp)) {
							zz.cnt++; brk = false; break;
						}
					}
					if (brk) { spv.push_back(Dig({ x,y }, 1)); }
				}
			}
		}
	}
}

void statistics::optimize_play_eurmil(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to) {
	std::chrono::system_clock::time_point tmptp = std::chrono::system_clock::now();
	gen_statistic(from, to);
	//Logerr::log_elapsedtime(to_string(datetime::get_sec(tmptp)));
	double tip_cost = 3.5;
	int cnt_tip = 0;
	double total_cost = 0.0;
	double total_win = 0.0;
	auto tmp = dig.size();
	auto tmps = sv.size();
	for (const auto& x : ranges::subrange(dig.begin() + tmp - 6, dig.end())) { tips[0].dig.append_range(x.dig); } //change if function use multiple tips
	for (const auto& x : ranges::subrange(sv.begin() + tmps - 3, sv.end())) { tips[0].star.append_range(x.dig); }
	int tip = tips.size();

	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(1d / 1 / 2025), d->eurmil.begin() + Euromillions::get_pd(datetime::ymdret_addday(datetime::ymdret_today(), -1)))) {
		total_cost += tip_cost * tip; cnt_tip++;
		int cnt_tmp = 0, star_tmp = 0;
		for (const auto& y : tips) {
			for (const auto& z : y.dig) {
				if (ranges::contains(x.wnr, z)) { cnt_tmp++; }
			}
			for (const auto& z : y.star) {
				if (ranges::contains(x.star, z)) { star_tmp++; }
			}
		}
		if (cnt_tmp > 1) {
			if (star_tmp > 0) {
				string str_tmp = to_string(cnt_tmp);
				str_tmp.append("+");
				str_tmp.append(to_string(star_tmp));
				for (const auto& y : x.wintbl) {
					if (y.nr_hit == str_tmp) { total_win += y.win; break; }
				}
			}
			string str_tmp = to_string(cnt_tmp);
			for (const auto& y : x.wintbl) {
				if (y.nr_hit == str_tmp) { total_win += y.win; break; }
			}
		}
	}
	cout << "Anzahl gespielte Tips: " << cnt_tip << " Total Ausgaben: " << total_cost << " Total Einnahmen: " << total_win << endl;
	//cout << "Tip Nr: " 
	/*
	spv.clear();
	sv.clear();
	digp.clear();
	dig.clear();*/ // maybe later change, atm no need to clean
}

void statistics::gen_statistic(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to) {
	count_digits(from, to);
	count_digitpair(from, to);

	count_starpair(from, to);
	count_star(from, to);




}
