// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef AMT2018_CONNECTION_HPP
#define AMT2018_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include <iostream>
#include <iomanip>

namespace amt2018 {

struct connection
{
  connection(boost::asio::io_service& service
             , boost::asio::ip::tcp::endpoint endpoint)
    : buffer_start(0), acceptor(service, endpoint), socket(service)
  {
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    
  }

  void listen()
  {
    buffer_start = 0;
    if(socket.is_open())
    {
      socket.close();
    }
    std::cout << "accepting" << std::endl;
    acceptor.async_accept(socket, std::bind(&connection::handle_accept, this, std::placeholders::_1));
  }

  void handle_read(boost::system::error_code const& ec, unsigned int size)
  {
    if(!ec)
    {
      std::cout << "received tcp size " << size << " buffer_start " << buffer_start << std::endl;
      std::cout << std::hex << std::setfill('0') << std::setw(2);
      for(auto first = &read_buffer[buffer_start], last = &read_buffer[buffer_start] + size
            ; first != last; ++first)
      {
        std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)*first << ' ';
      }
      std::cout << std::dec << std::setw(1) << std::endl;
      
      if(size)
      {
        int start = 0;
        auto is_nope = read_buffer[start] == 0xf7;
        unsigned int packet_size = is_nope ? 1 : read_buffer[start] ;
        while(is_nope || start + packet_size + 1 < buffer_start + size)
        {
          std::cout << "start " << start << std::endl;
          std::cout << "packet size " << packet_size << std::endl;
          std::string_view packet{static_cast<const char*>
              (static_cast<const void*>(&read_buffer[start+!is_nope])), packet_size};
          // should check crc8
          data(ec, packet);
          start += packet_size+!is_nope+!is_nope;
          is_nope = start != buffer_start + size ? read_buffer[start] == 0xf7 : 0;
          packet_size = is_nope ? 1 : read_buffer[start] ;
        }
        // remove data already read
        std::copy(std::next(read_buffer.begin(), start)
                  , std::next(read_buffer.begin(), buffer_start+size)
                  , read_buffer.begin());
        buffer_start = buffer_start + size - start;

        // read from where stopped
        socket.async_receive(boost::asio::mutable_buffers_1(&read_buffer[buffer_start]
                                                            , read_buffer.size() - buffer_start)
                             , std::bind(&connection::handle_read, this, std::placeholders::_1
                                         , std::placeholders::_2));
      }
    }
    else
    {
      std::cout << "handle_read error " << ec.message() << std::endl;
      data(ec, {});
    }
  }
  
  void handle_accept(boost::system::error_code const& ec)
  {
    if(!ec)
    {
      socket.set_option(boost::asio::ip::tcp::no_delay{true});
      std::cout << "connected" << std::endl;
      socket.async_receive(boost::asio::buffer(read_buffer) // from zero
                           , std::bind(&connection::handle_read, this, std::placeholders::_1
                                       , std::placeholders::_2));
    }
    else
    {
      std::cout << "handle_accept error " << ec.message() << std::endl;
      data(ec, {});
    }
  }

  boost::signals2::signal<void(boost::system::error_code const&, std::string_view)> data;

  unsigned int buffer_start;
  std::array<unsigned char, 4096> read_buffer;

  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::ip::tcp::socket socket;  
};
  
}

#endif
