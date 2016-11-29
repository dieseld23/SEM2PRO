#include "client.hpp"

client::client(boost::asio::io_service& io_service, 
			const char *host, const char *port) 
	: resolver_(io_service), socket_(io_service)
{
	iterator_ = resolver_.resolve(
		boost::asio::ip::tcp::resolver::query(host, port));

	do_connect(iterator_);
}

void client::write(std::string msg)
{
	tx_q.push_back(msg);
	do_write();
}

void client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
	boost::asio::async_connect(socket_, endpoint_iterator,
	    [this](boost::system::error_code ec, tcp::resolver::iterator)
    {
		if (!ec)
		{
			do_read();
		}
    });
}

void client::do_write()
{
	boost::asio::async_write(socket_,
	boost::asio::buffer(tx_q.back(), tx_q.back().length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
	{
		tx_q.pop_back();
		if (!ec)
		{
			if (!tx_q.empty())
			{
				do_write();
			}
		}
		else
		{
			socket_.close();
		}
	});
}

void client::do_read()
{
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this](boost::system::error_code ec, std::size_t length)
	{
  		if (!ec)
  		{
  			std::cout << "RX: " << data_ << std::endl;
    		do_read();
  		}
	});
}