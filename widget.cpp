#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //虽然上面一行代码进行widget和ui的窗口关联，但是如果发生窗口大小变化的时候，里面的布局不会随之变化
    //下面代码实现弹性布局
    this->setLayout(ui->verticalLayout);
    ui->widgetBottom->setLayout(ui->horizontalLayout);

    QShortcut* shortcutOpen = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    QShortcut* shortcutSave = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Save")),this);
    QShortcut* shortcutZoomIn = new QShortcut(QKeySequence(tr("Ctrl+SHIFT+=", "File|ZoomIn")),this);
    QShortcut* shortcutZoomOut = new QShortcut(QKeySequence(tr("Ctrl+SHIFT+-", "File|ZoomOut")),this);

    connect(shortcutOpen, &QShortcut::activated, [=](){
        on_dakai_clicked();
    });

    connect(shortcutSave, &QShortcut::activated, [=](){
        on_baocun_clicked();
    });

    connect(shortcutZoomIn, &QShortcut::activated, [=](){
        ZoomIn();
    });

    connect(shortcutZoomOut, &QShortcut::activated, [=](){
        ZoomOut();
    });

    //在构造函数中进行信号与槽的绑定
    //指针引用
    //QObject::connect(ui->pushButton_3, &QPushButton::clicked, this, &Widget::on_pushButton_3);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
    connect(ui->shurukuang, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
}

Widget::~Widget()
{
    delete ui;
}

//放大字体
void Widget::ZoomIn()
{
    //获得当前字体信息
    QFont font = ui->shurukuang->font();
    //获得当前字体大小
    int fontSize = font.pointSize();
    if(fontSize == -1) return;
    //改变大小，并设置字体大小
    int newFontSize = fontSize + 1;
    font.setPointSize(newFontSize);
    ui->shurukuang->setFont(font);
}

//缩小字体
void Widget::ZoomOut()
{
    //获得当前字体信息
    QFont font = ui->shurukuang->font();
    //获得当前字体大小
    int fontSize = font.pointSize();
    if(fontSize == -1) return;
    //改变大小，并设置字体大小
    int newFontSize = fontSize - 1;
    font.setPointSize(newFontSize);
    ui->shurukuang->setFont(font);
}

//打开文件
void Widget::on_dakai_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "D:/QtCode", tr("Text (*.txt)"));

    ui->shurukuang->clear();//清除第一次文件内容
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug() << "file open error";
    }
    this->setWindowTitle(fileName + "-记事本");
    //以utf-8编码读取文件
    QTextStream in(&file);
    //in.setCodec("UTF-8");
    QString str = ui->comboBox->currentText();
    const char* c_str = str.toStdString().c_str();
    in.setCodec(c_str);
    while(!in.atEnd()){
        QString context = in.readLine();
        //qDebug() << qPrintable(context);
        //ui->shurukuang->setText(context);//只显示最后一行
        ui->shurukuang->append(context);
    }
}

//保存文件
void Widget::on_baocun_clicked()
{
     if(!file.isOpen()){
         QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "D:/QtCode/untitled.txt", tr("Text (*.txt *.doc)"));

         file.setFileName(fileName);
         if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
             qDebug() << "file save error";
         }
         this->setWindowTitle(fileName + "-记事本");
     }
     QTextStream out(&file);
     out.setCodec(ui->comboBox->currentText().toStdString().c_str());
     QString context = ui->shurukuang->toPlainText();
     out << context;//通过out方式写入文件
}


//窗口关闭
void Widget::on_guanbi_clicked()
{
    ui->shurukuang->clear();
    if(file.isOpen()){
        file.close();
        this->setWindowTitle("记事本");
    }

    //close();
}

//处理乱码问题
void Widget::onCurrentIndexChanged(int index)
{
    ui->shurukuang->clear();
    if(file.isOpen()){
        QTextStream in(&file);
        in.setCodec(ui->comboBox->currentText().toStdString().c_str());
        //重新把光标回到开头，因为on_dakai_clicked中轮询时，光标已经读到了最后
        file.seek(0);
        while(!in.atEnd()){
            QString context = in.readLine();
            ui->shurukuang->append(context);
        }
    }
}

//解决行列不改变
void Widget::onCursorPositionChanged()
{
    QTextCursor cursor = ui->shurukuang->textCursor();
    //qDebug() << cursor.blockNumber() + 1<< "," << cursor.columnNumber() + 1;
    QString blockNum = QString::number(cursor.blockNumber() + 1);
    QString columnNum = QString::number(cursor.columnNumber() + 1);
    const QString labelMes = "行:" + blockNum + ",列:" + columnNum;
    ui->hanglie->setText(labelMes);
    //设置当前行高亮
    QList<QTextEdit::ExtraSelection>extraSelections;
    QTextEdit::ExtraSelection ext;
    //1、知道当前行
    ext.cursor = ui->shurukuang->textCursor();
    //2、颜色
    QBrush qBrush(Qt::lightGray);
    ext.format.setBackground(qBrush);
    ext.format.setProperty(QTextFormat::FullWidthSelection, true);
    //3、设置
    extraSelections.append(ext);
    ui->shurukuang->setExtraSelections(extraSelections);
}
