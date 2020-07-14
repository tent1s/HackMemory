#include "printtablethread.h"
#include <QDebug>
#include <windows.h>
#include <QCoreApplication>
#include <iostream>
#include <QtWidgets>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <iomanip>
#include <exception>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <QMessageBox>
printTableThread::printTableThread(QObject *parent) :
    QObject(parent),
    m_message("")
{

}

bool printTableThread::running() const
{
    return m_running;
}
void printTableThread::receiveTableInfo1(int TableNumber,DWORD pid,std::vector<uintptr_t>adr)
{
    this->TableNumber = TableNumber;
    this->pid = pid;
    this->adr = adr;

}

BOOL ListProcessModules(HANDLE hModuleSnap, uintptr_t addr)
{
    MODULEENTRY32 me32;
    if (hModuleSnap == INVALID_HANDLE_VALUE)
        return(FALSE);


    me32.dwSize = sizeof(MODULEENTRY32);

    if (!Module32First(hModuleSnap, &me32))
        return(FALSE);


    do
    {
        DWORD base = (uintptr_t)(me32.modBaseAddr) + me32.modBaseSize;
        DWORD cmp = addr;
        if (cmp < base && cmp >(DWORD_PTR)me32.modBaseAddr) {
            return(TRUE);
        }
        else if (cmp > (DWORD_PTR)me32.modBaseAddr) {}
        else
        {
            exit;
        }
    } while (Module32Next(hModuleSnap, &me32));
    return(TRUE);
}

// Самый важный метод, в котором будет выполняться "полезная" работа объекта
void printTableThread::run()
{
    count = 0;
    HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    // Переменная m_running отвечает за работу объекта в потоке.
    // При значении false работа завершается
    while (m_running)
    {
        HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
        int actualValue=0;
        int x=0;
        for (auto&list : adr) {
            ListProcessModules(hModuleSnap, list);
            ReadProcessMemory(phandle, (LPVOID)list, &actualValue, sizeof(actualValue), 0);
            emit send
            x++;
        }
    }
    emit finished();
}

void printTableThread::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(running);
}


