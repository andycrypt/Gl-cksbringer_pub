#include <assert.h>
#include <ranges>
#include <utility>
#include <intrin.h>
#include <execution>

#include "calc.h"
#include "datastore.h"
#include "log_msg.h"

using namespace std;

 void Random::gen_new_rseed() {
	 while (!_rdrand64_step(&_rdrand) && !rdnr_used(_rdrand)) { Logerr::log_msg("_rdrand64_step error"); }
	 while (!_rdseed64_step(&_rdseed) && !rdnr_used(_rdseed)) { Logerr::log_msg("_rdseed64_step error"); }
	Logerr::log_rdseed(_rdrand);
	Logerr::log_rdseed(_rdseed);
}

 Random::Random() {
	 while (!_rdrand64_step(&_rdrand) && !rdnr_used(_rdrand)) { Logerr::log_msg("_rdrand64_step error"); }
	 while(!_rdseed64_step(&_rdseed) && !rdnr_used(_rdseed)) { Logerr::log_msg("_rdseed64_step error"); }
	 rdrand_used = false;
	 Logerr::log_rdseed(_rdrand);
	 Logerr::log_rdseed(_rdseed);
 }

 unsigned __int64 Random::seed_feed() {
	 if (rdrand_used) { auto x = _rdseed; gen_new_rseed(); rdrand_used = false; return x; }
	 rdrand_used = true;
	 return _rdrand;
 }

 unsigned __int64 Random::seed_feed_experimental() {
	 gen.seed(seed_feed());
	 return base_rand(gen);
 }

 bool Random::rdnr_used(unsigned __int64 nr) {
	 for (const auto& x : d->rdstr) {
		 if (x == nr) { return true; }
	 }
	 return false;
 }

 void Random::casheurm(vector<int> base, vector<int> basestar, int c) {
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<> rng(1, 50);
	uniform_int_distribution<> star(1, 12);
	double los = 3.5;
	double total = 0.0;
	double totalwin = 0.0;


	for (auto x = 0; x < c; x++) {

		vector<int> winnr;
		vector<int> starnr;
		int winnrm = 0;
		int starm = 0;

		for (auto x = 0; x < 5; x++) {
			auto a = rng(gen);
			while (auto it = ranges::find(winnr, a) != winnr.end()) {
				a = rng(gen);
			}
			winnr.push_back(a);
		}

		for (auto x = 0; x < 2; x++) {
			auto a = star(gen);
			while (auto it = ranges::find(starnr, a) != starnr.end()) {
				a = star(gen);
			}
			starnr.push_back(a);
		}

		sort(winnr.begin(), winnr.end(), less<int>());
		sort(starnr.begin(), starnr.end(), less<int>());
		for (auto x : winnr) {
			cout << x << " ";
		}

		for (auto x : starnr) {
			cout << x << " ";
		}
		//cout << endl;
		total += los;
		for (auto x : winnr) {
			if (auto it = ranges::find(base.begin(), base.end(), x) != base.end()) { winnrm++; }
		}
		if (winnrm >= 1) {
			for (auto x : starnr) {
				if (auto it = ranges::find(basestar.begin(), basestar.end(), x) != basestar.end()) { starm++; }
			}
		}
		if (winnrm == 1 && starm == 2 || winnrm >= 2) {
			cout << "---" << winnrm << " " << starm << "  " << totalwin << endl;
			switch (winnrm) {
			case 1:
				totalwin += 10.8;
				break;
			case 2:

				if (starm == 1) { totalwin += 10.35; break; }
				if (starm == 2) { totalwin += 21.75; break; }
				totalwin += 7.85;
				break;
			case 3:

				if (starm == 1) { totalwin += 19.45; break; }
				if (starm == 2) { totalwin += 87.05; break; }
				totalwin += 17.60;
				break;
			case 4:

				if (starm == 1) { totalwin += 179.50; break; }
				if (starm == 2) { totalwin += 1627.80; break; }
				totalwin += 69.10;
				break;
			case 5:

				if (starm == 1) { totalwin += 146209.65; break; }
				if (starm == 2) { totalwin += 27553486.25; break; }
				totalwin += 13668.65;
				break;
			}
		}
		cout << endl;
	}
	cout << "Spiele " << c << " Tipps, Gesamtwert: " << total << " Gewinn ist: " << totalwin << endl;
}

 void Random::casheurm2(vector<int> base, vector<int> basestar, int c) {
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<> rng(1, 50);
	uniform_int_distribution<> star(1, 12);
	double los = 3.5;
	double global = 0.0;
	double globalwin = 0.0;
	int lcnt = 0;
	while (true) {
		double totalwin = 0.0;
		double total = 0.0;
		for (auto x = 0; x < c; x++) {

			vector<int> winnr;
			vector<int> starnr;
			int winnrm = 0;
			int starm = 0;

			for (auto x = 0; x < 5; x++) {
				auto a = rng(gen);
				while (auto it = ranges::find(winnr.begin(), winnr.end(), a) != winnr.end()) {
					a = rng(gen);
				}
				winnr.push_back(a);
			}

			for (auto x = 0; x < 2; x++) {
				auto a = star(gen);
				while (auto it = ranges::find(starnr.begin(), starnr.end(), a) != starnr.end()) {
					a = star(gen);
				}
				starnr.push_back(a);
			}

			sort(winnr.begin(), winnr.end(), less<int>());
			sort(starnr.begin(), starnr.end(), less<int>());
			total += los;
			for (auto x : winnr) {
				if (auto it = ranges::find(base.begin(), base.end(), x) != base.end()) { winnrm++; }
			}
			if (winnrm >= 1) {
				for (auto x : starnr) {
					if (auto it = ranges::find(basestar.begin(), basestar.end(), x) != basestar.end()) { starm++; }
				}
			}
			if (winnrm == 1 && starm == 2 || winnrm >= 2) {
				switch (winnrm) {
				case 1:
					totalwin += 10.8;
					break;
				case 2:

					if (starm == 1) { totalwin += 10.35; break; }
					if (starm == 2) { totalwin += 21.75; break; }
					totalwin += 7.85;
					break;
				case 3:

					if (starm == 1) { totalwin += 19.45; break; }
					if (starm == 2) { totalwin += 87.05; break; }
					totalwin += 17.60;
					break;
				case 4:

					if (starm == 1) { totalwin += 179.50; break; }
					if (starm == 2) { totalwin += 1627.80; break; }
					totalwin += 69.10;
					break;
				case 5:

					if (starm == 1) { totalwin += 146209.65; break; }
					if (starm == 2) { totalwin += 27553486.25; break; }
					totalwin += 13668.65;
					break;
				}
			}
		}
		if (total > totalwin) {
			lcnt++;
			globalwin += totalwin;
			global += total;
		}
		else {
			cout << "Spiele " << c << " Tipps, " << lcnt << " mal, Gesamtwert: " << format("{}", global) << " Gewinn total ist: " << format("{}", globalwin) << endl;
			break;
		}
	}

}



 void Random::eurm_mod() {
	gen.seed(seed_feed());
	uniform_int_distribution<> rng(1, 50);
	uniform_int_distribution<> star(1, 12);

	while (true) {
		bool abc = false;
		vector<int> winnr;
		vector<int> starnr;

		for (auto x = 0; x < 5; x++) {
			auto a = rng(gen);
			while (auto it = ranges::find(winnr, a) != winnr.end()) {
				a = rng(gen);
			}
			winnr.push_back(a);
		}

		for (auto x = 0; x < 2; x++) {
			auto a = star(gen);
			while (auto it = ranges::find(starnr, a) != starnr.end()) {
				a = star(gen);
			}
			starnr.push_back(a);
		}

		ranges::sort(winnr, less<int>());
		ranges::sort(starnr, less<int>());

		for (const auto& x : d->eurmil) {
			if (x.wnr == winnr) {
				abc = true;
				break;
			}
		}
		if (abc) { continue; }
		cout << "Euromil: ";
		for (auto x : winnr) {
			cout << x << " ";
		}
		cout << "star: ";
		for (auto x : starnr) {
			cout << x << " ";
		}
		cout << endl;
		break;

	}
}



 void Random::swiss() {
	//random_device rd;
	//mt19937_64 gen(gen_seed());
	gen.seed(seed_feed());
	uniform_int_distribution<> rng(1, 42);
	uniform_int_distribution<> star(1, 6);

	vector<int> winnr;
	int starnr = star(gen);

	for (auto x = 0; x < 6; x++) {
		auto a = rng(gen);
		while (auto it = ranges::find(winnr.begin(), winnr.end(), a) != winnr.end()) {
			a = rng(gen);
		}
		winnr.push_back(a);
	}
	sort(winnr.begin(), winnr.end(), less<int>());
	cout << "Swiss: ";
	for (auto x : winnr) {
		cout << x << " ";
	}
	cout << "star: " << starnr << endl;
}

 void Random::eurodream() {
	random_device rd;
	mt19937_64 gen(rd());
	uniform_int_distribution<> rng(1, 40);
	uniform_int_distribution<> star(1, 5);

	vector<int> winnr;
	int starnr = star(gen);

	for (auto x = 0; x < 6; x++) {
		auto a = rng(gen);
		while (auto it = ranges::find(winnr.begin(), winnr.end(), a) != winnr.end()) {
			a = rng(gen);
		}
		winnr.push_back(a);
	}
	sort(winnr.begin(), winnr.end(), less<int>());
	for (auto x : winnr) {
		cout << x << " ";
	}
	cout << "star: " << starnr << endl;
}

 void Random::predic() const {
	vector<double>error, x, y;
	y.push_back(0);
	for (const auto& [idx, num] : ranges::views::enumerate(d->eurmil)) {
		auto a = num.wnr[0];
		if (fmod(idx, 2) == 0) {
			x.push_back(a); continue;
		}
		y.push_back(a);
	}
	double err;
	double b0 = 0;                   //initializing b0
	double b1 = 0;                   //initializing b1
	double alpha = 0.01;             //intializing error rate
	cout << y.size() << " " << x.size() << endl;

	assert(y.size() == x.size());
	/*Training Phase*/
	for (int i = 0; i < y.size() * 5; i++) {   // since there are 5 values and we want 4 epochs so run for loop for 20 times
		int idx = i % y.size();              //for accessing index after every epoch
		double p = b0 + b1 * x[idx];  //calculating prediction
		err = p - y[idx];              // calculating error
		b0 = b0 - alpha * err;         // updating b0
		b1 = b1 - alpha * err * x[idx];// updating b1
		cout << "B0=" << b0 << " " << "B1=" << b1 << " " << "error=" << err << endl;// printing values after every updation
		error.push_back(err);
	}
	sort(std::execution::par, error.begin(), error.end(), custom_sort);//sorting based on error values
	cout << "Final Values are: " << "B0=" << b0 << " " << "B1=" << b1 << " " << "error=" << error[0] << endl;

	/*Testing Phase*/
	cout << "Enter a test x value";
	double test = 6;
	//cin >> test;
	double pred = b0 + b1 * test;
	cout << endl;
	cout << "The value predicted by the model= " << pred;
}
