#ifndef RoveEthernetUdp_h
#define RoveEthernetUdp_h

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

#endif // RoveEthernetUdp_h