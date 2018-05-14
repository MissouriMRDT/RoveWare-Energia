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

#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "RoveCommProtocol.h"

uint16_t  SEQ_NUM_DEPRECATED = 0; // todo
uint8_t   FLAGS_DEPRECATED   = 0; // todo

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoveComm_packPacket(uint8_t* rovecomm_packet, uint16_t data_id, size_t data_size, const uint8_t* data) 
{  
  rovecomm_packet[0] = ROVECOMM_VERSION;
  rovecomm_packet[1] = SEQ_NUM_DEPRECATED >>      8; // todo
  rovecomm_packet[2] = SEQ_NUM_DEPRECATED &  0x00FF; // todo
  rovecomm_packet[3] = FLAGS_DEPRECATED;             // todo
  rovecomm_packet[4] = data_id            >>      8;
  rovecomm_packet[5] = data_id            &  0x00FF;
  rovecomm_packet[6] = data_size          >>      8;
  rovecomm_packet[7] = data_size          &  0x00FF;
  memcpy(&(rovecomm_packet[8]), data, data_size);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void RoveComm_unpackPacket(const uint8_t* rovecomm_packet, uint8_t* rovecomm_version, uint16_t* data_id, size_t* data_size, uint8_t* data) 
{   
  *rovecomm_version  = rovecomm_packet[0];  
  SEQ_NUM_DEPRECATED = rovecomm_packet[1];                              //todo
  SEQ_NUM_DEPRECATED = rovecomm_packet[2] | (SEQ_NUM_DEPRECATED << 8); //todo 
  FLAGS_DEPRECATED   = rovecomm_packet[3];                              //todo
  *data_id           = 0;  
  *data_id           = rovecomm_packet[4];
  *data_id           = rovecomm_packet[5] | (*data_id            << 8);
  *data_size         = 0; 
  *data_size         = rovecomm_packet[6];
  *data_size         = rovecomm_packet[7] | (*data_size          << 8);
  memcpy(data, &(rovecomm_packet[8]), *data_size);    
}

