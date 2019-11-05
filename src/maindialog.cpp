#include "maindialog.h"
#include <QDebug>

MainDialog::MainDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("phyMotion shell");
    this->resize(600,400);

    auto mainLayout = new QVBoxLayout();
    auto low1Layout = new QHBoxLayout();
    auto low2Layout = new QHBoxLayout();

    combo_box = new QComboBox();
    button_refresh = new QPushButton("rfs.");
    button_refresh->setMaximumWidth(30);
    button_send = new QPushButton("send");
    button_send->setMaximumWidth(50);
    button_send->setShortcut(QKeySequence(Qt::Key_Return));
    button_ReadAllParams = new QPushButton("read all Parameters");
    button_ReadAllParams->setMaximumWidth(125);
    line_command = new QLineEdit();
    text_out = new QTextEdit();
    text_out->setReadOnly(true);
    spinBox_addr = new QSpinBox();
    spinBox_addr->setRange(0,0x10);
    spinBox_addr->setDisplayIntegerBase(16);
    spinBox_addr->setMaximumWidth(40);

    line_ip_addr = new QLineEdit();
    spin_port = new QSpinBox();
    spin_port->setRange(1,65536);
    spin_port->setValue(22222);

    auto lowIpLayout = new QHBoxLayout();
    lowIpLayout->addWidget(new QLabel("IP:"));
    lowIpLayout->addWidget(line_ip_addr);
    lowIpLayout->addWidget(spin_port);
    //low1Layout->addWidget(new QLabel("addr:"));
    low1Layout->addWidget(spinBox_addr);
    low1Layout->addWidget(combo_box);
    low1Layout->addWidget(button_refresh);
    //low1Layout->addWidget(button_ReadAllParams); //reservation
    //low1Layout->addStretch(0);
    low2Layout->addWidget(new QLabel("command: "));
    low2Layout->addWidget(line_command);
    low2Layout->addWidget(button_send);

    mainLayout->addLayout(lowIpLayout);
    mainLayout->addLayout(low1Layout);
    mainLayout->addLayout(low2Layout);
    mainLayout->addWidget(text_out);

    this->setLayout(mainLayout);

    serialRefresh();

    connect(button_refresh,SIGNAL(clicked(bool)),
            this,SLOT(serialRefresh()));
    connect(button_send,SIGNAL(clicked(bool)),
            this,SLOT(serialSend()));
    connect(&serial_port,SIGNAL(readyRead()),
            this,SLOT(readResponse()));
    connect(&tcp_sock,SIGNAL(readyRead()),
            this,SLOT(readResponse()));
    connect(button_ReadAllParams,SIGNAL(clicked(bool)),
            this,SLOT(readAllParams()));

    list_buffer.clear();
}

MainDialog::~MainDialog(){
    serial_port.close();
    tcp_sock.disconnect();
    tcp_sock.close();
}

void MainDialog::serialRefresh(){
    combo_box->clear();
    auto infos = QSerialPortInfo::availablePorts();
    for(int i=0;i<infos.size();i++){
        combo_box->addItem(infos.at(i).portName());
    }
}

void MainDialog::serialSend(){
    if(line_ip_addr->text()!="") tcp_ip = true;
    if(tcp_ip){
        //tcp_sock.disconnect()
        tcp_sock.connectToHost(line_ip_addr->text(),spin_port->value());
    }else{
        if(serial_port.portName()!=combo_box->currentText()){
            serial_port.close();
            serial_port.setPortName(combo_box->currentText());
        }
        if(!serial_port.isOpen()){
            if(!serial_port.open(QIODevice::ReadWrite)){
                QMessageBox::critical(nullptr,"Error open port","Error open port: "+combo_box->currentText());
                return;
            }
            serial_port.setBaudRate(115200);
        }
    }


   // PhyMotion::getBuffer(spinBox_addr->value(),line_command->text().toUtf8());
    QByteArray buffer = PhyMotion::getBuffer(spinBox_addr->value(),line_command->text().toUtf8());
    addText("<font color=\"green\">"+QTime::currentTime().toString()
            +"."+QString::number(QTime::currentTime().currentTime().msec())
            +"</font> <b>SEND:</b> "+PhyMotion::parseBuffer(buffer));
    addText("&nbsp;&nbsp;<font color=\"grey\">command: </font>"+line_command->text());
    if(tcp_ip){
        tcp_sock.write(buffer);
    }else{
        serial_port.write(buffer);
    }
}

void MainDialog::readResponse(){
    QByteArray buffer;
    QByteArray out;
    if(tcp_ip){
        buffer.append(tcp_sock.readAll());
    }else{
        buffer.append(serial_port.readAll());
    }

    addText("<font color=\"green\">"+QTime::currentTime().toString()
            +"."+QString::number(QTime::currentTime().currentTime().msec())
            +"</font> <b>RESPONSE:</b> "+PhyMotion::parseBuffer(buffer));
    for(int i=0;i<buffer.size();i++){
        if(buffer.at(i)==':') break;
        if(!(buffer.at(i) == '\x02' ||
             buffer.at(i) == '\x03' ||
             buffer.at(i) == '\x06' ||
             buffer.at(i) == '\x15')) out.append(buffer.at(i));
    }
    if(out.size()!=0) addText("&nbsp;&nbsp;<font color=\"grey\">return value: </font>"+QString(out));
    addText("");

}

void MainDialog::readAllParams(){
    if(serial_port.portName()!=combo_box->currentText()){
        serial_port.close();
        serial_port.setPortName(combo_box->currentText());
    }
    if(!serial_port.isOpen()){
        if(!serial_port.open(QIODevice::ReadWrite)){
            QMessageBox::critical(nullptr,"Error open port","Error open port: "+combo_box->currentText());
            return;
        }
        serial_port.setBaudRate(115200);
    }

    QString cmd;
    QByteArray buffer;
    for(int i=1;i<=2;i++){
        cmd = "M6.1P"+QString::number(i)+"R";
        addText(cmd);
        buffer = PhyMotion::getBuffer(spinBox_addr->value(),cmd.toUtf8());
        serial_port.write(buffer);
    }
}

