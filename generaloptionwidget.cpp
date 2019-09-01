#include <utility>

#include "generaloptionwidget.h"
#include "ui_generaloptionwidget.h"
#include <QFileDialog>

GeneralOptionWidget::GeneralOptionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralOptionWidget)
{
    ui->setupUi(this);
}

GeneralOptionWidget::~GeneralOptionWidget()
{
    delete ui;
}

void GeneralOptionWidget::setPath(QString path)
{
    _path = path;
    ui->lnPath->setText(_path);
}

void GeneralOptionWidget::on_tbntOpenFileDialog_clicked()
{
    _path = QFileDialog::getOpenFileName(this, tr("Select IWYUTOOL"));
    ui->lnPath->setText(_path);
}
