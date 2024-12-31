#include "statistics.h"
#include "datastore.h"
#include "log_msg.h"
#include "conv.h"
#include <ranges>

using namespace std;
using namespace chrono;

void statistics::count_digits(const year_month_day& from, const year_month_day& to, bool print) {
	vector<int> digit;
	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
		ranges::copy(x.wnr, back_inserter(digit));
	}
	//vector<pair<__int64, int>> cnt;
	for (auto x = 1; x <= 50; x++) {
		//cnt.push_back(make_pair(ranges::count(digit, x), x));
		dig.push_back(Star(x, ranges::count(digit, x)));
	}
	ranges::sort(dig, {}, &Star::cnt);
	if (print)
		for (const auto& x : dig) {
		cout << "Anzahl: " << x.cnt << " Zahl: " << x.dig1 << endl;
	}
}

void statistics::count_digitpair(const year_month_day& from, const year_month_day& to, bool print) {
	//vector<Dig> store;
	vector<pair<int, int>> cblock;
	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
		vector<int> digit;
		ranges::copy(x.wnr, back_inserter(digit));
		do {
			const auto& brk = ranges::size(digit) - 1;
			for (const auto& [idx, num] : ranges::views::enumerate(digit)) {
				bool brk2 = true;
				if (idx == brk) { break; }
				for (const auto& [f, s] : cblock) {
					if (f == num && s == digit[idx + 1] || f == digit[idx + 1] && s == num) { brk2 = false; break; }
				}
				if (brk2) {
					cblock.push_back(make_pair(num, digit[idx + 1]));
					vector<int> dig1{ num, digit[idx + 1] };
					for (const auto& y : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
						if (x.wnr == y.wnr) { continue; }
						auto a = ranges::search(y.wnr, dig1);
						if (!a.empty()) {
							for (auto& z : digp) {
								if (z.dig1 == a[0] && z.dig2 == a[1] || z.dig1 == a[1] && z.dig2 == a[0]) {
									z.cnt++; brk2 = false; break;
								}
							}
							if (brk2) {
								digp.push_back(Dig(a[0], a[1], 1));
							}

						}
					}
				}
			}
		} while (ranges::next_permutation(digit).found);
	}
	ranges::sort(digp, {}, &Dig::cnt);
	if (print)
		for (const auto& x : digp) {
		cout << x.dig1 << " " << x.dig2 << " cnt:" << x.cnt << endl;
	}
}

void statistics::count_star(const year_month_day& from, const year_month_day& to, bool print) {
	//vector<Star> sv;
	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
		for (const auto& y : x.star) {
			bool brk = true;
			for (auto& z : sv) {
				if (y == z.dig1) { z.cnt++; brk = false; break; }
			}
			if (brk) { sv.push_back(Star(y, 1)); }
		}
	}
	ranges::sort(sv, {}, &Star::cnt);
	if (print)
		for (const auto& x : sv) {
		cout << x.dig1 << " cnt: " << x.cnt << endl;
	}
}

void statistics::count_starpair(const year_month_day& from, const year_month_day& to, bool print) {
	//vector<Dig> spv;
	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {
		bool brk = true;
		for (auto& y : spv) {
			if (y.dig1 == x.star[0] && y.dig2 == x.star[1]) { y.cnt++; brk = false; break; }
		}
		if (brk) { spv.push_back(Dig(x.star[0], x.star[1], 1)); }
	}
	ranges::sort(spv, {}, &Dig::cnt);
	if(print)
		for (const auto& x : spv) {
			cout << x.dig1 << " " << x.dig2 << " cnt:" << x.cnt << endl;
		}
}

void statistics::optimize_play_eurmil(const year_month_day& from, const year_month_day& to) {
	gen_statistic(from, to);
	double tip_cost = 3.5;
	int cnt_tip = 0;
	double total_cost = 0.0;
	double total_win = 0.0;
	vector<Tip> tips;
	auto tmp = dig.size();
	auto tmps = sv.size();
	tips.push_back(Tip(dig[tmp - 1].dig1, dig[tmp - 1].dig1, dig[tmp - 2].dig1, dig[tmp - 3].dig1, dig[tmp - 4].dig1,sv[tmps-1].dig1, sv[tmps - 2].dig1));
	int tip = tips.size();

	for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(1d / 1 / 2024), d->eurmil.begin() + Euromillions::get_pd(datetime::ymdret_addday(datetime::ymdret_today(), -1)))) {
		total_cost += tip_cost * tip; cnt_tip++;
		int cnt_tmp = 0, star_tmp = 0;
		for (const auto& y : x.wnr) {
			for (const auto& z : tips) {
				if (z.dig1 == y || z.dig2 == y || z.dig3 == y || z.dig4 == y || z.dig5 == y) { cnt_tmp++; }
			}
		}
		for (const auto& y : x.star) {
			for (const auto& z : tips) {
				if (y == z.star1 || y == z.star2) { star_tmp++; }
			}
		}
		if(cnt_tmp > 1) {
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

	//uniform_int_distribution<> rng(1, 50);
	//uniform_int_distribution<> star(1, 12);
	//mt19937_64 gen(Random::seed_feed_experimental());
	/*
	gen_statistic(from, to);
	vector<Star> a = dig;
	dig.clear();
	count_digits(1d/1/2024, datetime::ymdret_addday(datetime::ymdret_today(), -1));
	
	for (const auto& x : dig) {
		for (const auto& y : a) {
			if (y.dig1 == x.dig1) {
				cout << x.dig1 << " addnew " << x.cnt - y.cnt << endl;;
			}
		}
		
	}
*/
	//for (const auto& x : ranges::subrange(d->eurmil.begin() + Euromillions::get_pd(from), d->eurmil.begin() + Euromillions::get_pd(to))) {}
	spv.clear();
	sv.clear();
	digp.clear();
	dig.clear();
}

void statistics::gen_statistic(const std::chrono::year_month_day& from, const std::chrono::year_month_day& to) {
	count_starpair(from, to);
	
	count_star(from, to);
	
	count_digitpair(from, to);
	
	count_digits(from, to);
}
