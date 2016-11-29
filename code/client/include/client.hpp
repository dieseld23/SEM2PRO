#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class client
{
public:
	client(boost::asio::io_service& io_service,
		const char *host, const char *port);
	void write(std::string);
private:
	const char *host;
	const char *port;

	void do_connect(tcp::resolver::iterator endpoint_iterator);
	void do_write();
	void do_read();

	std::vector<std::string> rx_q;
	std::vector<std::string> tx_q;

	enum { max_length = 1024 };
	char data_[max_length];

	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::iterator iterator_;
};

#endif 