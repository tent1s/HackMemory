#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QString>
#include <windows.h>
#include <QCoreApplication>
#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <iomanip>
#include <exception>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
class Thread : public QThread
{
Q_OBJECT
public:
explicit Thread(QString s);

void run();
public slots:
void receiveTableInfo(int TableNumber,DWORD pid, std::vector<u_int64> adr);
signals:
void send(int,int);
private:
QString name;
int TableNumber;
DWORD pid;
std::vector<u_int64> adr;
};

#endif // THREAD_H
