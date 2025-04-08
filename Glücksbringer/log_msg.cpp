#include "log_msg.h"
#include "fhc.h"
#include "datastore.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

void Logerr::log_msg(const std::string_view message, const std::source_location &location,const std::chrono::system_clock::time_point &tp) {
    //wout(message, location, tp);
    nlohmann::json tmp;
    tmp["Time"] = datetime::get_tp_hr(tp);
    tmp["Message"] = message;
    tmp["File"] = location.file_name();
    tmp["Function"] = location.function_name();
    tmp["line"] = location.line();
    tmp["column"] = location.column();
    Fhc::lout(d->log, tmp.dump());
}

void Logerr::log_rdseed(unsigned __int64 nr, const std::chrono::system_clock::time_point& tp) {
    nlohmann::json tmp;
    tmp["Time"] = datetime::get_tp_hr(tp);
    tmp["Nr"] = nr;
    Fhc::lout(d->rdstrp, tmp.dump());
}

void Logerr::log_euromil(const vector<int>& nr, const vector<int>& star, const std::chrono::system_clock::time_point& tp, const std::chrono::system_clock::time_point& tpt) {
    nlohmann::json tmp;
    tmp["Time"] = datetime::get_tp_hr(tp);
    tmp["Playtime"] = datetime::get_tp_hr(tpt);
    tmp["Nr"] = nr;
    tmp["Star"] = star;
    tmp["gen_version"] = "todo";
    Fhc::lout(d->win_nr, tmp.dump());
}

void Logerr::log_cout(const std::string_view& message, const std::source_location& location) {
    nlohmann::json tmp;
    //tmp["Time"] = datetime::get_tp_hr(tp);
    tmp["Message"] = message;
    tmp["File"] = location.file_name();
    tmp["Function"] = location.function_name();
    tmp["line"] = location.line();
    tmp["column"] = location.column();
    cout << tmp << endl;
}

void Logerr::log_web(const std::string_view& message, const std::chrono::year_month_day& ymd, const std::source_location& location, const std::chrono::system_clock::time_point& tp) {
    nlohmann::json tmp;
    tmp["Time"] = datetime::get_tp_hr(tp);
    tmp["Message"] = message;
    tmp["Date"] = datetime::get_formdmy(ymd);
    Fhc::lout(d->weberr, tmp.dump());
}

