﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QStandardPaths>
#include <iostream>
#include <QHBoxLayout>
#include <QObjectUserData>
#include <QPushButton>
#include <algorithm>
//#include <queue> //bfs(image)
#include <QMap>
using namespace std;
//using namespace UsersManagement;
//using namespace FileBox;
#include <QFileDialog>
#include <QDir>
#define STR(A) QString::fromWCharArray(L##A)
#define KOREAN(STR) QString::fromWCharArray(L##STR)

 /* << 파일관리 >>
 *
 * 송신자
 * 파일경로
 *
 * */
class MyData: public QObjectUserData
{
public:
    QString mSender;
    QString mFilePath; // 상대가 준 path (송신한 입장)
};


/*
 * < 사용자 정보 >>
 *
 * (입장한) 사용자
 * 사용자 명 수
 * 참여상태
 * 참여시간
 * */
class UserData: public QObjectUserData
{
public:
    QString mUser;
    QString mUserNumber;
    //bool mIn;
    //double mElapsed;
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //show ui
    ui->setupUi(this);
    connect(&soketQt, SIGNAL(readyRead()), this, SLOT(onReceive()));

    //room domain
    ui->AddressEdit->setText("boss2d.com");
    //room number
    roomNum = "123";


    // 가이드의견: roomNum을 작성하면 아래와 같이 UI도 변경하면 좋습니다
    ui->RoomName->setText(roomNum);

/*
    QDir TargetDir;
    TargetDir = QStandardPaths::standardLocations(QStandardPaths::DataLocation).value(0).toUtf8();
    const QStringList& List = TargetDir.entryList(QDir::Files|QDir::Dirs|QDir::Hidden|QDir::System|QDir::NoDotAndDotDot);
    for(int i=0,iend=List.size(); i<iend; ++i)
    {

    }
*/



//>>>>>>> 6caf40e7e08a9a3b5c760a1902cf649e2a09d765

    //get files object
    QDir FileList;
    //connect to dir
    FileList = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0);
    //stringList type for dir
    const QStringList& List = FileList.entryList(QDir::Files|QDir::Dirs|QDir::Hidden|QDir::System|QDir::NoDotAndDotDot);
    //add files ++
    for(int i=0,iend=List.size(); i<iend; ++i)
    {
        ui->ShowFiles->addItem(List[i]);
    }


}


//..1. at
//..2. length
//..3. & .. .at
MainWindow::~MainWindow()
{

    delete ui;

    //iterator for list
    foreach(auto& iFile, mFileList)
    {
        if(iFile != nullptr)
        {
            iFile->remove();
            delete iFile;

        }
    }

}


/*void MainWindow::on_aabbcc_textChanged()
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

}*/

/*void MainWindow::on_lineEdit_returnPressed()
{
    QString a= ui->TalkEdit->text();
     QString json = "#json begin {";
     //json += "{";
     json += "'type':'chat',";
     json += "'room':'r123',";
     json += "'text':'"+a+"'";
     json += "} #json end";

     soketQt.write(json.toUtf8().constData());
}*/

