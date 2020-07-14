#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "proclist.h"
#include <cstdlib>
#include <QMessageBox>
#include "ScanerMemory.hpp"
#include "changeadr.h"
QString PIDStr="NULL";
QString PName="NULL";
std::vector<uintptr_t> Addres;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    qRegisterMetaType<std::vector<uintptr_t>>("std::vector<uintptr_t>");
    qRegisterMetaType<u_int64>("u_int64");
    darkTheam();
    QIcon winIcon("Icon.ico");
    this->setWindowIcon(winIcon);
    setWindowTitle(tr("HackMemory"));
    ui->label_3->setText(PName);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(1, 75);
    ui->tableWidget->setColumnWidth(0, 75);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << trUtf8("Адресс") << trUtf8("Значение"));
    ui->tableWidget->setEditTriggers(0);
    ui->tableWidget_2->setColumnCount(2);
    ui->tableWidget_2->setShowGrid(true);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << trUtf8("Адресс") << trUtf8("Значение"));
    ui->tableWidget_2->setEditTriggers(0);
    ui->pushButton_4->hide();
    ui->lcdNumber->setPalette(Qt::black);
//*********************************************поточек намбер 1
    connect(&threadTable1, &QThread::started, &printAdrTable, &printAdrTable::run);
    connect(&printAdrTable, &printAdrTable::finished, &threadTable1, &QThread::terminate);
    connect(this, SIGNAL(sendTableInfo1(int,std::vector<uintptr_t>)), &printAdrTable, SLOT(receiveTableInfo1(int,std::vector<uintptr_t>)));
    connect(&printAdrTable, SIGNAL(send(int,u_int64,int)), this, SLOT(update1(int,u_int64,int)));
    printAdrTable.moveToThread(&threadTable1);
