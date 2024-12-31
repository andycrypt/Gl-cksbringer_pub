#pragma once
#include <SDKDDKVer.h>
#include <boost/asio/awaitable.hpp>
#include <string>
#include <chrono>
#include <vector>
#include <utility>


class web {
public:
	//web() { if (!BOOST_ASIO_HAS_CO_AWAIT) { throw "awaitables require C++20"; } }
	//static std::string web_s(const std::string &host,const std::string &port,const std::string &target);
	//static void get_day(const std::chrono::year_month_day &from);
	static void web_euromil();

	static std::pair<int, int> loop_prep(size_t x) {
		double y = static_cast<double>(x) / 10;
		double i;
		double f = std::modf(y, &i); f *= 10;
		return std::make_pair(static_cast<int>(i), static_cast<int>(round(f)));
	}

};




	
