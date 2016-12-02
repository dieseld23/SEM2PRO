#include "server.hpp"

server::server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service)
{
    do_accept();
}

void server::write(std::string msg)
{
    tx_q.push_back(msg);
    do_write();
}

void server::do_accept()
{
    acceptor_.async_accept(socket_,[this](boost::system::error_code ec)
    {
        if (!ec)
        {
            do_read();
        }
    });
}

void server::do_read()
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

void server::do_write()
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