#define NOMINMAX
#include <Windows.h>
#include "parse.h"
//#include <chrono>
//#include <string>

#include <nlohmann/json.hpp>
#include "fhc.h"
#include "log_msg.h"
#include "datastore.h"



using namespace std;
using namespace chrono;
using json = nlohmann::json;


void parse::htmlpars(const string &ghtml,const year_month_day &from) {

	auto loc = ghtml.find("ballsAscending");
	auto loc2 = ghtml.find("ballsDrawn");
	auto winnrandstar = ghtml.substr(loc + 16, loc2 - loc - 64);
	if(from >= June/2/2015)
	loc = ghtml.rfind("DrawCH");
	loc2 = ghtml.find("superStar", loc + 35);
	auto superstar = ghtml.substr(loc2 - 12, 35);

	loc = ghtml.rfind("PrizeCH");
	loc2 = ghtml.rfind("PrizeBE");
	auto ptf = Fhc::gen_gpathf(datetime::get_formdmy(from)).generic_string();

	auto wintbl = conv::replall(ghtml.substr(loc - 9, loc2 - loc), "CHF", "AA");
	string ymd = "<div class=\"date\">";
	ymd.append(datetime::get_formdmy(from));
	ymd.append("</div>");

	Fhc::fout(d->wrklst, ptf);
	Fhc::fout(ptf, wintbl);
	Fhc::fout(ptf, winnrandstar);
	Fhc::fout(ptf, superstar);
	Fhc::fout(ptf, ymd);
}

