#include "proclist.h"
#include "ui_proclist.h"

ProcList::ProcList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcList)
{
    ui->setupUi(this);
    QIcon winIcon("Icon.ico");
    this->setWindowIcon(winIcon);
    setWindowTitle(tr("Выбор процесса"));
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << trUtf8("ID") << trUtf8("Процесс")<< trUtf8("Память"));
    ui->tableWidget->setEditTriggers(0);
    QObject::connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(getPID()));
    on_pushButton_clicked();

}

ProcList::~ProcList()
{
    delete ui;
    MainWindow *fmr = new  MainWindow(0);

}
QString copyToQString(WCHAR array[])
{
    QString string;
    int i = 0;

    while (array[i] != 0)
    {
        string[i] = array[i];
        i++;
    }
    return string;
}
void ProcList::on_pushButton_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->model()->removeRows(0, ui->tableWidget->rowCount());
    QString procName;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap  == NULL)
    {
        QMessageBox::critical(0, "ОШИБКА", "Error Load ToolHelp", QMessageBox::Close);
        return;
    }
    PROCESSENTRY32 pc32 = { sizeof(pc32) };
    quint64 i = 0;
    if (Process32First(hSnap , &pc32))
    {
        while (Process32Next(hSnap , &pc32))
        {
            HANDLE curHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pc32.th32ProcessID);
            PROCESS_MEMORY_COUNTERS_EX pmc;
            if(GetProcessMemoryInfo(curHandle, reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc))!=0)
            {
            ui->tableWidget->insertRow(i);
            qint64 ID = pc32.th32ProcessID ;
            procName = copyToQString(pc32.szExeFile);

            ui->tableWidget->setItem(i,0, new QTableWidgetItem(QString::number(ID)));
            ui->tableWidget->setItem(i,1, new QTableWidgetItem(procName));
            ui->tableWidget->setItem(i,2, new QTableWidgetItem(QString::number(pmc.WorkingSetSize / 1024)));
            ++i;
            }
        }
    }
    ui->tableWidget->resizeColumnsToContents();
}


void ProcList::on_pushButton_2_clicked()
{
   QWidget::close();
}

QString Pid;
QString Pname;
void ProcList::on_tableWidget_cellClicked(int row, int column)
{


       QTableWidgetItem *item = ui->tableWidget->item(row,0);
       if (NULL != item) {
       Pid = item->text();
     }
       QTableWidgetItem *item2 = ui->tableWidget->item(row,1);
       if (NULL != item2) {
       Pname = item2->text();
     }
}

void ProcList::getPID()
{
    MainWindow *fmr = new  MainWindow(0);
    fmr->setProcInfo(Pname,Pid);
 }


