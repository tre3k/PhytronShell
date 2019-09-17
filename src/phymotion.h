#ifndef PHYMOTION_H
#define PHYMOTION_H

#include <unitypes.h>
#include <cstring>
#include <QByteArray>

class PhyMotion
{
public:
    static uint16_t checksum(char addr,char *message){
        uint16_t retval = addr;
        for(size_t i=0;i<strlen(message);i++){
          retval ^= message[i];
        }
        retval ^= ':';
        return retval;
      }

    static uint8_t convertAddr(int value){
        if(value >= 0 && value < 0x0a) return value+0x30;
        if(value >= 0x0a && value <= 0x0f) return value+0x37;
        if(value == 0x10) return 0x40;      // broadcast @
    }

    static QByteArray setBuffer(int addr,QByteArray message){
        QByteArray buffer;
        buffer.clear();
        buffer.append((char)0x02);        // <STX>
        buffer.append((char)convertAddr(addr));
        buffer.append(message);

        return buffer;
    }
};



#endif // PHYMOTION_H
