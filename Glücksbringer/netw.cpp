//https://stackoverflow.com/questions/9750344/boostasio-winsock-and-winsock-2-compatibility-issue
#include <SDKDDKVer.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/io_context.hpp>


#include <openssl/ssl.h>

#include <boost/certify/extensions.hpp>
#include <boost/certify/https_verification.hpp>

#include <execution>
#include <mutex>
#include <ranges>

#include "netw.h"
#include "datetime.h"
#include "parse.h"
#include "game.h"
#include "log_msg.h"
#include "datastore.h"
#include "fhc.h"

using namespace std;

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;

#pragma comment(lib, "libcrypto.lib") //ssl
#pragma comment(lib, "libssl.lib")    //ssl  


#pragma comment(lib, "ws2_32.lib") //boost/certify needed
#pragma comment(lib, "gdi32.lib") //boost/certify needed
#pragma comment(lib, "advapi32.lib") //boost/certify needed
#pragma comment(lib, "crypt32.lib") //boost/certify needed
#pragma comment(lib, "user32.lib") //boost/certify needed



void web::web_euromil() {
	//error msg store
	boost::system::error_code ec;

	// The io_context is required for all I/O
	boost::asio::io_context ioc;


	string host = "euro-millions.com";
	//string port = "443";

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);

	//boost::certify certificate verif -->mostly unknown, need more time
	boost::certify::enable_native_https_server_verification(ctx);


	// resolver & query, need for connect
	boost::asio::ip::tcp::resolver resolver(ioc);

	if (ed.target.size() > 10) {

		vector< boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> v;
		for (auto x = 0; x < 10; x++) {
			v.push_back(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(ioc, ctx));
			if (!SSL_set_tlsext_host_name(v[x].native_handle(), host.c_str())) {
				Logerr::log_msg("SSL_set_tlsext_host_name faild");
				throw static_cast<int>(::ERR_get_error());
			}

		}
		ctx.set_default_verify_paths();
		ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
		ctx.set_verify_callback(boost::asio::ssl::host_name_verification(host));

		for (auto& x : v) {
			boost::asio::connect(x.lowest_layer(), resolver.resolve(host, "https"));
			x.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
			x.handshake(boost::asio::ssl::stream_base::client);
		}

		auto i = loop_prep(ed.target.size());
		auto l = 0;
		mutex mx;
		for_each(std::execution::par, v.begin(), v.end(), [&](boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& inp) {
			mx.lock(); auto ll = l; l += i.first; mx.unlock();
			for (auto& [tgt, msg, from] : ranges::subrange(ed.target.begin() + ll, ed.target.begin() + ll + i.first)) {
				http::request<http::string_body> req{ http::verb::get, tgt, 11 };
				req.set(http::field::host, host);
				req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
				http::write(inp, req);
				beast::flat_buffer buffer;
				http::response<http::dynamic_body> res;
				http::read(inp, buffer, res);
				mx.lock();
				msg = boost::beast::buffers_to_string(res.body().data());
				mx.unlock();
			}
		});
		i.first *= 10;
		for_each(std::execution::par, ed.target.begin()+i.first, ed.target.end(), [&](Msg& x) {
			mx.lock(); auto ll = i.second; i.second--; mx.unlock();
			http::request<http::string_body> req{ http::verb::get,x.tgt, 11 };
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			http::write(v[ll], req);
			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;
			http::read(v[ll], buffer, res);
			mx.lock();
			x.msg = boost::beast::buffers_to_string(res.body().data());
			mx.unlock();
		});

		for (auto& x : v) {
			//Initiat shudown procedure --> docs say stream.shutdown(ec) first --> not working, need 1 min --> shutdown lowest_layer first work
			x.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			x.shutdown(ec);
			x.lowest_layer().cancel(ec);
			x.lowest_layer().close();
		}
	} 
	else {
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> v(ioc,ctx);
		if (!SSL_set_tlsext_host_name(v.native_handle(), host.c_str())) {
			Logerr::log_msg("SSL_set_tlsext_host_name faild");
			throw static_cast<int>(::ERR_get_error());
		}
		ctx.set_default_verify_paths();
		ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
		ctx.set_verify_callback(boost::asio::ssl::host_name_verification(host));
		boost::asio::connect(v.lowest_layer(), resolver.resolve(host, "https"));
		v.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
		v.handshake(boost::asio::ssl::stream_base::client);
		for (auto& [tgt, msg, from] : ed.target) {
			http::request<http::string_body> req{ http::verb::get, tgt, 11 };
			req.set(http::field::host, host);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
			http::write(v, req);
			beast::flat_buffer buffer;
			http::response<http::dynamic_body> res;
			http::read(v, buffer, res);
			msg = boost::beast::buffers_to_string(res.body().data());
		}
		v.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		v.shutdown(ec);
		v.lowest_layer().cancel(ec);
		v.lowest_layer().close();
	}
		//Stop IOC --> needet???
		ioc.stop();

		//Second loop --> process parse
		for (const auto& [tgt, msg, from] : ed.target) {
			parse::htmlpars(msg, from);
		}

		//Cleanup tgt,msg store
		ed.target.clear();



}

void web::web_euromilnew() {
	//error msg store
	boost::system::error_code ec;

	// The io_context is required for all I/O
	boost::asio::io_context ioc;


	string host = "euro-millions.com";
	//string port = "443";

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);

	//boost::certify certificate verif -->mostly unknown, need more time
	boost::certify::enable_native_https_server_verification(ctx);


	// resolver & query, need for connect
	boost::asio::ip::tcp::resolver resolver(ioc);

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> v(ioc, ctx);
	if (!SSL_set_tlsext_host_name(v.native_handle(), host.c_str())) {
		Logerr::log_msg("SSL_set_tlsext_host_name faild");
		throw static_cast<int>(::ERR_get_error());
	}
	ctx.set_default_verify_paths();
	ctx.set_verify_mode(boost::asio::ssl::context::verify_peer);
	ctx.set_verify_callback(boost::asio::ssl::host_name_verification(host));
	boost::asio::connect(v.lowest_layer(), resolver.resolve(host, "https"));
	v.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
	v.handshake(boost::asio::ssl::stream_base::client);
	for (auto& [tgt, msg, from] : ed.target) {
		http::request<http::string_body> req{ http::verb::get, tgt, 11 };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		http::write(v, req);
		beast::flat_buffer buffer;
		http::response<http::dynamic_body> res;
		http::read(v, buffer, res);
		auto webreserr = res.result_int();
		if (webreserr == 200) {
			msg = boost::beast::buffers_to_string(res.body().data());
			//cout << "ok" << endl;
		}
		else {
			msg = ""; Logerr::log_web(to_string(webreserr), from);
			//cout << webreserr << endl;
			//--> wout dslist
			Fhc::dslistout(from);
		}
	}
	v.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	v.shutdown(ec);
	v.lowest_layer().cancel(ec);
	v.lowest_layer().close();

	//Stop IOC --> needet???
	ioc.stop();

	//Second loop --> process parse
	for (const auto& [tgt, msg, from] : ed.target) {
		if (msg.empty()) { continue; }
		parse::htmlpars(msg, from);
	}

	//Cleanup tgt,msg store
	ed.target.clear();
}

