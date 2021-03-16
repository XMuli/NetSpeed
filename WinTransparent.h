#ifndef WINTRANSPARENT_H
#define WINTRANSPARENT_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QGraphicsBlurEffect>

class WinTransparent : public QWidget
{
    Q_OBJECT
public:
    explicit WinTransparent(QWidget *parent = nullptr);

    void init();

signals:
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
//    virtual void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

private:
    QPoint m_posStart;
    bool m_isMoveWindow;
};

#endif // WINTRANSPARENT_H