void MainWindow::onReceive()
{

    //get socket
    QTcpSocket* Peer = (QTcpSocket*) sender();
       qint64 PacketSize = Peer->bytesAvailable();

       if(0 < PacketSize)
       {
           //get packet object
           char* PacketBuffer = new char[PacketSize + 1];
           Peer->read(PacketBuffer, PacketSize);
           PacketBuffer[PacketSize] = '\0';
           //QMessageBox(QMessageBox::Information,"hello",PacketBuffer).exec();


           QString jasonString = PacketBuffer;
           delete[] PacketBuffer;

           jasonString.remove("#json begin");
           jasonString.remove("#json end");


           //받을 데이터 타입, subtype text name
            QJsonDocument json = QJsonDocument::fromJson(jasonString.toUtf8());
            auto subType = json["subtype"].toString();
            //QString
            auto text = json["text"].toString("");
            //QString
            auto name = json["name"].toString("noName");
            //ui->TalkList->addItem("["+c+"]"+b);

            // 가이드의견: 여기서 유저이름을 수집하여 봅시다
            bool NeedAdd = true;
            for(int i = 0; i < ui->ShowUsers->count(); ++i)
            {
                if(name == ui->ShowUsers->item(i)->text()) // 가이드의견: 같은 이름이 리스트에 이미 있다면
                    NeedAdd = false; // 가이드의견: 추가할 필요가 없습니다
            }
            if(NeedAdd)
            {
                ui->ShowUsers->addItem(name);
                ui->ShowUsers->sortItems(); // 가이드의견: 소팅도 한번 해봅시다
            }

            //subType 데이터 처리, fileshare getfile setfile
            if(subType =="fileshare")
            {

                 ui->TalkList->addItem("~~~");
                 QListWidgetItem* NewItem = new QListWidgetItem();
                 NewItem->setBackgroundColor(QColor(255,20,200));
                 NewItem->setSizeHint(QSize(0,100));
                 ui->TalkList->addItem(NewItem);

                 //MyData*
                 auto NewFilePath = json["filepath"].toString();
                 auto NewData = new MyData();
                 NewData->mSender = name;
                 NewData->mFilePath = NewFilePath;

                 // ShowFiles 에서 전송
                 QPushButton* button = new QPushButton("file download");
                 button->setUserData(0,NewData);

                 // Widget 간의 연결
                 connect(button, SIGNAL(pressed()),this,SLOT(DownloadBtn()));

                 // 보낼 레이아웃구조 생성
                 QHBoxLayout* NewLayout = new QHBoxLayout();
                 NewLayout->addWidget(button);
                 NewLayout->addWidget(new QLabel(text));


                // TalkList 뷰에 생성
                 QWidget* NewWidget = new QWidget();
                 NewWidget->setLayout(NewLayout);

                 ui->TalkList->setItemWidget(NewItem,NewWidget);


                 //

            }
            else if(subType =="getfile")
            {


            }
            else if(subType =="setfile") //파일 조각에 대한 교신을 받는다.
            {
               // 디버깅
               // QMessageBox(QMessageBox::Information,"setFile is on~~",json.toJson().constData()).exec();


                //QMessageBox(QMessageBox::Information,"0000000000",json.toJson().constData()).exec();

                auto Base64 = json["base64"].toString();

                QByteArray aa(Base64.toUtf8().constData()); // ## 속도 개선 가능성...
                QByteArray NewBuffer = QByteArray::fromBase64(aa);



                //QMessageBox(QMessageBox::Information,"111111",json.toJson().constData()).exec();

                QFile WriteFile("aaa.txt");

                //QMessageBox(QMessageBox::Information,"222222222",json.toJson().constData()).exec();

                if(WriteFile.open(QFileDevice::WriteOnly))
                {
                    //QMessageBox(QMessageBox::Information,"33333333",json.toJson().constData()).exec();

                    //int Base64.toUtf8().constData();
                    //WriteFile.write(Base64.toUtf8().constData(),Base64.length());
                    //QMessageBox(QMessageBox::Information,"44444444",json.toJson().constData()).exec();

                    WriteFile.write(NewBuffer);
                    WriteFile.close();
                    //QMessageBox(QMessageBox::Information,"5555555",json.toJson().constData()).exec();

                }

                //QMessageBox(QMessageBox::Information,"666666666",json.toJson().constData()).exec();

                auto NewItem = new QListWidgetItem("<"+name+">"+text);
                NewItem->setBackgroundColor(QColor(224,255,192));
                ui->TalkList->addItem(NewItem);

                auto FileID = json["fileid"].toString().toInt();
                auto Done = json["done"].toString().toInt();
                //auto Base64 = json["base64"].toString();


                //**************
                if(auto CurFile = mFileList.at(FileID))
                {
                    QMessageBox(QMessageBox::Information,"666666666",json.toJson().constData()).exec();

                    QByteArray NewBase64(Base64.toUtf8().constData());
                    QByteArray NewBuffer = QByteArray::fromBase64(NewBase64);
                    CurFile->write(NewBuffer);

                    if(Done==1)
                    {

                        QMessageBox(QMessageBox::Information,"77777776",json.toJson().constData()).exec();
                        CurFile->close();
                        auto OldFileName = CurFile->fileName();
                        CurFile->rename(OldFileName.left(OldFileName.length()-9));
                        delete CurFile;
                        mFileList.replace(FileID,nullptr);
                    }
                }



            }

            else
            {
                 ui->TalkList->addItem("[" + name + "]" + text);
            }

            ////////////////
            ////////////////
            /// const QStringList& List =
            /*if(true)
            {
                QIcon img("../image/in-love.png"); //역슬래쉬 쓰려면 두번 써야한다.  "//"
                QListWidgetItem* item = new QListWidgetItem(img, b);
                ui->TalkList->addItem(item); //qt에서 알아서 delete해줌
            }
            else{
                ui->TalkList->addItem("["+c+"]"+b);
            }*/


           //ui->TalkList->addItem(jasonString);


           //ui->TalkList->addItem("ab");
       }
}


//주소버튼
void MainWindow::on_AddressEdit_textChanged(const QString &arg1)
{
    // 송신
    address = arg1;
}


