#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
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

    void on_ImageLoad_clicked(); //이모지 리스트에 넣기
    
    void on_ImageBox_itemClicked(QListWidgetItem *item); //이모지 클릭 -> edit 창으로

private:
    Ui::MainWindow *ui;
    QTcpSocket soketQt;
    QString address;
    QString textMessage;
    QString name;
    QString roomNum;
    int FileSize;
    QList<QFile*> mFileList;
    //QList<QIcon*> mIconLoad; // * 체크
    QMap<QIcon,QString> imozyset;

};

#endif // MAINWINDOW_H
