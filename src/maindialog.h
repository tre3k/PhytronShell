#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSpinBox>
#include <QMessageBox>
#include <QScrollBar>
#include <QThread>
#include <QTcpSocket>

#include "phymotion.h"

#define LISTBUFFER 100

class MainDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private:
    QComboBox *combo_box;
    QPushButton *button_send;
    QPushButton *button_refresh;
    QPushButton *button_ReadAllParams;
    QLineEdit *line_command;
    QLineEdit *line_ip_addr;
    QSpinBox *spin_port;
    QTextEdit *text_out;
    QSpinBox *spinBox_addr;

    QSerialPort serial_port;

    QTcpSocket tcp_sock;
    bool tcp_ip = false;

    QVector<QString> list_buffer;

private:
    void updateOutText(){
        QString text = "";
        for(int i=0;i<list_buffer.size();i++){
            text += list_buffer.at(i) + "<br>";
        }
        text_out->setText(text);
    }

    void addText(QString text){
        list_buffer.append(text);
        if(list_buffer.size() > LISTBUFFER) list_buffer.removeFirst();
        updateOutText();
        text_out->verticalScrollBar()->setValue(text_out->verticalScrollBar()->maximum());
    }

signals:

public slots:
    void serialRefresh();
    void serialSend();
    void readAllParams();
    void readResponse();
};

#endif // MAINDIALOG_H
