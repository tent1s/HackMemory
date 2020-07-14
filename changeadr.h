#ifndef CHANGEADR_H
#define CHANGEADR_H

#include <QDialog>
#include <QString>
namespace Ui {
class changeAdr;
}

class changeAdr :  public QDialog
{
    Q_OBJECT

public:
    Ui::changeAdr *ui;
    explicit changeAdr(QWidget *parent = nullptr);
    ~changeAdr();
private slots:
    void getAdr1();
    void receiveAdrInfo(QString adr,QString value,int column,int row);
    void setLineInfo();
private:
    QString adr;
    QString value;
    int column;
    int row;
};

#endif // CHANGEADR_H
