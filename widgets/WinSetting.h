//
// Created by xmuli on 2020/12/25.
//

#ifndef LFXNET_WINSETTING_H
#define LFXNET_WINSETTING_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WinSetting; }
QT_END_NAMESPACE

class WinSetting : public QWidget {
Q_OBJECT

public:
    explicit WinSetting(QWidget *parent = nullptr);

    ~WinSetting() override;

private:
    Ui::WinSetting *ui;
};

#endif //LFXNET_WINSETTING_H
