#include <utility>

#include "generaloptionwidget.h"
#include "ui_generaloptionwidget.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPalette>

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

void GeneralOptionWidget::setKeySeq(QKeySequence keySeq)
{
    _keySeq = keySeq;
    ui->keKeySeq->setKeySequence(_keySeq);
}

const QKeySequence GeneralOptionWidget::getKeySeq()
{
    return _keySeq;
}

void GeneralOptionWidget::on_tbntOpenFileDialog_clicked()
{
    _path = QFileDialog::getOpenFileName(this, tr("Select IWYUTOOL"));
    ui->lnPath->setText(_path);
    _chekPath();
}

void GeneralOptionWidget::on_lnPath_editingFinished()
{
    _path = ui->lnPath->text();
    _chekPath();
}

void GeneralOptionWidget::_chekPath()
{
    if (!QFile::exists(_path)) {
        auto *palette = new QPalette();
        palette->setColor(QPalette::Text, Qt::red);
        ui->lnPath->setPalette(*palette);
    } else {
        auto *palette = new QPalette();
        palette->setColor(QPalette::Text, QApplication::palette().text().color());
        ui->lnPath->setPalette(*palette);
    }
}

void GeneralOptionWidget::on_keKeySeq_editingFinished()
{
    _keySeq = ui->keKeySeq->keySequence();
}
