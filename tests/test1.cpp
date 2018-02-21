// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <amt2018/communication.hpp>

#if __has_include(<variant>)
#include <variant>
#else
#include <experimental/variant>
#endif

#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/asio.hpp>

// namespace beast {

// std::ostream& operator<<(std::ostream& os, beast::multi_buffer const& mb)
// {
//   auto buffers = mb.data();
//   for(auto&& buffer : buffers)
//   {
//     const char* data = boost::asio::buffer_cast<const char*>(buffer);
//     std::copy(data, data + boost::asio::buffer_size(buffer), std::ostream_iterator<char>(os));
//   }
//   return os;
// }
  
// }



int main()
{
  boost::asio::io_service io_service;

  boost::asio::deadline_timer timer(io_service);

  amt2018::connection connection
    (io_service, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), 9009});

  connection.listen();
  amt2018::communication communication(connection);

  auto password = "909000"; // default password
  
  communication.zone_state.connect
    ([&communication] (auto open, auto bypass)
     {
       std::cout << "porta 18 esta " << (open[18-1] ? " aberta " : " fechada ") << std::endl;

       if(!open[17])
       {
         std::cout << "activating partition 1" << std::endl;
         communication.activate_partition(1, password);
       }
     });

  connection.data.connect
    ([&] (auto const& ec, auto buffer)
     {
       static bool never = true;
       if(ec)
       {
         connection.listen();
       }
       else if(never/* && buffer.size() == 7 && (unsigned char)buffer[0] == 0xc4*/)
       {
         never = false;
         std::cout << "request zone" << std::endl;
         communication.request_zones(password);
       }
     });
  
  
  {
    // boost::asio::ip::tcp::socket socket(io_service);
    // boost::asio::ip::tcp::no_delay no_delay(true);
    // acceptor.accept(socket);
    // socket.set_option(no_delay);

    io_service.run();


    // boost::system::error_code ignored_error;
    // int size = 0;

    // const unsigned char login[] = {0x0b, 0xe9, 0x21, /* senha */ 0x35, 0x38, 0x31, 0x30, 0x30, 0x30, /* fim da senha */ 0x41, 0x41, 0x21, 0x11};
    // //const unsigned char msg[] = {0x0b, 0xe9, 0x21, /* senha */ 0x35, 0x38, 0x31, 0x30, 0x30, 0x30, /* fim da senha */ 0x41, 0x41, 0x21, 0x11, 0x0a, 0xe9, 0x21, /* senha */ 0x35, 0x38, 0x31, 0x30, 0x30, 0x30, /* fim da senha */ 0x5a, 0x21, 0x4a};
    // const unsigned char activate_or_deactivate[] = {0x0a, 0xe9, 0x21, /* senha */ 0x35, 0x38, 0x31, 0x30, 0x30, 0x30, /* fim da senha */ 0x5a, 0x21, 0x4a};

    // // size = boost::asio::write(socket, boost::asio::buffer(login), ignored_error);

    // // std::cout << "sent login " << size << std::endl;

    // std::vector<char> buffer(4096);

    // ///
    
    // size = boost::asio::read(socket, boost::asio::buffer(buffer), ignored_error);

    // buffer.resize(size);

    // std::cout << "received " << size << std::endl;

    // std::cout << std::hex << std::setfill('0') << std::setw(2);
    // for(auto& c : buffer)
    // {
    //   std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ' ';
    // }
    // std::cout << std::dec << std::setw(1) << std::endl;

    // size -= 4;
    
    // const unsigned char ack[] = {0xfe};
    // while((size -= 9) > 0)
    // {
    //   boost::asio::write(socket, boost::asio::buffer(ack), ignored_error);
    //   std::cout << "sent ack " << size << std::endl;
    // }

    // size = boost::asio::write(socket, boost::asio::buffer(activate_or_deactivate), ignored_error);

    // std::cout << "sent activate or deactivate " << size << std::endl;
    
    // // std::cout << "sent " << size << std::endl;

    // buffer.resize(4096);

    // do {
    // size = boost::asio::read(socket, boost::asio::buffer(buffer), ignored_error);
    // } while(!size);
    // buffer.resize(size);

    // std::cout << "received " << size << std::endl;

    // std::cout << std::hex << std::setfill('0') << std::setw(2);
    // for(auto& c : buffer)
    // {
    //   std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ' ';
    // }
    // std::cout << std::dec << std::setw(1) << std::endl;
  }

  return -1;
  //io_service.run();
}

