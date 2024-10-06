#include "mytextedit.h"

#include <QWheelEvent>
#include <QDebug>

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void MyTextEdit::wheelEvent(QWheelEvent *e)
{
    qDebug() << e->angleDelta().y();
    if(ctrlKeyPressed == 1){
        if(e->angleDelta().y() > 0){
            zoomIn();
        }else if(e->angleDelta().y() < 0){
            zoomOut();
        }
        e->accept();
    }else{
        QTextEdit::wheelEvent(e);
    }

}

void MyTextEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        qDebug() << "ctrl按下";
        ctrlKeyPressed = 1;
    }
    QTextEdit::keyPressEvent(e);
}

void MyTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Control){
        qDebug() << "ctrl松开";
        ctrlKeyPressed = 0;
    }
    QTextEdit::keyReleaseEvent(e);
}
