#ifndef GENERALOPTIONWIDGET_H
#define GENERALOPTIONWIDGET_H

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

private slots:
    void on_tbntOpenFileDialog_clicked();

private:
    Ui::GeneralOptionWidget *ui;
    QString _path;
};

#endif // GENERALOPTIONWIDGET_H
