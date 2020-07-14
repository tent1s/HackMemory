#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include "thread.h"
#include "changeadr.h"
#include "printadrtable.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString ProcInfo() const { return newPName,newPID; }
               void setProcInfo(const QString& newPName,const QString& newPID);
    QString setAdr1() const { return adr; }
                void setAdr1(const QString& adr);

protected:
    void changeEvent(QEvent *) override;
private slots:
    void changeStyle(const QString &styleName);
    void styleChanged();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    int getTyp();
    DWORD getPID();
    void on_pushButton_3_clicked();
    void printAddress(int m,int x, HANDLE hModuleSnap, HANDLE phandle);
    void on_pushButton_4_clicked();
    void UptValue();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_tableWidget_2_cellDoubleClicked(int row, int column);
    void on_pushButton_6_clicked();
    void update (int i,int j);
    void setTableAdr(int row, int column);
    void startStream1();
    void darkTheam();
    bool OpenMyProcess(DWORD pID);
    void on_checkBox_stateChanged(int arg1);
    void update1(int i,u_int64 j,int actualValue);


private:

    QString newPName;
    QString newPID;
    QString adr;
    QThread threadTable1;
    printAdrTable printAdrTable;

signals:
    void sendTableInfo(int,DWORD, std::vector<u_int64>);
    void sendAdrInfo(QString,QString,int,int);
    void sendTableInfo1(int, std::vector<uintptr_t>);

};
#endif // MAINWINDOW_H
