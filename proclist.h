#ifndef PROCLIST_H
#define PROCLIST_H

#include <QDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <QWinEventNotifier>
#define _WIN32_WINNT 0x0500
#include<windows.h>
#include <TlHelp32.h>
#include <w32api.h>
#include <tlhelp32.h>
#include <QMap>
#include <QString>


namespace Ui {
class ProcList;
}

class ProcList : public QDialog
{
    Q_OBJECT

public:
    explicit ProcList(QWidget *parent = nullptr);
    ~ProcList();

private slots:
    void getPID();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::ProcList *ui;
};

#endif // PROCLIST_H
