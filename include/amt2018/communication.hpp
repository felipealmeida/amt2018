// Copyright Felipe Magno de Almeida 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef AMT2018_COMMUNICATION_HPP
#define AMT2018_COMMUNICATION_HPP

#include <amt2018/connection.hpp>
#include <amt2018/events.hpp>

#include <boost/dynamic_bitset.hpp>

#include <iostream>
#include <iomanip>

namespace amt2018 {

struct communication
{
  communication(connection& conn)
    : connection_(&conn)
  {
    signal_connection = connection_->data.connect
      (std::bind(&communication::data, this, std::placeholders::_1, std::placeholders::_2));
  }

  void send_ack()
  {
    std::cout << "sending ack" << std::endl;
    const unsigned char fe[1] = {0xfe};
    boost::asio::write(connection_->socket, boost::asio::buffer(fe));
  }

  void set_bypass()
  {
    
  }

  void activate_partition(int partition, std::string_view password)
  {
    unsigned char buffer[] = {0x0b, 0xe9, 0x21, /* senha */ 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, /* fim da senha */ 0x41, 0x41, 0x21, 0x00};
    if(password.size() <= 4)
    {
      std::reverse_iterator iterator(&buffer[7]);
      std::copy(password.rbegin(), password.rend(), iterator);
    }
    else if(password.size() <= 6)
    {
      std::reverse_iterator iterator(&buffer[9]);
      std::copy(password.rbegin(), password.rend(), iterator);
    }
    else
    {
      throw std::logic_error("Password must not be bigger than 6 digits");
    }
    
    buffer[12] = 0x11;

    std::cout << "sending: " << std::hex << std::setfill('0') << std::setw(2);
    for(auto& c : buffer)
    {
      std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ' ';
    }
    std::cout << std::dec << std::setw(1) << std::endl;
    boost::asio::write(connection_->socket, boost::asio::buffer(buffer));
    // answer is e9 fe
  }
  
  void request_zones(std::string_view password)
  {
    // 0a:e9:21:35:38:31:30:30:30:5a:21:4a
    unsigned char buffer[12] = {0x0a, 0xe9, 0x21, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a, 0x21, 0x00};
    if(password.size() <= 4)
    {
      std::reverse_iterator iterator(&buffer[7]);
      std::copy(password.rbegin(), password.rend(), iterator);
    }
    else if(password.size() <= 6)
    {
      std::reverse_iterator iterator(&buffer[9]);
      std::copy(password.rbegin(), password.rend(), iterator);
    }
    else
    {
      throw std::logic_error("Password must not be bigger than 6 digits");
    }
    // crc
    buffer[11] = 0x4a;

    std::cout << "sending: " << std::hex << std::setfill('0') << std::setw(2);
    for(auto& c : buffer)
    {
      std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ' ';
    }
      std::cout << std::dec << std::setw(1) << std::endl;
    boost::asio::write(connection_->socket, boost::asio::buffer(buffer));
  }

  void data(boost::system::error_code const& ec, std::string_view buffer)
  {
    if(!ec)
    {
      std::cout << "no error" << std::endl;
      std::cout << "received data:" << std::endl;
      std::cout << std::hex << std::setfill('0') << std::setw(2);
      for(auto& c : buffer)
      {
        std::cout << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ' ';
      }
      std::cout << std::dec << std::setw(1) << std::endl;
      if(buffer.size())
      {
        std::cout << "buffer not empty" << std::endl;
        if(buffer.size() == 1 && (unsigned char)buffer[0] == 0xf7)
        {
          send_ack();
        }
        if((unsigned char)buffer[0] == 0x94)
        {
          // 07:94:45:67:86:29:fe:92:8d
          send_ack();
          // const unsigned char something[3] = {0x01, 0xc4, 0x3a};
          // boost::asio::write(connection_->socket, boost::asio::buffer(something));
        }
        else if((unsigned char)buffer[0] == 0xc4)
        {
          send_ack();
        }
        else if((unsigned char)buffer[0] == 0xb0
                && buffer.size() == 17
                && (unsigned char)buffer[1] == 0x12)
        {
          std::cout << "event" << std::endl;
          auto unescape_zero = [] (unsigned char i)
            {
              return i == 0xa ? 0 : i;
            };
          //11:b0:12:06:07:08:06:01:08:03:04:0a:01:0a:01:0a:0a:0a:47
          int client_id = unescape_zero(buffer[2])*1000
            + unescape_zero(buffer[3])*100
            + unescape_zero(buffer[4])*10
            + unescape_zero(buffer[5]);
          int ev_id = unescape_zero(buffer[8])*1000
            + unescape_zero(buffer[9])*100
            + unescape_zero(buffer[10])*10
            + unescape_zero(buffer[11]);
          int partition = unescape_zero(buffer[12])*10
            + unescape_zero(buffer[13]);
          int zone = unescape_zero(buffer[14])*100
            + unescape_zero(buffer[15])*10
            + unescape_zero(buffer[16]);

          std::cout << "event is " << message_event(static_cast<enum event>(ev_id))
                    << " client_id " << client_id << " partition " << partition
                    << " zone " << zone << std::endl;
          send_ack();
          event(static_cast<enum event>(ev_id), client_id, partition, zone);
        }
        else if((unsigned char)buffer[0] == 0xe9
                && buffer.size() == 2
                && (unsigned char)buffer[1] == 0xfe)
        {
          send_ack();
        }
        else if((unsigned char)buffer[0] == 0xe9
                && buffer.size() >= 3*6 /* open, something, bypass */ + 1 /* 0xe9 command */ + 1/*crc*/)
        {
          // std::cout << "received zone information" << std::endl;
          
          boost::dynamic_bitset<> open(6*8), bypass(6*8);

          auto populate_bitset =
            [] (auto& bitset, const char* buffer)
          {
            for(int char_offset = 0; char_offset != 6; ++char_offset)
            {
              unsigned char c = buffer[char_offset];
              // std::cout << "char offset " << char_offset << " value is " << (int)c << std::endl;
              for(int bit = 0; bit != 8; ++bit)
              {
                // std::cout << "index " << (char_offset*8 + bit) << " is " << (bool)((c >> bit) & 1) << std::endl;
                bitset[char_offset*8 + bit] = (c >> bit) & 1;
              }
            }
          };

          populate_bitset(open, &buffer[1]);
          populate_bitset(bypass, &buffer[1+6+6]);

          zone_state(open, bypass);
        }
      }
    }
  }

  boost::signals2::signal<void(enum event, int client_id, int part, int zone)> event;
  boost::signals2::signal<void(boost::dynamic_bitset<> open, boost::dynamic_bitset<> bypass)> zone_state;
  
  connection* connection_;
  boost::signals2::connection signal_connection;
};
  
}

#endif
