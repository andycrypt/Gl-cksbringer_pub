#define NOMINMAX
#include <Windows.h>

#include "fhc.h"


#include "datastore.h"
#include "parse.h"
#include "log_msg.h"

#include <nlohmann/json.hpp>



using namespace std;
using namespace filesystem;

using json = nlohmann::json;

 path Fhc::gen_gpathf(const string &p) {
	wstring tmp = d->gpath;
	tmp.append(string_to_wide_string(p));
	return tmp;
}

 void Fhc::gen_fpath() {
	 d->wrklst = gen_gpathf("worklist.txt"); d->store = gen_gpathf("datastore.json"); d->log = gen_gpathf("log.json"); d->errlog = gen_gpathf("errlog.txt"); d->rdstrp = gen_gpathf("rand.json"); d->win_nr = gen_gpathf("win_nr.json");
}

 void Fhc::prep_gpath() {
	wstring tmp = std::filesystem::current_path().generic_wstring();
	tmp.erase(tmp.begin() + tmp.find_last_of(L"/") + 1, tmp.end());
	tmp.erase(tmp.begin() + tmp.find_last_of(L"/") + 1, tmp.end());
#if defined( _DEBUG) && defined(_WIN64) 
	tmp.append(L"x64/Debug/");
#else
	tmp.append(L"x64/Release/");
#endif
	d->gpath = tmp;
}

 uintmax_t Fhc::fsize(const path& p) {
	return file_size(p);
}

 ifstream::pos_type Fhc::filesize(const string &filename)
{
	ifstream in(filename, ifstream::ate | ifstream::binary);
	return in.tellg();
}

 void Fhc::hrfout(const string &sout) {
	 json jsout = json::parse(sout);
	fstream out{ gen_gpathf("hread_datastore.json"), ios::out | ios::app };
	if (!out.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	out << jsout.dump(4) << endl;
}

 void Fhc::fout_json(std::filesystem::path p, std::string sout) {
	 fstream out{ p, ios::out | ios::app };
	 if (!out.is_open()) {
		 Logerr::log_msg(to_string(GetLastError()));
	 }
	 out << sout << endl;
#if defined (_DEBUG)
	 hrfout(sout);
#endif // defined (_DEBUG)

 }

 void Fhc::fout(const path &p,const string &sout) {
	fstream out{ p, ios::out | ios::app };
	if (!out.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	out << sout << endl;
}

 string Fhc::fin(const path &pin) {
	ifstream in{ pin,in.in };
	if (!in.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

 string Fhc::fin(const string &sin) {
	ifstream in{ sin,in.in };
	if (!in.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

 void Fhc::rm(const path &p) {
	error_code ec;
	if (!remove(p, ec)) {
		//throw ec.message();
		Logerr::log_msg(ec.message());
	}
}

 void Fhc::lout(const path &p,const string &sout) {
	fstream out{ p, ios::out | ios::app };
	if (!out.is_open()) {
		throw "Log error";
	}
	out << sout << endl;
}

 void Fhc::fout_overwrite(const path &p,const string &sout) {
	fstream out{ p, ios::out | ios::trunc };
	if (!out.is_open()) {
		Logerr::log_msg(to_string(GetLastError()));
	}
	out << sout << endl;
}



 string Fhc::fs_string(const ifstream &in) {
	stringstream ss;
	ss << in.rdbuf();
	return ss.str();
}
