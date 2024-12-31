// Glücksbringer.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "Glücksbringer.h"

#include <chrono>
#include "netw.h"


using namespace std;
using namespace chrono;


void Glücksbringer::start() {
    year_month_day to = datetime::ymdret_today();
    if (d->eurmil.size() > 0) {
        year_month_day from = d->eurmil.back().ymd;
        if (Euromillions::to_reload()) { //modify vector & trunc file
            for (const auto& x : ed.rld) {
                Euromillions::web_tgt(x.ymd);
            }
            ed.rld.clear();
            web::web_euromil();
            parse::jsonfparse(true);
            Fhc::rm(d->wrklst);
            parse::wout_euromil(true, 0);
        }
        if (to > from) { // push back & app file
            auto cnt = d->eurmil.size();
            datetime::clk(datetime::ymdret_addday(from, 1), datetime::ymdret_addday(to, -1));
            parse::jsonfparse(false);
            Fhc::rm(d->wrklst);
            if (cnt != d->eurmil.size()) {
                parse::wout_euromil(false, cnt);
            }
        }
    }
    else {
        datetime::clk(ed.superstar, datetime::ymdret_addday(to, -1));
        parse::jsonfparse(false);
        Fhc::rm(d->wrklst);
        parse::wout_euromil(false, 0);
    }
}



int main() {
   
    Glücksbringer g;
   
    
    g.optimize_play_eurmil(1d / 1 / 2023, 31d / 12 / 2023);
    g.eurm_mod();
    g.eurm_mod();
    g.eurm_mod();
    
}
