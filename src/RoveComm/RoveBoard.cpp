// RoveBoard.cpp for Energia
// Author: Gbenga Osibodu

#include "RoveBoard.h"

EthernetUDP udpReceiver;

roveUART_Handle roveBoard_UART_open(unsigned int uart_index, unsigned int baud_rate) {
  HardwareSerial* uartArray[8] = {&Serial , &Serial1, &Serial2, &Serial3,
                                  &Serial4, &Serial5, &Serial6, &Serial7};
  
  uartArray[uart_index] -> begin(baud_rate);
  return uartArray[uart_index];
}

roveBoard_ERROR roveBoard_UART_write(roveUART_Handle uart, void* write_buffer, size_t bytes_to_write) {
  uart -> write((uint8_t*)write_buffer, bytes_to_write);
  uart -> flush();
    
  return ROVE_BOARD_ERROR_SUCCESS;
}

roveBoard_ERROR roveBoard_UART_read(roveUART_Handle uart, void* read_buffer, size_t bytes_to_read) {
  char* temp; //Serial.print("read#: "); Serial.println(bytes_to_read);

  if (bytes_to_read == 0) {
    return ROVE_BOARD_ERROR_SUCCESS;
  }
  
  if (read_buffer == NULL) {
    char trash[bytes_to_read];
    temp = trash;
  } else {
    temp = (char*)read_buffer;
  }

  for (int i =0; i<bytes_to_read; i++) {
    while(uart -> available() == 0);
    temp[i] = uart -> read();//Serial.println(temp[i],DEC);
  }
  
  return ROVE_BOARD_ERROR_SUCCESS;
} 

bool roveBoard_UART_available(roveUART_Handle uart) {
  if (uart -> available() > 0) {
    return true;
  } else {
    return false;
  }
}

void roveEthernet_NetworkingStart(roveIP myIP)
{
  Ethernet.begin(0, myIP);
  Ethernet.enableLinkLed();
  Ethernet.enableActivityLed();
}

roveIP roveEthernet_SetIP(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
  roveIP temp = IPAddress(first_octet, second_octet, third_octet, fourth_octet);
  return temp;
}

roveEthernet_Error roveEthernet_UdpSocketListen(uint16_t port)
{
  udpReceiver.begin(port);
  return ROVE_ETHERNET_ERROR_SUCCESS;
}

roveEthernet_Error roveEthernet_SendUdpPacket(roveIP destIP, uint16_t destPort, const uint8_t* msg, size_t msgSize)
{
  udpReceiver.beginPacket(destIP, destPort);
  udpReceiver.write(msg, msgSize);
  udpReceiver.endPacket();
  return ROVE_ETHERNET_ERROR_SUCCESS;
}

roveEthernet_Error roveEthernet_GetUdpMsg(roveIP* senderIP, void* buffer, size_t bufferSize)
{
  int packetSize = udpReceiver.parsePacket(); 
  
  if (packetSize > 0) //if there is a packet available
  {
    udpReceiver.read((char*)buffer, bufferSize);
    *senderIP = udpReceiver.remoteIP();
    return ROVE_ETHERNET_ERROR_SUCCESS;
  }
  else
  {
    return ROVE_ETHERNET_ERROR_WOULD_BLOCK;
  }
}

void wait(int micros) {
  delayMicroseconds(micros);
}
