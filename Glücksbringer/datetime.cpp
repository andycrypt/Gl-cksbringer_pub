#include "datetime.h"
#include "netw.h"
#include "game.h"

using namespace std;
using namespace chrono;

month datetime::get_month(const string &gm) {
	for (auto m = January; m < December; m++) {
		//if (format(locale("de_CH.UTF8"),"{:L%B}", m) == gm) { return m; }
		if (format(locale(locale("").name()), "{:L%B}", m) == gm) { return m; }
	}
	return January; //-->todo return???
}

string datetime::get_formdmy(const year_month_day &ymd) {
	return format("{:%d-%m-%Y}", ymd);
}

string datetime::get_formymd(const year_month_day &ymd) {
	return format("{:%Y-%m-%d}", ymd);
}

year_month_day datetime::ymdret(const string &inp) {
	stringstream ss(inp);
	year_month_day ymd;
	from_stream(ss, "%d-%m-%Y", ymd);
	return ymd;
}

year_month_day datetime::ymdret_today(const system_clock::time_point &tp) {
	stringstream ss(format("{:%Y-%m-%d}", tp));
	year_month_day ymd;
	from_stream(ss, "%Y-%m-%d", ymd);
	return ymd;
}

year_month_day datetime::ymdret_addday(const year_month_day &ymdin, int day) {
	year_month_day_last ymdl{ ymdin.year(),ymdin.month() / last };
	if (day < 0) {
		if (static_cast<unsigned>(ymdin.day()) + day < static_cast<unsigned>(ymdl.day())) {
			year_month_day ymdtmp = ymdin;
			while (day != 0) {
				if (static_cast<unsigned>(ymdtmp.day()) == 1) {
					ymdl -= months(1);
					ymdtmp = ymdl.year() / ymdl.month() / ymdl.day();
					continue;
				}
				int dayup = static_cast<unsigned>(ymdtmp.day());
				dayup--; day++;
				ymdtmp = ymdl.year() / ymdl.month() / dayup;
			}
		}
		else {
			int dayup = static_cast<unsigned>(ymdin.day()) + day;
			year_month_day ymd = ymdin.year() / ymdin.month() / dayup;
			return ymd;
		}
		
	}
	if (static_cast<unsigned>(ymdin.day()) + day > static_cast<unsigned>(ymdl.day())) {
		year_month_day ymdtmp = ymdin;
		while (day != 0) {
			if (ymdtmp.day() == ymdl.day()) {
				ymdl += months(1);
				ymdtmp = ymdl.year() / ymdl.month() / 1;
				continue;
			}
			int dayup = static_cast<unsigned>(ymdtmp.day());
			dayup++; day--;
			ymdtmp = ymdl.year() / ymdl.month() / dayup;
		}
	}
	else {
		int dayup = static_cast<unsigned>(ymdin.day()) + day;
		year_month_day ymd = ymdin.year() / ymdin.month() / dayup;
		return ymd;
	}
	throw "something wrong addday";
}

string datetime::get_tp_hr(const system_clock::time_point &tp) {
	return format("{:%d.%m.%Y %X}", tp);
}

seconds datetime::get_sec(const std::chrono::system_clock::time_point& tp) {
	return std::chrono::duration_cast<std::chrono::seconds>(system_clock::now() - tp);
}

void datetime::clk(year_month_day from,const year_month_day &to) {
	year_month_day_last ymdl{ from.year(),from.month() / last };
	while (from <= to) {
		if (Euromillions::check_playday(from) && from != 25d/8/2023) {
			Euromillions::web_tgt(from);
		}            
		if (ymdl.day() == from.day()) {
			from = from.year() / from.month() / 1;
			from += months(1);
			ymdl = from.year() / from.month() / last;
			continue;
		}
		int dayup = static_cast<unsigned>(from.day());
		dayup++;
		from = from.year() / from.month() / dayup;
	}
	web::web_euromil();
}
