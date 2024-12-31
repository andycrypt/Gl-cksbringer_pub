#pragma once

#include <vector>
#include <chrono>
#include <string>

class parse {
public:
	static void htmlpars(const std::string &ghtml,const std::chrono::year_month_day &from);
	static void jsonfparse(bool reload);
	static void jp(std::string inp,bool reload);
	static void wout_euromil(bool trunc, size_t cnt);
	static void imp_euromil();
	static void imp_rdnr();
	//static void log_winnr(const std::vector<int>& wnr, const std::vector<int>& star);
};

