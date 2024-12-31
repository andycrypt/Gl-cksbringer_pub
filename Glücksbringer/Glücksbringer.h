#pragma once

//#include "netw.h"
#include "fhc.h"
#include "calc.h"
//#include "conv.h" --> Fhc
//#include "game.h"
#include "datastore.h"
#include "datetime.h"
#include "parse.h"
#include "log_msg.h"

class Glücksbringer : public Fhc, public datetime, public Random {
public:
	Glücksbringer() { Logerr::log_msg("Startup"); parse::imp_euromil(); parse::imp_rdnr(), start();} 
	~Glücksbringer() { delete d; }
	static void start();

};


