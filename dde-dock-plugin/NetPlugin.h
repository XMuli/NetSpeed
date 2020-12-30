//
// Created by xmuli on 2020/12/25.
//

#ifndef LFXNET_NETPLUGIN_H
#define LFXNET_NETPLUGIN_H

#include <QObject>
#include <pluginsiteminterface.h>
#include <dtkwidget_global.h>
#include "../widgets/WinDockNet.h"

DWIDGET_USE_NAMESPACE

class QLabel;
class NetPlugin : public QObject, public PluginsItemInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginsItemInterface)  // 声明实现了的接口
    Q_PLUGIN_METADATA(IID "com.deepin.dock.PluginsItemInterface" FILE "MonitorNet.json")  // 插件元数据

public:
    explicit NetPlugin(QObject *parent = nullptr);

    // 插件必须重写函数
    virtual const QString pluginName() const override;
    virtual void init(PluginProxyInterface *proxyInter) override;
    virtual QWidget *itemWidget(const QString &itemKey) override;

private:
    QLabel *m_labTest;
    PluginProxyInterface *m_proxyInter;
};

#endif //LFXNET_NETPLUGIN_H
