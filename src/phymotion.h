#ifndef PHYMOTION_H
#define PHYMOTION_H

#include <unitypes.h>
#include <cstring>
#include <QByteArray>
#include <QString>
#include <QChar>

class PhyMotion
{
public:
    static uint16_t checksum(char addr,QByteArray message){
        uint16_t retval = addr;
        for(int i=0;i<message.size();i++){
          retval ^= message.at(i);
        }
        retval ^= ':';
        return retval;
      }

    static uint8_t convertAddr(int value){
        if(value >= 0 && value < 0x0a) return value+0x30;
        if(value >= 0x0a && value <= 0x0f) return value+0x37;
        if(value == 0x10) return 0x40;      // broadcast @
    }

    static QByteArray getBuffer(int addr,QByteArray message){
        QByteArray buffer;
        buffer.clear();
        buffer.append('\x02');        // <STX>
        buffer.append((char)convertAddr(addr));
        buffer.append(message);
        buffer.append(':');
        buffer.append(QString("%1").arg(checksum((char)convertAddr(addr),message),2,16,QChar('0')).toUpper());
        buffer.append('\x03');
        return buffer;
    }

    static QString parseBuffer(QByteArray buffer){
        QString text = "";
        for(int i=0;i<buffer.size();i++){
            switch(buffer.at(i)){
            case '\x02':
                text += "<font color=\"gray\"><b>STX</b></font>";
                break;
            case '\x03':
                text += "<font color=\"gray\"><b>ETX</b></font>";
            break;
            case '\x06':
                text += "<font color=\"green\"><b>ACK</b></font>";
                break;
            case '\x15':
                text += "<font color=\"red\"><b>NAK</b></font>";
                break;
            default:
                text += buffer.at(i);
                break;
            }
        }
        return text;
    }
};



#endif // PHYMOTION_H
