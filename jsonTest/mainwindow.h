#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDialog>
#include <string>
#include <QFile>
/*
class QCheckBox;

class FindDialog : public QDialog
{
    Q_OBJECT 
    
 public:
    
    FindDialog(QWidget *parent = 0);

 signals:
    
    void findNext(const QString &str, Qt::CaseSensitivity cs);
 
 private slots:
 
 private:
    
};
*/

/* 상속관계 고민하기.

//1명 사용자 정보
namespace anonym{
class People;
}

class People: public
public:

private slots:

private:

};

*/

/*
//전체 사용자 관리

namespace UsersManagement{
//class Records;
//}

class Records//: public anonymManagement
{

public:

    Records();
    //오버로딩
    Records(const std::string& mUserName);

    //객체 메서드
    void invite(int roomNumber = 123);
    void vanish();
    //void promotion();
    void display() const;
    void newcomerMessage() const;
    void leftPerson() const;

    void setInitRecords();
    const std::string& getInitRecords() const;
    //int getInitRecords() const;

    void setCurrRecords(const std::string& mUserName);
    const std::string& getCurrRecords() const;

    void setNewcomer(const std::string& mUserName);
    const std::string getNewcomer() const;

    void setUserName(const int roomNumber);
    const std::string getUserName() const;

    void setVanishNum(const std::string& mUserName);
    const int getVanishNum() const;

    void setOutRecords(const std::string& mUserName);
    const int getOutRecords() const;


private slots:

private: //객체의 프로퍼티 (데이터멤버)

    std::string mUserName;
    std::string mComingName;
    int totalUesrsNumber;
    int roomNumber;
    int userPosition;

    };

}
*/

/* 파일 1개 정보 일반화 시키기. #확장자 분류? */
/*
namespace FileBox{

class FileInfo// : public FileBox
{

public:

    FileInfo();

    //객체 메서드
    void send();
    void receive();
    void display() const;

    void setFileName(const std::string& fileName);
    const std::string getFileName() const; // 파일이름에 변경이 없음 
    int getSendingCount();
    int getreceiveCount();

private slots:
private:

    //객체 프로퍼티 (데이터멤버)
    std::string mFileName;
    int fileCounts;
    int sendingCounts;
    int receiveCounts;

    };
}

*/
/* 사용자 전체 파일관리 */
// 사용자 전체 파일관리 객체는 파일 1개 정보에 대한 객체를 가진다.
/*
namespace FileManagement{
//class Files;
//}

class Files// : public FileManagement
{

public:


private slots:


private:


    };
}

*/

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
    //void on_aabbcc_textChanged();

    //void on_lineEdit_returnPressed();

    void onReceive();

    void on_AddressEdit_textChanged(const QString &arg1);

    void on_ConnectBtn_clicked();

    void on_TalkEdit_textEdited(const QString &arg1);

    void on_TalkEdit_returnPressed();

    void on_UserName_textEdited(const QString &arg1);

    void on_RoomName_textEdited(const QString &arg1);

    //void on_PeopleNum_indexesMoved(const QModelIndexList &indexes);

    //void on_ShowFiles_itemEntered(QListWidgetItem *item);

    //void on_ShowFiles_itemPressed(QListWidgetItem *item);

    //void on_SendFile_clicked();

    //void on_SaveFiles_clicked();

    void on_SendFile_pressed();

    void DownloadBtn(); //신호받기

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

};

#endif // MAINWINDOW_H
