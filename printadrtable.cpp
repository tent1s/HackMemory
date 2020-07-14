#include "printadrtable.h"

#include <QDebug>


printAdrTable::printAdrTable(QObject *parent) :
    QObject(parent),
    m_message("")
{

}

bool printAdrTable::running() const
{
    return m_running;
}

void printAdrTable::receiveTableInfo1(int pid, std::vector<uintptr_t> adr)
{
    this->Pid = pid;
    this->addres = adr;
    qDebug() << "1 ";
    go();
}

BOOL printAdrTable::ListProcessModules1(HANDLE hModuleSnap, uintptr_t addr)
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
        if (cmp < base && cmp >(DWORD64)me32.modBaseAddr) {

            return(TRUE);
        }
        else if (cmp > (DWORD64)me32.modBaseAddr) {}
        else
        {
            exit;
        }
    } while (Module32Next(hModuleSnap, &me32));
    return(TRUE);
}



void printAdrTable::run()
{

}
void printAdrTable::go(){
    count = 0;
    qDebug() << "2 ";
    while (m_running)
    {
        int actualValue;
        HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE,Pid);
        HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, Pid);
        int id = qRegisterMetaType <u_int64> ();
        for (auto&list : addres) {
            ListProcessModules1(hModuleSnap, list);
            ReadProcessMemory(phandle, (LPVOID)list, &actualValue, sizeof(actualValue), 0);
            id=list;
            emit send(count,list,actualValue);
            count++;
        }
        emit finished();
        if(count==addres.size()){
            break;
            CloseHandle(hModuleSnap);
        }
    }
    emit finished();
}

void printAdrTable::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(running);
}

