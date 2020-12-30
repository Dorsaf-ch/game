#include "arduino.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
arduino::arduino()
{
    data ="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;
}

int arduino::connect_arduino()
{
    foreach(const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) //des info sur tt les ports con
    {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier())
        {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier() == arduino_uno_product_id)
            {
                arduino_is_available=true;
                arduino_port_name=serial_port_info.portName(); //port name dans laquelle elle est con
            }
        }
    }
    qDebug() << "arduino port name: " << arduino_port_name;
       if (arduino_is_available)
       {
           serial->setPortName(arduino_port_name);
           if (serial->open(QSerialPort::ReadWrite)) //envoyer + recevoir
           {
               serial->setBaudRate(QSerialPort::Baud9600);
               serial->setDataBits(QSerialPort::Data8);
               serial->setParity(QSerialPort::NoParity);
               serial->setStopBits(QSerialPort::OneStop); // 1 bit de stockage
               serial->setFlowControl(QSerialPort::NoFlowControl);
               return 0;
           }
           qDebug() << serial->error();
           return 1;
       }
       return -1;
}


int arduino::close_arduino()
{
    if (serial->isOpen())
    {
        serial->close();
        return 0;
    }
    return 1;
}


int arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable())
    {
        serial->write(d);
    }
    else
        qDebug() << "Couldn't write to serial.";
}


QByteArray arduino:: read_from_arduino()
{
   if (serial->isReadable())
   {
       data=serial->readAll();
       return data;
   }
}
