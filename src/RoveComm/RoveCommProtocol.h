 /*
  RoveCommProtocol.h - Library for Wiring + Processing IDE's (Energia/Arduino) 
  
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

#ifndef RoveCommProtocol_h
#define RoveCommProtocol_h

#include <stdint.h>
#include <stddef.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The RoveComm 1 Packet Header is 8 bytes long:
// uint8_t  rovecomm_version  
// uint16_t SEQ_NUM_DEPRECATED
// uint8_t  FLAGS_DEPRECATED  
// uint16_t data_id                     
// uint16_t data_size 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint8_t  ROVECOMM_VERSION             =  1;  
const uint8_t  ROVECOMM_VERSION_ERROR       = -1;
const size_t   ROVECOMM_PACKET_HEADER_SIZE  =  8;  

const uint16_t ROVECOMM_ETHERNET_UDP_PORT  = 11000;

const uint16_t ROVECOMM_ETHERNET_UDP_SUBSCRIBERS = 20;

const uint16_t ROVECOMM_SUBSCRIBE_REQUEST   = 3; 
const uint16_t ROVECOMM_UNSUBSCRIBE_REQUEST = 4;

void RoveComm_packPacket(uint8_t* rovecomm_packet, uint16_t  data_id, size_t  data_size, const uint8_t* data);
void RoveComm_unpackPacket(const uint8_t* rovecomm_packet, uint8_t* rovecomm_version, uint16_t* data_id, size_t* data_size, uint8_t* data);

#endif

