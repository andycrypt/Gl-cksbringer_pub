
#include <ranges>
#include "game.h"
#include "datastore.h"

using namespace std;
/*
bool Euromillions::to_reload() {
	for (const auto& x : d->eurmil) {
		if (x.need_reload) { ed.rld.push_back(x); }
	}
	return ed.rld.size() > 0;
}*/

bool Euromillions::check_playday(const year_month_day &from) {
	return from._Calculate_weekday() == 2 || from._Calculate_weekday() == 5;
}

void Euromillions::web_tgt(const year_month_day& from) {
	string trgt = "https://www.euro-millions.com/de-ch/gewinnzahlen/";
	trgt.append(datetime::get_formdmy(from));
	ed.target.push_back(Msg(trgt,"",from));
}
/*
void Euromillions::web_tgt(const year_month_day& from) {
	string trgt = "de-ch/gewinnzahlen/";
	trgt.append(datetime::get_formdmy(from));
	ed.target.push_back(Msg(trgt, "", from));
}*/

int Euromillions::get_pd(const year_month_day& from) {
	for (const auto& [idx, x] : ranges::views::enumerate(d->eurmil)) {
		if (x.ymd >= from ) { return idx; }
	}
	return d->eurmil.size() - 1;
}

Euromillions_datastore ed;