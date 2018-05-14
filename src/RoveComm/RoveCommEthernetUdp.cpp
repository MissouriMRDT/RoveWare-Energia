 /*
  RoveCommEthernetUdp.h - Library for Wiring + Processing IDE's (Energia/Arduino) 
  
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

#include "RoveComm.h"
#include "RoveCommProtocol.h"

////////////////////////////////////////////////////////////////////////////
// If => Texas Instruments Ethernet (Tiva C 129)
//
// Board Manager installs to => C:\Users\<YourUser>\AppData\Local\Energia15
//
// #include <Energia.h>
// #include <SPI.h>
// #include <Ethernet.h>    
// #include <EthernetUdp.h>

////////////////////////////////////////////////////////////////////////////
// Else => Arduino Ethernet (Wiznet)
//
// Board Manager installs to =>  C:\Users\<YourUser>\AppData\Local\Arduino15
//
// #include <Arduino.h>
// #include <SPI.h>
// #include <Ethernet.h>    
// #include <EthernetUdp.h>

#ifdef Energia_h
  #include <Energia.h>
#else
  #include <Arduino.h>
#endif 
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h> // EthernetClass Ethernet; => instance already in Ethernet.cpp

EthernetUDP EthernetUdp; 

const IPAddress NULL_IP(0, 0, 0, 0);

IPAddress RoveCommEthernetUdpSubscribers[ROVECOMM_ETHERNET_UDP_SUBSCRIBERS];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void RoveCommEthernetUdp::begin(uint8_t ip_octet_1, uint8_t ip_octet_2, uint8_t ip_octet_3, uint8_t ip_octet_4) 
{ 
  int  i;
  for (i=0; i < ROVECOMM_ETHERNET_UDP_SUBSCRIBERS; i++) 
  {
    RoveCommEthernetUdpSubscribers[i] = NULL_IP;
  }
  
  IPAddress LocalIp(ip_octet_1, ip_octet_2, ip_octet_3, ip_octet_4);

 // Ethernet.enableActivityLed();
 // Ethernet.enableLinkLed(); 
  
  Ethernet.begin(0, LocalIp);    
  EthernetUdp.begin(ROVECOMM_ETHERNET_UDP_PORT);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void RoveCommEthernetUdp::write(uint16_t data_id, size_t data_size, const void* data) 
{ 
  uint8_t rovecomm_packet[ROVECOMM_PACKET_HEADER_SIZE + data_size]; 
  
  RoveComm_packPacket(rovecomm_packet, data_id, data_size, (uint8_t*)data);
  
  int  i;
  for (i=0; i < ROVECOMM_ETHERNET_UDP_SUBSCRIBERS; i++) 
  {
    if (!(RoveCommEthernetUdpSubscribers[i] == NULL_IP)) 
    {       
      EthernetUdp.beginPacket(RoveCommEthernetUdpSubscribers[i], ROVECOMM_ETHERNET_UDP_PORT);
      EthernetUdp.write(rovecomm_packet, ROVECOMM_PACKET_HEADER_SIZE + data_size);
      EthernetUdp.endPacket();
    }
  } 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
void RoveCommEthernetUdp::writeTo(uint16_t data_id, size_t  data_size, const void* data, uint8_t ip_octet_1, uint8_t ip_octet_2, uint8_t ip_octet_3, uint8_t ip_octet_4, uint16_t port) 
{ 
  uint8_t rovecomm_packet[ROVECOMM_PACKET_HEADER_SIZE + data_size];
  
  RoveComm_packPacket(rovecomm_packet, data_id, data_size, (uint8_t*)data);

  IPAddress RemoteIp(ip_octet_1, ip_octet_2, ip_octet_3, ip_octet_4); 
  
  EthernetUdp.beginPacket(RemoteIp, port);
  EthernetUdp.write(rovecomm_packet, ROVECOMM_PACKET_HEADER_SIZE + data_size);
  EthernetUdp.endPacket(); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RoveCommEthernetUdp::read(uint16_t* data_id, size_t* data_size, void* data) 
{ 
  *data_id   = 0; 
  *data_size = 0; 
   
  int packet_size = EthernetUdp.parsePacket();  
  if (packet_size > 0)
  {       
    uint8_t rovecomm_packet[packet_size];    
    EthernetUdp.read(rovecomm_packet, packet_size);     
    IPAddress RemoteIp = EthernetUdp.remoteIP();
    
    uint8_t rovecomm_version; 
    
    RoveComm_unpackPacket(rovecomm_packet, &rovecomm_version, data_id, data_size, (uint8_t*)data); 
    
    if (rovecomm_version != 1) 
    {
      *data_id   = ROVECOMM_VERSION_ERROR;
      *data_size = 0; 
      
    } else if  (*data_id == ROVECOMM_SUBSCRIBE_REQUEST)
    {
      int  i;
      for (i=0; i < ROVECOMM_ETHERNET_UDP_SUBSCRIBERS; i++) 
      {
        if (RoveCommEthernetUdpSubscribers[i] == RemoteIp) 
        {
          break; // already subscribed         
        } else if (RoveCommEthernetUdpSubscribers[i] == NULL_IP) 
        {
          RoveCommEthernetUdpSubscribers[i] = RemoteIp; // add subscriber
          break;
        }
      }
        
    } else if (*data_id == ROVECOMM_UNSUBSCRIBE_REQUEST) 
    {
      int  i;
      for (i=0; i < ROVECOMM_ETHERNET_UDP_SUBSCRIBERS; i++) 
      {
        if (RoveCommEthernetUdpSubscribers[i] == RemoteIp) 
        {
          RoveCommEthernetUdpSubscribers[i] = NULL_IP; // remove subscriber
          break;        
        }
      }
    }  
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// todo   
// todo 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*
void RoveCommEthernetUdp::readFrom(uint16_t  data_id, size_t data_size, const void* data, uint8_t* ip_octet_1, uint8_t* ip_octet_2, uint8_t* ip_octet_3*, uint8_t* ip_octet_4) 
{   
  //todo uint8_t* ip_octet_1, uint8_t* ip_octet_2, uint8_t* ip_octet_3*, uint8_t* ip_octet_4
  *data_id   = 0; 
  *data_size = 0; 
   
  int packet_size = EthernetUdp.parsePacket();  
  if (packet_size > 0)
  {       
    char rovecomm_packet[packet_size];    
    EthernetUdp.read(rovecomm_packet, rovecomm_packet);     
    IPAddress RemoteIp = EthernetUdp.remoteIP();
    
    uint8_t rovecomm_version; 
    
    RoveComm_unpackPacket(rovecomm_packet, &rovecomm_version, data_id, data_size, (char*)data);  
    
    if (rovecomm_version != 1) 
    {
      *data_id   = ROVECOMM_VERSION_ERROR;
      *data_size = 0; 
      
    } else if  (data_id == ROVECOMM_SUBSCRIBE_REQUEST)
    {
      int  i;
      for (i=0; i < ROVECOMM_ETHERNET_UDP_SUBSCRIBERS; i++) 
      {
        if (EthernetUdpSubscribers[i] == RemoteIp) 
        {
          break; // already subscribed         
        } elif (EthernetUdpSubscribers[i] == NULL_IP) 
        {
          EthernetUdpSubscribers[i] = RemoteIp; // add subscriber
          break;
        }
      }
        
    } else if (data_id == ROVECOMM_UNSUBSCRIBE_REQUEST) 
    {
      int  i;
      for (i=0; i < ROVECOMM_MAX_SUBSCRIBERS; i++) 
      {
        if (EthernetUdpSubscribersIpList[i] == RemoteIp) 
        {
          EthernetUdpSubscribers[i] = NULL_IP; // remove subscriber
          break;        
        }
      }
    }
  }
}
*/
