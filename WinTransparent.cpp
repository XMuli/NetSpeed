#include "WinTransparent.h"

#include <QMouseEvent>
#include <QDebug>
#include <QBoxLayout>
#include <QGraphicsEffect>
#include <QGraphicsBlurEffect>

WinTransparent::WinTransparent(QWidget *parent)
    : QWidget(parent)
    , m_posStart(QPoint(0, 0))
    , m_isMoveWindow(false)
{
    init();

}

void WinTransparent::init()
{
    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setWindowOpacity(1); // 透明度
//    resize(400, 600);
//    setAttribute(Qt::WA_TranslucentBackground);

//    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
//    blur->setBlurRadius(0.5);
//    blur->setBlurHints(QGraphicsBlurEffect::QualityHint);
//    setGraphicsEffect(blur);
}

void WinTransparent::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMoveWindow = true;
        m_posStart = event->pos();
    } else if (event->button() == Qt::MiddleButton) {
        close();
    }
}

void WinTransparent::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMoveWindow)
        move(event->pos() - m_posStart + pos());
}

void WinTransparent::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_isMoveWindow = false;
}
