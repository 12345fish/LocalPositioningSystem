/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "modules\kSerial.h"
/*====================================================================================================*/
/*====================================================================================================*/
static ptrSerial pSerialFunc = NULL;
/*====================================================================================================*/
/*====================================================================================================*
**函數 : kSerial_config
**功能 : Point to Serial Send Function
**輸入 : None
**輸出 : None
**使用 : kSerial_config(Serial_SendByte);
**====================================================================================================*/
/*====================================================================================================*/
void kSerial_config( ptrSerial pSerial )
{
  pSerialFunc = pSerial;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : getTypeSize
**功能 : get data type size
**輸入 : type
**輸出 : dataSize
**使用 : dataSize = getTypeSize(KS_INT16);
**====================================================================================================*/
/*====================================================================================================*/
static uint8_t getTypeSize( uint8_t type )
{
  switch(type) {
    case KS_INT8:     return 1;
    case KS_UINT8:    return 1;
    case KS_INT16:    return 2;
    case KS_UINT16:   return 2;
    case KS_INT32:    return 4;
    case KS_UINT32:   return 4;
    case KS_INT64:    return 8;
    case KS_UINT64:   return 8;
    case KS_FLOAT32:  return 4;
    case KS_FLOAT64:  return 8;
    default:          return 0;
  }
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : kSerial_sendData
**功能 : kSerial Send Data
**輸入 : *signalData, lens, type
**輸出 : None
**使用 : kSerial_sendData(signalData, 12, KS_INT16);
**====================================================================================================*/
/*====================================================================================================*/
void kSerial_sendData( void *signalData, uint8_t lens, uint8_t type )
{
  uint8_t packet[64] = {0};
  uint8_t *pPacket = packet;

  uint16_t dataBytes  = lens * getTypeSize(type);
  uint16_t packetSize = dataBytes + 6;
  uint16_t packetInfo = ((uint16_t)type << 8) | dataBytes;

  packet[0] = 'K';                                      /* header 'K'  */
  packet[1] = 'S';                                      /* header 'S'  */
  packet[2] = (packetInfo & 0x00FF);                    /* lens & type */
  packet[3] = (packetInfo & 0xFF00) >> 8;               /* lens & type */
  for(uint8_t count = 0; count < dataBytes; count++)    /* data        */
    packet[count + 4] = ((uint8_t*)signalData)[count];
  packet[packetSize - 2] = '\r';                        /* 'r'         */
  packet[packetSize - 1] = '\n';                        /* 'n'         */

  do {
    pSerialFunc(*pPacket++);
  } while(--packetSize);
}
/*====================================================================================================*/
/*====================================================================================================*/
