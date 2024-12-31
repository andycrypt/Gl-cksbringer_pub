#pragma once
//#include <Windows.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

#include "conv.h"

class Fhc : public conv {
public:
	Fhc() { prep_gpath(); gen_fpath(); }
	static std::filesystem::path gen_gpathf(const std::string &p);
	void gen_fpath();
	void prep_gpath();

	std::uintmax_t fsize(const std::filesystem::path& p);
	std::ifstream::pos_type filesize(const std::string &filename);
	static void fout(const std::filesystem::path &p,const std::string &sout);
	static void hrfout(const std::string &sout);
	static void fout_json(std::filesystem::path p, std::string sout);
	static std::string fin(const std::filesystem::path &pin);
	static std::string fin(const std::string &sin);
	static void rm(const std::filesystem::path &p);
	static void lout(const std::filesystem::path &p,const std::string &sout);
	static void fout_overwrite(const std::filesystem::path &p,const std::string &sout);
	//static void fout_overwrite_json(const std::filesystem::path& p, const std::string& sout);
	std::string fs_string(const std::ifstream &in);
	const std::string& ss_string(const std::stringstream &in) {
		return in.str();
	}
};