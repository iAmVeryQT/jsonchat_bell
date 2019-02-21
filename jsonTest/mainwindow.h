#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDialog>
#include <string>
#include <QFile>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onReceive();//client socket

    void on_AddressEdit_textChanged(const QString &arg1); // args

    void on_ConnectBtn_clicked(); // 서버연결버튼

    void on_TalkEdit_textEdited(const QString &arg1); // args 채팅

    void on_TalkEdit_returnPressed();// enter key

    void on_UserName_textEdited(const QString &arg1); //args 사용자이름 그대로 받아오기

    void on_RoomName_textEdited(const QString &arg1); //args 방번호 그대로 받아오기

    void on_SendFile_pressed();

    void DownloadBtn(); //신호받기

    void on_ImageLoad_clicked();
    
   // void on_pushButton_clicked(const QIcon &recentIcon);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket soketQt;
    QString address;
    QString textMessage;
    QString name;
    QString roomNum;
    //QString ShortName;
    int FileSize;
    QList<QFile*> mFileList;
    QList<QIcon*> mIconLoad; // * 체크

};

#endif // MAINWINDOW_H
