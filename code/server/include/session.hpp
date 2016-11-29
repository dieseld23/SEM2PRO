#ifndef SESSION_HPP
#define SESSION_HPP

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session : public std::enable_shared_from_this<session>
{
public:
  	session(tcp::socket socket) : socket_(std::move(socket)){};

  	void start(){ do_read(); };

private:
  	void do_read();
  	void do_write(std::size_t length);

  	tcp::socket socket_;
  	enum { max_length = 1024 };
	char data_[max_length];
};

#endif