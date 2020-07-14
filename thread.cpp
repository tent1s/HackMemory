#include "thread.h"
#include <QDebug>


Thread::Thread(QString s) : name(s)
{
}
void Thread::receiveTableInfo(int TableNumber,DWORD pid, std::vector<u_int64> adr)
{
    this->TableNumber = TableNumber;
    this->pid = pid;
    this->adr = adr;

}
void Thread::run()
{

   int i=1;
   while(true)
    {

    for (int j=0;j<TableNumber;j++)
    {  
        unsigned long long bytesRead = 0;
        HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, false, pid);
        ReadProcessMemory(phandle, (LPVOID)adr[j], &i,sizeof(i),&bytesRead);
        if(!bytesRead) {
            i=0;
             qDebug() << "ошибка чтения поток номер 2 "<< GetLastError();
        }
        emit send(i,j);
    }
    Sleep(1000);
    }
}