void parse::jsonfparse(bool reload) {
	ifstream in{ d->wrklst,in.in };
	if (!in.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	for (string line; getline(in, line);) {
		conv::html2json(line);
		jp(Fhc::fin(line + ".json"),reload);
		Fhc::rm(line);
		Fhc::rm(line + ".json");
	}
}

void parse::jp(string inp, bool reload) {
	auto cnt = 0;
	Euromillions sav;
	json j = json::parse(conv::replall(inp, "AA", ""));
	for (const auto& [key, value] : j.items()) {
		Wintable wintbl;
		for (const auto& [key, value] : value.items()) {
			if (key == "Anzahl Richtige") {
				cnt++;
				switch (cnt) {
				case 14:
					wintbl.nr_hit = conv::replall(value," ","");
					break;
				case 15:
					sav.add_wnr(value);
					break;
				case 16:
					sav.add_wnr(value);
					break;
				case 17:
					sav.add_wnr(value);
					break;
				case 18:
					sav.add_wnr(value);
					break;
				case 19:
					sav.add_wnr(value);
					break;
				case 20:
					sav.add_star(value);
					break;
				case 21:
					sav.add_star(value);
					break;
				case 22:
					sav.add_superstar(value);
					break;
				case 23:
					sav.add_ymd(datetime::ymdret(value));
					break;
				default:
					if (value.is_string()) { wintbl.nr_hit = conv::replall(value, " ", ""); break; }
					if (value.is_number()) { wintbl.nr_hit = value.dump(); }
					break;
				}
				continue;
			}
			if (key == "Gewinn") {
				if (value == "-") { wintbl.win = 0; continue; }
				wintbl.win = stod(conv::replall(value, "'", ""));
				continue;
			}
			if (key.contains("Schweizer")) {
				if (value.is_string()) {
					if (value == "-") { wintbl.ch_w = 0; sav.need_reload = true; sav.add_wintable(wintbl); continue; }
					wintbl.ch_w = stoi(conv::replall(value, "'", "")); sav.add_wintable(wintbl); continue;
				}
				wintbl.ch_w = value;
				sav.add_wintable(wintbl);
				continue;
			}
			if (key == "Gesamtzahl Gewinner") {
				auto tmp = value.dump();
				if (value.is_string()) {
					if (tmp.contains("nicht")) { sav.add_jw(false);  wintbl.totalw = 0; continue; }
					if (value == "-") { wintbl.totalw = 0; sav.need_reload = true; continue; }
					wintbl.totalw = stoi(conv::replall(value, "'", "")); continue;

				}
				wintbl.totalw = stoi(conv::replall(tmp, "'", ""));
			}
		}
	}
	if (reload) {
		if (!sav.need_reload) {
			for (auto& x : d->eurmil) {
				if (x.ymd == sav.ymd) {
					x.wintbl = sav.wintbl;
					break;
				}
			}
		}
	}
	else {
		d->eurmil.push_back(sav);
	}
}



void parse::wout_euromil(bool trunc, size_t cnt) {
	vector<json> j;
	for (const auto& x : ranges::subrange(d->eurmil.begin() + cnt, d->eurmil.end())) {
		json tmp;
		tmp["Date"] = datetime::get_formdmy(x.ymd);
		tmp["Wnr"] = x.wnr;
		tmp["Star"] = x.star;
		tmp["Superstar"] = x.superstar;
		tmp["Jpwin"] = x.jackpot_win;
		tmp["internal_reload"] = x.need_reload;
		int i = 0;
		for (const auto& y : x.wintbl) {
			tmp["Wintable"][to_string(i)].push_back(json::object_t::value_type("Nr_Hit", y.nr_hit));
			tmp["Wintable"][to_string(i)].push_back(json::object_t::value_type("Win", y.win));
			tmp["Wintable"][to_string(i)].push_back(json::object_t::value_type("CH_W", y.ch_w));
			tmp["Wintable"][to_string(i)].push_back(json::object_t::value_type("Total_W", y.totalw));
			i++;
		}
		j.push_back(tmp);
	}
	if (trunc) {
		Fhc::rm(Fhc::gen_gpathf("hread_datastore.json"));
		Fhc::rm(d->store);
	}
	if(j.size() > 0) { for (const auto& x : j) { Fhc::fout_json(d->store, x.dump()); } }
}

void parse::imp_euromil() {
	if (exists(d->store)) {
		ifstream in{ d->store,in.in };
		if (!in.is_open()) {
			//throw std::runtime_error(std::to_string(GetLastError()));
			Logerr::log_msg(to_string(GetLastError()));
		}
		for (string line; getline(in, line);) {
			Euromillions sav;
			json j = json::parse(line);
			for (auto& [key, value] : j.items()) {
				if (key == "Date") { sav.add_ymd(datetime::ymdret(value)); continue; }
				if (key == "Wnr") { sav.wnr.assign_range(value); continue; }
				if (key == "Star") { sav.star.assign_range(value); continue; }
				if (key == "Superstar") { sav.add_superstar(value); continue; }
				if (key == "Jpwin") { sav.jackpot_win = value; continue; }
				if (key == "internal_reload") { sav.need_reload = value; continue; }
				if (key == "Wintable") {
					for (auto& [key, value] : value.items()) {
						Wintable wtb;
						for (auto& [key, value] : value.items()) {
							if (key == "CH_W") { wtb.ch_w = value; continue; }
							if (key == "Nr_Hit") { wtb.nr_hit = value; continue; }
							if (key == "Total_W") { wtb.totalw = value; continue; }
							if (key == "Win") { wtb.win = value; continue; }
						}
						sav.add_wintable(wtb);
					}
				}
			}
			d->eurmil.push_back(sav);
		}
	}
}
void parse::imp_rdnr() {
	if (exists(d->rdstrp)) {
		ifstream in{ d->rdstrp,in.in };
		if (!in.is_open()) {
			//throw std::runtime_error(std::to_string(GetLastError()));
			Logerr::log_msg(to_string(GetLastError()));
		}
		for (string line; getline(in, line);) {
			json j = json::parse(line);
			for (auto& [key, value] : j.items()) {
				if (key == "Nr") { d->rdstr.push_back(value); continue; }
			}
		}
	}
}
/*
void parse::log_winnr(const vector<int>& wnr, const vector<int>& star) {
	json j;
	j["Date"] = datetime::get_formdmy(datetime::ymdret_today());
	j["Game"] = "euromil"; //-->Todo as function param, maybe enum, incl uniform int distr to game ...
	j["Wnr"] = wnr;
	j["Star"] = star;
	j["Seed"] = ""; // -->why?? needet?
	Fhc::fout(d->win_nr, j.dump());//path???
}*/
