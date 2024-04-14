#ifndef FLOATBUTTONFILTER_H
#define FLOATBUTTONFILTER_H

#include <QObject>
#include <QPushButton>
#include <QMouseEvent>
class FloatButtonFilter : public QObject
{
    Q_OBJECT
public:
    explicit FloatButtonFilter(QPushButton *button, QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override ;

private:
    //floatin button
    bool m_dragging;
    QPoint m_dragStartposition;
    QPushButton *m_button;


};

#endif // FLOATBUTTONFILTER_H
