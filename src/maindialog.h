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

#include "phymotion.h"

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
    QLineEdit *line_command;
    QTextEdit *text_out;
    QSpinBox *spinBox_addr;

    QSerialPort serial_port;

signals:

public slots:
    void serialRefresh();
    void serialSend();
};

#endif // MAINDIALOG_H
