#ifndef GENERALOPTIONWIDGET_H
#define GENERALOPTIONWIDGET_H

#include <QKeySequence>
#include <QUrl>
#include <QWidget>

namespace Ui {
class GeneralOptionWidget;
}

class GeneralOptionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralOptionWidget(QWidget *parent = nullptr);
    ~GeneralOptionWidget();
    const QUrl getPath() { return _path; };
    void setPath(QString path);
    void setKeySeq(QKeySequence keySeq);
    const QKeySequence getKeySeq();

private slots:
    void on_tbntOpenFileDialog_clicked();
    void on_lnPath_editingFinished();

    void on_keKeySeq_editingFinished();

private:
    Ui::GeneralOptionWidget *ui;
    QString _path;
    QKeySequence _keySeq;

    void _chekPath();
};

#endif // GENERALOPTIONWIDGET_H
