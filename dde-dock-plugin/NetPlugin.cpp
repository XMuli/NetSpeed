//
// Created by xmuli on 2020/12/25.
//

#include "NetPlugin.h"
#include <QLabel>
#include <QDebug>

DWIDGET_USE_NAMESPACE

NetPlugin::NetPlugin(QObject *parent)
    : QObject(parent)
    , m_winDockNet(nullptr)
{
}

/*!
 * \brief NetPlugin::pluginName 返回插件名称
 * \return 插件名（后会更正 MonitorNet）
 * \note 解决压缩 Bug，在 dde-dock 1.2.3 被修复，故名称叫 “datetime”
 * \see 参见文末 https://xmuli.tech/posts/c225b552/ 缺陷分析
 */
const QString NetPlugin::pluginName() const
{
    return "datetime"; // MonitorNet
}

/*!
 * \brief NetPlugin::init 插件初始化入口函数
 * \param proxyInter 可认为是主程序的进程
 */
void NetPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;
    m_winDockNet = new WinDockNet();

    if (!pluginIsDisable())
        m_proxyInter->itemAdded(this, pluginName());
}

/*!
 * \brief NetPlugin::itemWidget 返回插件主控件,用于显示在 dde-dock 面板上
 * \param itemKey
 * \return
 */
QWidget *NetPlugin::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey)
    return m_winDockNet;
}

const QString NetPlugin::itemContextMenu(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    QList<QVariant> items;
    items.reserve(1);

    QMap<QString, QVariant> update;
    update["itemId"] = "update";
    update["itemText"] = "刷新";
    update["isActive"] = true;
    items.push_back(update);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    // 返回 JSON 格式的菜单数据
    return QJsonDocument::fromVariant(menu).toJson();
}
