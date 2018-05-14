 /*
  RoveComm.h - Library for Wiring + Processing IDE's (Energia/Arduino) 
  
  Copyright (c) 2018 Missouri S&T Mars Rover Design Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef RoveComm_h
#define RoveComm_h

#include <stdint.h>
#include <stddef.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RoveCommEthernetUdp
{
public:
  void begin(uint8_t ip_octet_1, uint8_t ip_octet_2, uint8_t ip_octet_3, uint8_t ip_octet_4);  
  
  void write(uint16_t  data_id, size_t  data_size,  const void* data);
  void read( uint16_t* data_id, size_t* data_size,        void* data);

  void writeTo( uint16_t data_id, size_t data_size, const void* data, uint8_t  ip_octet_1, uint8_t  ip_octet_2, uint8_t  ip_octet_3, uint8_t  ip_octet_4, uint16_t port);
  void readFrom(uint16_t data_id, size_t data_size, const void* data, uint8_t* ip_octet_1, uint8_t* ip_octet_2, uint8_t* ip_octet_3, uint8_t* ip_octet_4);
};

#endif
