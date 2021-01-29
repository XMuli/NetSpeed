#include "ProgressLabel.h"

#include <QPainter>
#include <QDebug>

ProgressLabel::ProgressLabel()
    : m_val(0)
    , m_all(100)
    , m_borderWidth(2)
    , m_opacity(0)  // 暂时未设置
    , m_borderColor(Qt::black)
    , m_contentColor(Qt::black)
    , m_contentFreeColor(Qt::red)
    , m_ptr(nullptr)
{
}

void ProgressLabel::winDestktopPtr(WinDesktop *ptr)
{
    m_ptr = ptr;
    connect(m_ptr, &WinDesktop::sigValChange, this, &ProgressLabel::onupdate);
}

void ProgressLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    const int radius = 8;
    QPainter pa(this);
    pa.setPen(QPen(m_borderColor, m_borderWidth));
    pa.setBrush(Qt::NoBrush);
    pa.drawRoundedRect(rect(), radius, radius);

    QRect contect(rect().topLeft(), QPoint(rect().width() * m_val  / m_all, rect().height()));
//    QRect contectFree(rect().left() + rect().width() * m_val / m_all, rect().top(), rect().width() * m_val * 1.0 / m_all, rect().height());
    pa.setBrush(m_contentFreeColor);
    pa.setPen(Qt::NoPen);
    pa.drawRoundedRect(contect.adjusted(m_borderWidth, m_borderWidth, -m_borderWidth, -m_borderWidth), radius, radius);
//    pa.drawRoundedRect(contectFree, radius, radius);
}

void ProgressLabel::onupdate()
{
//    static int i = 0;
//    qDebug() <<"ProgressLabel================================>"<< i++ << m_val << m_all;
//    if (i >= 16)
//        i = 0;

    update();
}
