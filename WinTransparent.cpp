#include "WinTransparent.h"

#include <QMouseEvent>
#include <QDebug>
#include <QBoxLayout>

WinTransparent::WinTransparent(QWidget *parent)
    : QWidget(parent)
    , m_posStart(QPoint(0, 0))
    , m_isMoveWindow(false)
{
    init();
}

void WinTransparent::init()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setWindowOpacity(0.5);
    resize(400, 600);
//    setAttribute(Qt::WA_TranslucentBackground);
}

void WinTransparent::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isMoveWindow = true;
        m_posStart = event->pos();
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
