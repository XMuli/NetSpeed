//
// Created by xmuli on 2020/12/25.
//

#ifndef LFXNET_WINABOUT_H
#define LFXNET_WINABOUT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WinAbout; }
QT_END_NAMESPACE

class WinAbout : public QWidget {
Q_OBJECT

public:
    explicit WinAbout(QWidget *parent = nullptr);

    ~WinAbout() override;

private:
    Ui::WinAbout *ui;
};

#endif //LFXNET_WINABOUT_H
