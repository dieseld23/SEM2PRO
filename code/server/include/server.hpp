#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class server
{
public:
    server(boost::asio::io_service& io_service, short port);
    void write(std::string);

private:
    void do_accept();
    void do_write();
    void do_read();

    std::vector<std::string> rx_q;
    std::vector<std::string> tx_q;

    enum { max_length = 1024 };
    char data_[max_length];

    tcp::socket socket_;
    tcp::acceptor acceptor_;
};

#endif