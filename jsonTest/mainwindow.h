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
    void on_aabbcc_textChanged();

    void on_lineEdit_returnPressed();

    void onReceive();

private:
    Ui::MainWindow *ui;
    QTcpSocket soketQt;
};

#endif // MAINWINDOW_H
