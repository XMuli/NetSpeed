//
// Created by xmuli on 2020/12/25.
//

#include "NetPlugin.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

DWIDGET_USE_NAMESPACE

NetPlugin::NetPlugin(QObject *parent)
    : QObject(parent)
    , m_winSetting(nullptr)
    , m_winDockNet(nullptr)
    , m_proxyInter(nullptr)
{
}

/*!
 * \brief NetPlugin::pluginName 返回插件名称
 * \return 插件名（后会更正 MonitorNet）
 * \note 解决压缩 Bug，在 dde-dock 1.2.3 被修复，故名称叫 “datetime”
 * \see 参见 https://xmuli.tech/posts/c225b552/ 文末缺陷分析
 */
const QString NetPlugin::pluginName() const
{
    return "datetime";
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

/*!
 * \brief NetPlugin::pluginIsAllowDisable 插件是否允许被禁用
 * \return 告诉 dde-dock 本插件允许禁用
 */
bool NetPlugin::pluginIsAllowDisable()
{
    return true;
}

/*!
 * \brief NetPlugin::pluginIsDisable 插件是否被禁用
 * \return 插件是否被禁用的状态
 * \note getValue() 的第二个参数 “disabled” 表示存储这个值的键（所有配置都是以键值对的方式存储的）
 *       第三个参数表示默认值，即默认不禁用
 */
bool NetPlugin::pluginIsDisable()
{
    return m_proxyInter->getValue(this, "disabled", false).toBool();
}

/*!
 * \brief NetPlugin::pluginStateSwitched 插件状态切换
 */
void NetPlugin::pluginStateSwitched()
{
    // 将旧的 "禁用状态" 数值取反后,保存到 key-val 中
    const bool disableState = !pluginIsDisable();
    m_proxyInter->saveValue(this, "disabled", disableState);

    // 根据新的禁用状态值,处理主控的加载与卸载
    if (disableState)
        m_proxyInter->itemRemoved(this, pluginName());
    else
        m_proxyInter->itemAdded(this, pluginName());
}

/*!
 * \brief NetPlugin::pluginDisplayName 插件在 dock 的右键时候，显示名称
 * \return 插件在 dock 右键显示的预称
 */
const QString NetPlugin::pluginDisplayName() const
{
    return QString("MonitorNet");
}

/*!
 * \brief NetPlugin::itemContextMenu 插件右键后现实的菜单列表
 * \param itemKey
 * \return 返回 JSON 格式的菜单数据
 * \note 增加右键菜单功能需要实现此两个接口：itemContextMenu + invokedMenuItem
 */
const QString NetPlugin::itemContextMenu(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    QMap<QString, QVariant> update;
    update["itemId"] = "update";
    update["itemText"] = "刷新";
    update["isActive"] = true;

    QMap<QString, QVariant> setting;
    setting["itemId"] = "setting";
    setting["itemText"] = "设置";
    setting["isActive"] = true;

    QList<QVariant> items;
    items.reserve(2);
    items.push_back(update);
    items.push_back(setting);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    return QJsonDocument::fromVariant(menu).toJson();
}

/*!
 * \brief NetPlugin::invokedMenuItem 菜单项被点击后的回调函数
 * \param itemKey
 * \param menuId  menu item ID
 * \param checked
 * \note * \note 增加右键菜单功能需要实现此两个接口：itemContextMenu + invokedMenuItem
 */
void NetPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(itemKey)
    Q_UNUSED(checked)

    if (menuId == "update") {
        m_proxyInter->itemRemoved(this, pluginName());
        m_proxyInter->itemAdded(this, pluginName());
    } else if (menuId == "setting") {
        m_winSetting = new WinDdeDockSetting();
        m_winSetting->move((QApplication::desktop()->width() - m_winSetting->width())/2,(QApplication::desktop()->height() - m_winSetting->height())/2);
        m_winSetting->show();
    }
}
