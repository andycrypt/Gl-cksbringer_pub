#pragma once

#include <string>
#include <chrono>
#include <string_view>

using namespace std;
using namespace chrono;

struct Wintable {
    string nr_hit;
    double win;
    int ch_w;
    int totalw;
};

struct Msg {
    string tgt;
    string msg;
    year_month_day ymd;
};

struct Msg2 {
    //string tgt;
    string msg;
    year_month_day ymd;
};

class Euromillions {
public:
    year_month_day ymd;
    vector<int> wnr;
    vector<int> star;
    string superstar;
    vector<Wintable> wintbl;
    bool jackpot_win = true;
    bool need_reload = false;

    Euromillions() = default;
    //Euromillions(const year_month_day& ymd, const vector<int>& wnr, const vector<int>& star, const string& superstar, const vector< Wintable>& wintbl, bool jackpot_win)
    //	: ymd(ymd), wnr(wnr), star(star), superstar(superstar), wintbl(wintbl), jackpot_win(jackpot_win) {}

    void add_wnr(int inp) { wnr.push_back(inp); }
    void add_star(int inp) { star.push_back(inp); }
    void add_superstar(const string &inp) { superstar = inp; }
    void add_wintable(const Wintable &wintb) { wintbl.push_back(wintb); }
    void add_jw(bool jw) { jackpot_win = false; }
    void add_ymd(const year_month_day &ymdin) { ymd = ymdin; }
    static bool to_reload();
    static bool check_playday(const year_month_day &from);
    static void web_tgt(const year_month_day& from);
    //get position of date in datastore
    static int get_pd(const year_month_day& from);
};

struct Euromillions_datastore {
    //string_view trgt = "https://www.euro-millions.com/de-ch/gewinnzahlen/";
    year_month_day euromil_begin{ October / 8 / 2004 }; //--> first day swiss statistic available on source
    //year_month_day superstar{ June / 2 / 2015 };
    year_month_day superstar{ 2015y / June / 2d };
    vector<Euromillions> rld; //-->overkill, could be ymd only
    vector<Msg2> target2;
    vector<Msg> target;
    int pos = 0;
};
extern Euromillions_datastore ed;

class Swissloto {

};

class Eurodreams {

};