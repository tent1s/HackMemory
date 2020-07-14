#include "changeadr.h"
#include "ui_changeadr.h"
#include <QDebug>
#include <windows.h>
#include "mainwindow.h"
QString A;
QString B;
int C;
changeAdr::changeAdr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changeAdr)
{
    ui->setupUi(this);
    QIcon winIcon("Icon.ico");
    this->setWindowIcon(winIcon);
    setWindowTitle(tr("Редактор"));
    setLineInfo();
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(getAdr1()));
}

changeAdr::~changeAdr()
{
    delete ui;
}
void changeAdr::receiveAdrInfo(QString adr,QString value,int column,int row)
{
    A = adr;
    B = value;
    C = column;
    this->row=row;
}

void changeAdr::getAdr1()
{
    MainWindow *fmr = new  MainWindow(0);
    QString adr = ui->lineEdit->text();
    fmr->setAdr1(adr);
 }
void changeAdr::setLineInfo()
{
    if(C==0)
    {
        ui->lineEdit->setText(A);
    }
    else
    {
        ui->lineEdit->setText(B);
    }
}

