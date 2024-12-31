#pragma once
#include <chrono>
#include <string>



class datetime {
	std::chrono::system_clock::time_point start_time{ std::chrono::system_clock::now() };
public:

	std::chrono::month get_month(const std::string &gm);
	static std::string get_formdmy(const std::chrono::year_month_day &ymd);
	static std::string get_formymd(const std::chrono::year_month_day &ymd);
	static std::chrono::year_month_day ymdret(const std::string &inp);
	static std::chrono::year_month_day ymdret_today(const std::chrono::system_clock::time_point &tp = std::chrono::system_clock::now());
	static std::chrono::year_month_day ymdret_addday(const std::chrono::year_month_day &ymdin,int day);
	static std::string get_tp_hr(const std::chrono::system_clock::time_point &tp);
	static std::chrono::seconds get_sec(const std::chrono::system_clock::time_point& tp);
	int count_days() {

	}
	int count_playdays() {

	}
	static void clk(std::chrono::year_month_day from,const std::chrono::year_month_day &to);
	
};