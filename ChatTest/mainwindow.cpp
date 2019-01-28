#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //
    ui->setupUi(this);
    connect(&soketQt, SIGNAL(readyRead()), this, SLOT(onReceive()));

    //
    ui->AddressEdit->setText("boss2d.com");
    roomNum = "123";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReceive()
{
    //
    QTcpSocket* Peer = (QTcpSocket*) sender();
        qint64 PacketSize = Peer->bytesAvailable();

        //
        if(0 < PacketSize)
        {
            char* PacketBuffer = new char[PacketSize + 1];
            //
            Peer->read(PacketBuffer, PacketSize);
            //
            PacketBuffer[PacketSize] = '\0';
            QString jasonString = PacketBuffer;
            jasonString.remove("#json begin");
            jasonString.remove("json end");
            //
            QJsonDocument a = QJsonDocument::fromJson(jasonString.toUtf8());
            QString b = a["text"].toString("");
            QString c = a["name"].toString("noName");
            //
            if(true)
            {
                QIcon img("../image/in-love.png");
                QListWidgetItem* item = new QListWidgetItem(img, b);
                ui->TalkList->addItem(item);
            }
            else{
                ui->TalkList->addItem("["+c+"]"+b);
            }

            delete[] PacketBuffer;

        }
}

void MainWindow::on_AddressEdit_textChanged(const QString &arg1)
{
    address = arg1;
}

void MainWindow::on_UserName_textEdited(const QString &arg1)
{
    name = arg1;
}

void MainWindow::on_RoomName_textEdited(const QString &arg1)
{
    roomNum = arg1;
}

void MainWindow::on_TalkEdit_textEdited(const QString &arg1)
{
    textMessage = arg1;
}

void MainWindow::on_TalkEdit_returnPressed()
{
    //
    QString json = "#json begin {";
    json += "'type':'chat',";
    json += "'room':'"+roomNum+"',";
    json += "'name':'"+name+"',";
    json += "'text':"+textMessage+"'";
    json += "} #json end";

    //
    soketQt.write(json.toUtf8().constData());

    //
    ui->TalkEdit->clear();


}

void MainWindow::on_ConnectBtn_clicked() //접속
{
    //
    soketQt.connectToHost(address,10125);
    if(soketQt.waitForConnected(5000))
    {
        ui->TalkEdit->setEnabled(true);
        ui->TalkList->setEnabled(true);
    }
    else
    {
         QMessageBox(QMessageBox::Information,"에러",address).exec();
    }
}
