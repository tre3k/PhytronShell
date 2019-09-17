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
    line_command = new QLineEdit();
    text_out = new QTextEdit();
    text_out->setReadOnly(true);
    spinBox_addr = new QSpinBox();
    spinBox_addr->setRange(0,0x10);
    spinBox_addr->setDisplayIntegerBase(16);
    spinBox_addr->setMaximumWidth(40);

    low1Layout->addWidget(spinBox_addr);
    low1Layout->addWidget(combo_box);
    low1Layout->addWidget(button_refresh);
    low2Layout->addWidget(new QLabel("command: "));
    low2Layout->addWidget(line_command);
    low2Layout->addWidget(button_send);

    mainLayout->addLayout(low1Layout);
    mainLayout->addLayout(low2Layout);
    mainLayout->addWidget(text_out);

    this->setLayout(mainLayout);

    serialRefresh();

    connect(button_refresh,SIGNAL(clicked(bool)),
            this,SLOT(serialRefresh()));
    connect(button_send,SIGNAL(clicked(bool)),
            this,SLOT(serialSend()));

    text_out->setText(QTime::currentTime().toString());
}

MainDialog::~MainDialog(){
    serial_port.close();
}

void MainDialog::serialRefresh(){
    combo_box->clear();
    auto infos = QSerialPortInfo::availablePorts();
    for(int i=0;i<infos.size();i++){
        combo_box->addItem(infos.at(i).portName());
    }
}

void MainDialog::serialSend(){
    if(serial_port.portName()!=combo_box->currentText()){
        serial_port.close();
        serial_port.setPortName(combo_box->currentText());
    }
    if(!serial_port.isOpen()){
        if(!serial_port.open(QIODevice::ReadWrite)){
            QMessageBox::critical(nullptr,"Error open port","Error open port: "+combo_box->currentText());
        }
    }

    qDebug() << PhyMotion::setBuffer(spinBox_addr->value(),line_command->text().toUtf8());


}
