#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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


    void onReceive();

    void on_AddressEdit_textChanged(const QString &arg1);

    void on_UserName_textEdited(const QString &arg1);

    void on_RoomName_textEdited(const QString &arg1);

    void on_TalkEdit_textEdited(const QString &arg1);

    void on_TalkEdit_returnPressed();

    void on_ConnectBtn_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket soketQt;
    QString address;
    QString textMessage;
    QString name;
    QString roomNum;

};

#endif // MAINWINDOW_H
