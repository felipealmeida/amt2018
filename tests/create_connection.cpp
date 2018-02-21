// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <amt2018/connection.hpp>

int main()
{
  boost::asio::io_service service;
  
  amt2018::connection connection
    (service, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), 9009});
}
