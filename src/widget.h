#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButtonCreate_released();
    void on_toolButtonSetFirstFrame_released();
    void on_toolButtonSetResult_released();

private:
    Ui::Widget *ui;
    QString makeNextName(const QString& name) const;

    int nFrames;
};

#endif // WIDGET_H
