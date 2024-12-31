#pragma once

//#include <chrono>
//#include <string>
#include <filesystem>
#include <vector>
#include "datetime.h"
#include "game.h"


struct Datastore {
	//std::chrono::system_clock::time_point start_time{ std::chrono::system_clock::now() };
	//zoned_time xx;
	datetime start_time;
	std::vector<Euromillions> eurmil;
	std::vector<unsigned __int64> rdstr;
	std::filesystem::path gpath;
	//std::filesystem::path gpath{ std::filesystem::current_path().generic_u8string() };
	std::filesystem::path wrklst;
	std::filesystem::path store,rdstrp,win_nr;
	std::filesystem::path log, errlog;
	
};
extern Datastore* d;



