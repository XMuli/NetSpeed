#ifndef PROGRESSLABEL_H
#define PROGRESSLABEL_H
#include <QLabel>
#include "WinDesktop.h"

class ProgressLabel : public QLabel
{
public:
    ProgressLabel();
    void winDestktopPtr(WinDesktop *ptr);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

public slots:
    void onupdate();

public:
    int m_val;
    int m_all;
    double m_opacity;
    int m_borderWidth;
    QColor m_borderColor;
    QColor m_contentColor;
    QColor m_contentFreeColor;

private:
    WinDesktop *m_ptr;
};

#endif // PROGRESSLABEL_H
