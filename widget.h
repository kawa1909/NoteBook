#ifndef WIDGET_H
#define WIDGET_H
#include "ui_widget.h"
#include <QWidget>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QComboBox>
#include <QShortcut>

QT_BEGIN_NAMESPACE//支持命名空间的宏定义
namespace Ui { class Widget; }
QT_END_NAMESPACE//配对使用，标志着命名空间的结束

class Widget : public QWidget
{
    Q_OBJECT//qt框架中非常重要的宏，启用qt对象的元对象系统

public:
    QFile file;
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void ZoomIn();
    void ZoomOut();

signals:


private slots:
    void on_guanbi_clicked();

    void on_dakai_clicked();

    void on_baocun_clicked();

    void onCurrentIndexChanged(int index);

    void onCursorPositionChanged();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