//**********************************************поточек намбер 2

    Thread *thread= new Thread("A");
    std::vector<u_int64> adr(ui->tableWidget_2->rowCount());
    for (int i=0;i<ui->tableWidget_2->rowCount();i++ ) {
        QTableWidgetItem *item = ui->tableWidget_2->item(i,0);
        adr[i] = item->text().toLongLong(nullptr,16);
    }
    connect(thread, SIGNAL(send(int,int)), this, SLOT(update(int,int)));
    connect(this, SIGNAL(sendTableInfo(int,DWORD, std::vector<u_int64>)), thread, SLOT(receiveTableInfo(int,DWORD, std::vector<u_int64>)));
    emit sendTableInfo(ui->tableWidget_2->rowCount(), getPID(),adr);
    thread->start();


    //*****************************************STYLE**********************************************
    const QString defaultStyleName = QApplication::style()->objectName();
    QStringList styleNames = QStyleFactory::keys();
    for (int i = 1, size = styleNames.size(); i < size; ++i) {
        if (defaultStyleName.compare(styleNames.at(i), Qt::CaseInsensitive) == 0) {
            styleNames.swapItemsAt(0, i);
            break;
        }
    }
    ui->styleComboBox->addItems(styleNames);
    connect(ui->styleComboBox, &QComboBox::textActivated,
            this, &MainWindow::changeStyle);



    //*****************************************STYLE**********************************************
}
MainWindow::~MainWindow()
{
    delete ui;
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void MainWindow::update1(int i,u_int64 j,int actualValue)// инфа из потока
{
    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i,0, new QTableWidgetItem(QString::number(j, 16)));
    ui->tableWidget->setItem(i,1, new QTableWidgetItem(QString::number(actualValue)));
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//*********************************************************************************************************************************//
void MainWindow::startStream1()
{
    Thread *thread= new Thread("A");
    std::vector<u_int64> adr(ui->tableWidget_2->rowCount());
    for (int i=0;i<ui->tableWidget_2->rowCount();i++ ) {
        QTableWidgetItem *item = ui->tableWidget_2->item(i,0);
        adr[i] = item->text().toLongLong(nullptr,16);
    }
    connect(thread, SIGNAL(send(int,int)), this, SLOT(update(int,int)));
    connect(this, SIGNAL(sendTableInfo(int,DWORD, std::vector<u_int64>)), thread, SLOT(receiveTableInfo(int,DWORD, std::vector<u_int64>)));
    emit sendTableInfo(ui->tableWidget_2->rowCount(), getPID(),adr);
}

//********************************************************************************************************************************//
void MainWindow::on_pushButton_clicked()//proclist
{

    ProcList window;
    window.setModal(true);
    window.exec();
    ui->label_3->setText(PName);

}
enum type_s
{
    byBytes = 1,
    by2Bytes = 2,
    by8Bytes = 8,
    by4Bytes = 4,
    byFloat = 3
};

void MainWindow::setProcInfo(const QString& newPName, const QString& newPID)//инфа из сет проц инфа
{
    PName = newPName;
    PIDStr = newPID;
}

int MainWindow::getTyp(){
    int typ=0;
    QString typ2 =ui->comboBox->currentText();
    if (typ2=="--"){
        QMessageBox errortyp;
        errortyp.setText("Выберите тип переменной!");
        errortyp.exec();
    }
    if (typ2=="4 Bytes"){
        typ=4;
    }
    if (typ2=="Byte"){
        typ=1;
    }
    if (typ2=="2 Bytes"){
        typ=2;
    }
    if (typ2=="8 Bytes"){
        typ=8;
    }
    if (typ2=="Float"){
        typ=3;
    }
    return typ;
}

DWORD MainWindow::getPID(){
    int PID = PIDStr.toInt();
    DWORD pid = (DWORD)PID;
    return pid;

}

void MainWindow::UptValue()//говно
{
    DWORD pid = getPID();
    HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
    int actualValue=1;
    for (int i=0;i<ui->tableWidget_2->rowCount();i++ ) {
        QTableWidgetItem *item = ui->tableWidget_2->item(i,0);
        int_least64_t adr = item->text().toInt(nullptr,16);
        ReadProcessMemory(phandle, (LPVOID)adr, &actualValue, sizeof(actualValue), 0);
        ui->tableWidget_2->setItem(i,1, new QTableWidgetItem(QString::number(actualValue)));
    }
}
//------------------------------------------------------------------------------------------------------------------------------//

void MainWindow::printAddress(int m,int x, HANDLE hModuleSnap, HANDLE phandle){
    ui->lcdNumber->display(m);
    ui->tableWidget->clearContents();
    ui->tableWidget->model()->removeRows(0, ui->tableWidget->rowCount());
    printAdrTable.setRunning(true);
    threadTable1.start();
    emit sendTableInfo1(getPID(),Addres);
}


bool MainWindow::OpenMyProcess(DWORD pID) {
    HANDLE pHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pID); //PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION
    if (pHandle == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, L"Can't Open Process.\n Run As Admin.", L"FV2", MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}
void MainWindow::on_pushButton_2_clicked()// 1 scan
{
    DWORD pid = getPID();
    OpenMyProcess(pid);
    int typ=getTyp();
    ui->comboBox->setDisabled(true);
    ui->pushButton_2->hide();
    ui->pushButton_4->show();
    HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE,pid);
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    mapmemory(phandle, Readable);
    TypeV Tp;
    int x = 0;
    if(ui->lineEdit->text()== NULL){
        QMessageBox errortyp;
        errortyp.setText("А КАВО ИЩЕМ ДРУГАЛЕК");
        errortyp.exec();
        return;
    }
    QString S2 = ui->lineEdit->text();
    double ValorScan = S2.toDouble();
    switch (typ)
    {
    case byBytes:{
        Tp = Bytes;
        Addres = scanner<byte>(ValorScan, phandle, pid, x, Tp, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;
   }
    case by2Bytes:{
        Tp = D2Bytes;
        Addres = scanner<__int32>(ValorScan, phandle, pid, x, Tp, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;
   }
    case by8Bytes:{
        Tp = D8Bytes;
        Addres = scanner<DWORD>(ValorScan, phandle, pid, x, Tp, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;
   }
    case by4Bytes:{
        Tp = D4Bytes;
        Addres = scanner<int>(ValorScan, phandle, pid, x, Tp, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;
   }
    case byFloat:{
        Tp = D4Bytes;
        Addres = scanner<float>(ValorScan, phandle, pid, x, Tp, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
   }
    default:
        break;
    }
     CloseHandle(hModuleSnap);

}

void MainWindow::on_pushButton_3_clicked()// 2 scan
{
    printAdrTable.setRunning(false);
    DWORD pid = getPID();
    int typ=getTyp();
    HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
    HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    QString S2 = ui->lineEdit->text();
    int x = 0;
    double ValorScan = S2.toDouble();
    switch (typ)
    {
    case byBytes:
        Addres = NxtScan<byte>(ValorScan, phandle, Addres, x, pid, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;

    case by2Bytes:
        Addres = NxtScan<__int32>(ValorScan, phandle, Addres, x, pid, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;

    case by8Bytes:
        Addres = NxtScan<DWORD>(ValorScan, phandle, Addres, x, pid, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;

    case by4Bytes:
        Addres = NxtScan<int>(ValorScan, phandle, Addres, x, pid, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);
        break;

    case byFloat:
        Addres = NxtScan<float>(ValorScan, phandle, Addres, x, pid, typ);
        printAddress(Addres.size(), x, hModuleSnap, phandle);

    default:
        break;
    }

}

void MainWindow::on_pushButton_4_clicked()//clean
{
       ui->pushButton_4->hide();
       ui->pushButton_2->show();
       ui->comboBox->setDisabled(false);
       ui->lcdNumber->display(0);
       ui->tableWidget->clearContents();
       ui->tableWidget->setRowCount(0);
       printAdrTable.setRunning(false);
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)// перенос инфы в нижнию таблицу
{
    QTableWidgetItem *item = ui->tableWidget->item(row,0);
    QTableWidgetItem *item2 = ui->tableWidget->item(row,1);
    if (NULL != item && NULL != item2)
    {
        ui->tableWidget_2->insertRow( ui->tableWidget_2->rowCount() );
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,0, new QTableWidgetItem(item->text()));

        DWORD pid = getPID();
        HANDLE phandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
        int actualValue=1;
        QTableWidgetItem *item = ui->tableWidget->item(row,0);
        int_least64_t adr = item->text().toLongLong(nullptr,16);
        ReadProcessMemory(phandle, (LPVOID)adr, &actualValue, sizeof(actualValue), 0);
        ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,1, new QTableWidgetItem(QString::number(actualValue)));
    }
    //запуск потока
    startStream1();
}

//---------------------------------------------------------------------------------------------------------------------------------//

void MainWindow::update(int i,int j)// инфа из потока
{
    ui->tableWidget_2->setItem(j,1, new QTableWidgetItem(QString::number(i)));
}
QString AA="ноль";
void MainWindow::setAdr1(const QString& adr)// инфа из потока
{
    qDebug() << "infa = " << adr;
    AA=adr;
}
void MainWindow::setTableAdr(int row, int column)
{
    if (column==0)
    {
        if (AA!="ноль"){
        ui->tableWidget_2->setItem(row,column, new QTableWidgetItem(AA));
        }
    AA="ноль";
    }
    if (column==1)
    {
        DWORD pid= getPID();
        QTableWidgetItem *item2 = ui->tableWidget_2->item(row,0);
        int_least64_t value = item2->text().toLongLong(nullptr,16);
        int newValue = AA.toLongLong();
        DWORD protection = PAGE_EXECUTE_READWRITE;
        VirtualProtect( (LPVOID)value, sizeof(newValue), PAGE_EXECUTE_READWRITE, &protection );
        HANDLE pHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);
        WriteProcessMemory(pHandle, (LPVOID)value, &newValue, sizeof(newValue), 0);
        VirtualProtect( (LPVOID)value, sizeof(newValue), protection, &protection );
        AA="ноль";
    }
}

void MainWindow::on_tableWidget_2_cellDoubleClicked(int row, int column)
{

    changeAdr *change = new  changeAdr(0);
    QTableWidgetItem *item = ui->tableWidget_2->item(row,0);
    QString adr = item->text();
    QTableWidgetItem *item2 = ui->tableWidget_2->item(row,1);
    QString value = item2->text();
    connect(this, SIGNAL(sendAdrInfo(QString,QString,int,int)), change, SLOT(receiveAdrInfo(QString,QString,int,int)));
    emit sendAdrInfo(adr,value,column,row);

    changeAdr window;
    window.setModal(true);
    window.exec();
    setTableAdr(row,column);
    startStream1();
}

void MainWindow::on_pushButton_6_clicked()// ремув строки
{
    QModelIndexList selectedRows = ui->tableWidget_2->selectionModel()->selectedRows();
    if (!selectedRows.empty()) {
        ui->tableWidget_2->removeRow(selectedRows[0].row());
    }
    //перезапуск потока
    startStream1();

}
//*****************************************STYLE**********************************************
void MainWindow::changeStyle(const QString &styleName)

{
        QApplication::setStyle(QStyleFactory::create(styleName));
}


void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::StyleChange)
        styleChanged();
}

void MainWindow::styleChanged()
{
    auto styleName = QApplication::style()->objectName();
    for (int i = 0; i < ui->styleComboBox->count(); ++i) {
        if (QString::compare(ui->styleComboBox->itemText(i), styleName, Qt::CaseInsensitive) == 0) {
            ui->styleComboBox->setCurrentIndex(i);
             qApp->setPalette(style()->standardPalette());
             ui->checkBox->setCheckState(Qt::Checked);
            break;
        }
    }


}
void MainWindow::darkTheam()
{
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    qApp->setPalette(darkPalette);
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1==Qt::Unchecked)
    {
        darkTheam();

    }
    else
    {
            qApp->setPalette(style()->standardPalette());
    }
}
//*****************************************STYLE**********************************************