//접속버튼
void MainWindow::on_ConnectBtn_clicked()
{

    // 송신
    soketQt.connectToHost(address,10125);
    if(soketQt.waitForConnected(5000))
       {
            //QMessageBox(QMessageBox::Information,"hello world~~",address).exec();
            ui->TalkEdit->setEnabled(true);
            ui->TalkList->setEnabled(true);
       }
    else
      {
            QMessageBox(QMessageBox::Information,"error!!!",address).exec();

      }
}

//채팅박스 - 편집
void MainWindow::on_TalkEdit_textEdited(const QString &arg1)
{
    textMessage = arg1;

}


//채팅박스 - 엔터키로 대화 전송
void MainWindow::on_TalkEdit_returnPressed()
{

    // \"
    //데이터 타입
    QString json = "#json begin {";
     //json += "{";
     json += "\"type\":\"chat\",";
     json += "\"room\":\""+roomNum+"\",";
     json += "\"name\":\""+name+"\","; //필드를 지워버리면 noName이 나온다..
     json += "\"text\":\""+textMessage+"\"";
     json += "} #json end";


     //송신 후 clear
     soketQt.write(json.toUtf8().constData());
     ui->TalkList->scrollToBottom(); // 대화내용 스크롤링하기
     ui->TalkEdit->clear();

}



//유저 이름
void MainWindow::on_UserName_textEdited(const QString &arg1)
{

    
    name = arg1;

    //using user object

    name = arg1;

    // 가이드의견: 유저이름을 받아오려면 자신의 이름만 받을 것이 아니니
    // onReceive를 사용하는 것이 더 좋겠습니다. 자신의 채팅도 수집되니까요.
    // 여기 아래쪽은 return으로 제외시키겠습니다.
    return;

    //<1>
    //
    // 유저 객체 받아오기
    // 유저 멤버변수 이용하기
//>>>>>>> 6caf40e7e08a9a3b5c760a1902cf649e2a09d765
           auto NewUser = new UserData;
           NewUser->mUser = name;

        for(int j=0, jend=NewUser->mUser.size(); j<jend; ++j)
        {
             //ui->ShowUsers->addItem(name[j]);
             NewUser->mUserNumber += NewUser->mUser;
             ui->ShowUsers->addItem(NewUser->mUser);
        }
}


//방번호
void MainWindow::on_RoomName_textEdited(const QString &arg1)
{
    //송신
    roomNum = arg1;
}





//파일전송 버튼
void MainWindow::on_SendFile_pressed()
{
    auto List = ui->ShowFiles->selectedItems();
     if(0 < List.length())
     {
         const QString ShortName = List[0]->text();

    // const QString FilePath = ui->ShowFiles->;
     QString FullName = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0) + "/" + ShortName;
     const int64_t FileSize = QFileInfo(FullName).size();
    // QString ShortName; // 파일path에서 실제 파일네임만 뺀 이름.
     //FilePath.lastIndexOf("/");

         QString json = "#json begin {";
          json += "\"type\":\"chat\",";
          json += "\"room\":\""+roomNum+"\",";
          json += "\"name\":\""+name+"\",";
          json += "\"text\":\"" + name + " user sends for a File[" + ShortName
              + "(" + QString::number(FileSize) + "Byte)].\",";
          json += "\"subtype\":\"fileshare\",";
          json += "\"filepath\":\"" + ShortName +"\"";
          json += "} #json end";


          //송신
          soketQt.write(json.toUtf8().constData());

     }


}


