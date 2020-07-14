#ifndef PRINTADRTABLE_H
#define PRINTADRTABLE_H


#include <QObject>
#include <windows.h>
#include <tlhelp32.h>
#include <QThread>
class printAdrTable : public QObject
{
    Q_OBJECT
    // Свойство, управляющее работой потока
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    // Первое сообщение в объекте




    bool m_running;
    QString m_message;
    int count;  // Счётчик, по которому будем ориентироваться на то,
                // что потоки выполняются и работают

public:
    explicit printAdrTable(QObject *parent = 0);
    bool running() const;

signals:
    void send(int,u_int64,int);
    void finished();    // Сигнал, по которому будем завершать поток, после завершения метода run
    void runningChanged(bool running);



public slots:
    void receiveTableInfo1(int pid, std::vector<uintptr_t> adr);
    void run(); // Метод с полезной нагрузкой, который может выполняться в цикле
    void setRunning(bool running);
    BOOL ListProcessModules1(HANDLE hModuleSnap, uintptr_t addr);
    void go();
private:
    int TableNumber;
    DWORD Pid;
    std::vector<uintptr_t> addres;
};
#endif // PRINTADRTABLE_H
