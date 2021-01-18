//
// Created by xmuli on 2020/12/25.
//

#ifndef LFXNET_NETPLUGIN_H
#define LFXNET_NETPLUGIN_H

#include <QObject>
#include <pluginsiteminterface.h>
#include <dtkwidget_global.h>

#include "WinDockNet.h"
#include "WinDdeDockSetting.h"

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

    // 插件禁用和启用相关的接口
    virtual bool pluginIsAllowDisable() override;
    virtual bool pluginIsDisable() override;
    virtual void pluginStateSwitched() override;

    // 其它额外接口
    virtual const QString pluginDisplayName() const override;
    virtual const QString itemContextMenu(const QString &itemKey) override;
    virtual void invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked) override;
    virtual QWidget *itemTipsWidget(const QString &itemKey) override;
    virtual QWidget *itemPopupApplet(const QString &itemKey) override;
    virtual bool itemAllowContainer(const QString &itemKey) override;

public slots:
    void onHoverDisplay();

private:
    WinDdeDockSetting *m_winSetting;
    WinDockNet *m_winDockNet;
    PluginProxyInterface *m_proxyInter;
    QLabel *m_isHoverDisplay;
    QTimer *m_timer;
};

#endif //LFXNET_NETPLUGIN_H