//채팅박스 내 리스트에서 display
//다운로드 버튼
 void MainWindow::DownloadBtn()
 {
    //QMessageBox(QMessageBox::Information,"Debug","aaaaa").exec();
    //QPushButton* Btn =
     if(auto Btn = qobject_cast<QPushButton*>( sender() ))
     {

        //Btn->userData(0);
        /* args ( 부모 파라미터, 메시지, ) */
        //QFileDialog Dialog (nullptr, QString::fromWCharArray(L"전송받을 폴더 알려주세요"));
        QFileDialog Dialog (nullptr, STR("전송받을 폴더 알려주세요"));
        Dialog.setFileMode(QFileDialog::Directory);
        if(Dialog.exec())
        {


            const QString DirPath = Dialog.selectedFiles()[0];
            //QMessageBox(QMessageBox::Information, "Debug", DirPath).exec();


            //auto List = ui->ShowFiles->selectedItems();
            //const QString ShortName = List[0]->text();
            // const QString FilePath = ui->ShowFiles->;
            //QString FullName = QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).value(0) + "/" + ShortName;
            //const int64_t FileSize = QFileInfo(FullName).size();

            auto Data = (MyData*)Btn->userData(0);


            QString ShortName;
            //short path 만들기 . 제일 마지막 슬래시 위치파악
            int SlashPos = Data->mFilePath.lastIndexOf('/');
            if( SlashPos == -1 )
                ShortName = Data-> mFilePath;
            else
                ShortName = Data->mFilePath.right(Data->mFilePath.length()-SlashPos-1);

            
            
            //수신
            QFile* WriteFile = new QFile(DirPath+"/"+ShortName+".download"); // 확장자는 구분하는 이름일 뿐이다. 의미없음.
            if(WriteFile->open(QFileDevice::WriteOnly))
            {

                int FileID = mFileList.length();
                //.append 는 제일 마지막에 꽂아넣기.
                mFileList.append(WriteFile);


                QString Msg = "#json begin {";
                //....
                Msg += "\"fileid\":\"" + QString:: number(FileID) + "\"";
                Msg += "}#json end";

                soketQt.write(Msg.toUtf8().constData());

            }

            
            //송신
            //데이터는 앞쪽경계선

            // 선언과 = 는 "초기화"
            //  = 는 "대입"
            //이라 부른다.

            // 초기화 타이밍
            // 초기화 리스트의 존재 이유는 const int b; 와 같은 경우 때문이다. 대입을 못하니까 생성자 아규먼트에서 선언해줌.
            // const 가 적혀있는 선언문은 반드시 초기화 리스트를 작성해준다.
            // 주소변환 금지 int* const a;  =======> 0으로 초기화해준다.
            // 값 변환 금지 const:: int b ;   =======> nullptr로 초기화 해준다.

            QString Msg = "#json begin {";
            Msg += "\"type\":\"chat\",";
            Msg += "\"room\":\""+roomNum+"\",";
            Msg += "\"name\":\""+name+"\",";
            Msg += KOREAN("\"text\":\" 파일받기를 시작합니다\",");

            Msg += "\"subtype\":\"getfile\",";
            Msg += "\"sender\":\""+ Data->mSender+"\",";
            Msg += "\"filepath\":\"" + Data->mFilePath +"\",";
            Msg += "\"fileid\":\"0\"";
            Msg += " } #json end";
            soketQt.write(Msg.toUtf8().constData());

        }

        //dynamic_cast<QPushButton*>( sender() );   //( QPushButton*)sender();
        //Btn->userData(0);

        // c++은 0 외 모두 성공
        // JAVA, C# 은 불린형식으로 !=0 처럼 해준다.

        //MyData* OneData =(MyData*)Btn->userData(0);
        //OneData->mFilePath;
        //QMessageBox(QMessageBox::Information,"Debug",OneData->mFilePath).exec();
    }
 }


//이모지 리스트에 넣기
void MainWindow::on_ImageLoad_clicked()
{
/*
    QListWidget list;
    QIcon im("../image/in-love.png");
    QListWidgetItem* item = new QListWidgetItem();
    item->setIcon(img);
    //test1
    list.addItem(item);
    ui->ImageBox->addItem(item);
*/

    //QMap<QIcon,QString> imozyset;

    QIcon love("../image/in-love.png");
    imozyset.value(love);
    imozyset.key("love");
    //happy
    QIcon happy("../image/happy.png");
    imozyset.value(happy);
    imozyset.key("happy");
    //sad
    QIcon crying("../image/crying.png");
    imozyset.value(crying);
    imozyset.key("crying");
    //angry
    QIcon angry("../image/angry.png");
    imozyset.value(angry);
    imozyset.key("angry");

    QMap<QIcon,QString>::iterator it;
    for (it->begin();it->end();++it) {

        QIcon con; con = it.key();
        QString st; st = it.value();
        QListWidgetItem *item= new QListWidgetItem(con,st);
       // item->setIcon(imozyset.key(),imozyset.value());
        ui->ImageBox->addItem(item);
    }

/* 가이드  요청 1.

    C:\Qt\5.12.0\msvc2017_64\include\QtCore\qmap.h:71: error: C2678: 이항 '<': 왼쪽 피연산자로 'const Key' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
    with
    [
    Key=QIcon
    ]
*/
}



//이모지 클릭 -> edit 창으로
void MainWindow::on_ImageBox_itemClicked(QListWidgetItem *item)
{
    /* 가이드 요청 2. */
    return imozyset = item;
    //C:\Users\Jeon2\Documents\GitHub\jsonchat_bell\jsonTest\mainwindow.cpp:647: error: no viable overloaded '='
}



