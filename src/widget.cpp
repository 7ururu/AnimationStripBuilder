#include "widget.h"
#include "ui_widget.h"
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    QDesktopWidget desktop;
    move((desktop.width() - width()) / 2, (desktop.height() - height()) / 2);

    nFrames = 0;
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::makeNextName(const QString& name) const
{
    QString res = name;
    for (int i = name.size() - 1; i >= 0; i--)
        if (name[i].isDigit())
        {
            res[i] = QChar(name[i].cell() + 1);
            if (res[i].isDigit())
                break;
            else
                res[i] = QChar('0');
        }
    if (res < name)
    {
        res = "";
        qDebug("All digits in file name became zeroes");
    }
    qDebug(res.toStdString().c_str());
    return res;
}

void Widget::on_pushButtonCreate_released()
{
    QString firstFrame = ui->lineEditFirstFrame->text(),
            result = ui->lineEditResult->text();
    int nRows = ui->lineEditNRows->text().toInt(),
        nColumns = ui->lineEditNColumns->text().toInt();

    QString file = QFileInfo(firstFrame).fileName(),
            path = QFileInfo(firstFrame).absolutePath();
    QPixmap frame(firstFrame);
    QPixmap resultPixmap(frame.width() * nColumns, frame.height() * nRows);
    int currentRow = 0,
        currentColumn = 0;
    QPainter painter(&resultPixmap);

    int frames = nFrames;
    while (frames >= 0)
    {
        if (currentRow == 0 && currentColumn == 0)
        {
            if (firstFrame != path + "/" + file)
            {
                resultPixmap.save(result);
                result = QFileInfo(result).absolutePath() + "/" + makeNextName(QFileInfo(result).fileName());
            }
            resultPixmap.fill(Qt::white);
        }

        if (frames == 0)
            break;

        frames--;

        painter.drawPixmap(currentColumn * frame.width(), currentRow * frame.height(), frame.width(), frame.height(), frame);
        file = makeNextName(file);
        frame = QPixmap(path + "/" + file);
        if (ui->radioButtonColsRows->isChecked())
        {
            currentColumn++;
            if (currentColumn == nColumns)
            {
                currentColumn = 0;
                currentRow++;
                if (currentRow == nRows)
                    currentRow = 0;
            }
        }
        else
        {
            currentRow++;
            if (currentRow == nRows)
            {
                currentRow = 0;
                currentColumn++;
                if (currentColumn == nColumns)
                    currentColumn = 0;
            }
        }
    }
}

void Widget::on_toolButtonSetFirstFrame_released()
{
    static QString path = "";
    QString s = QFileDialog::getOpenFileName(this, "Set first frame", path,
                                             "Images (*.png *.jpg *.bmp *.jpeg *.ppm *.xbm *.xpm)");
    if (s.size() > 0)
    {
        path = s;
        ui->lineEditFirstFrame->setText(path);

        QString file = QFileInfo(s).fileName(),
                path = QFileInfo(s).absolutePath();
        qDebug(path.toStdString().c_str());

        nFrames = 0;
        while (true)
        {
            QPixmap frame(path + "/" + file);
            if (frame.isNull())
            {
                break;
            }
            nFrames++;
            file = makeNextName(file);
        }
        if (nFrames == 0)
        {
            qDebug("Can't find frames!");
            return;
        }

        if (ui->lineEditNRows->text().size() == 0 && ui->lineEditNColumns->text().size() == 0)
        {
            int nRows = 1,
                nColumns = nFrames;
            for (int rows = 2; rows * rows <= nFrames; rows++)
            {
                if (nFrames % rows == 0 && rows <= nFrames / rows)
                {
                    nRows = rows;
                    nColumns = nFrames / rows;
                }
            }
            ui->lineEditNRows->setText(QString::number(nRows));
            ui->lineEditNColumns->setText(QString::number(nColumns));
        }
    }
}

void Widget::on_toolButtonSetResult_released()
{
    static QString fileName = "";
    QString s = QFileDialog::getSaveFileName(this, "Set result", fileName,
                                             "Images (*.png *.jpg *.bmp *.jpeg *.ppm *.xbm *.xpm)");
    if (s.size() > 0)
    {
        fileName = s;
        ui->lineEditResult->setText(fileName);
    }
}
