#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&soketQt, SIGNAL(readyRead()), this, SLOT(onReceive()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_aabbcc_textChanged()
{

    QString a= ui->aabbcc->toPlainText();

    if(a.right(1)=="\n")
    {
        a.resize(a.size()-1);
        //QMessageBox(QMessageBox::Information,"hello world~~",a).exec();
        soketQt.connectToHost(a,10125);
        if(soketQt.waitForConnected(5000))
           {
                QMessageBox(QMessageBox::Information,"hello world~~",a).exec();
           }
        else
          {
                QMessageBox(QMessageBox::Information,"error!!!",a).exec();

          }

        ui->aabbcc->setText(a);

        QTextCursor cursor = ui->aabbcc->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->aabbcc->setTextCursor(cursor);
    }

}

void MainWindow::on_lineEdit_returnPressed()
{
    QString a= ui->lineEdit->text();
     QString json = "#json begin {";
     //json += "{";
     json += "'type':'chat',";
     json += "'room':'r123',";
     json += "'text':'"+a+"'";
     json += "} #json end";

     soketQt.write(json.toUtf8().constData());
}


void MainWindow::onReceive()
{
    QTcpSocket* Peer = (QTcpSocket*) sender();
       qint64 PacketSize = Peer->bytesAvailable();

       if(0 < PacketSize)
       {
           char* PacketBuffer = new char[PacketSize + 1];
           Peer->read(PacketBuffer, PacketSize);
           PacketBuffer[PacketSize] = '\0';
           QMessageBox(QMessageBox::Information,"hello",PacketBuffer).exec();
           delete[] PacketBuffer;
       }


}
