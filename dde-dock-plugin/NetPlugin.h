//
// Created by xmuli on 2020/12/25.
//

#ifndef LFXNET_NETPLUGIN_H
#define LFXNET_NETPLUGIN_H

#include <QObject>
#include <pluginsiteminterface.h>
#include <dtkwidget_global.h>

DWIDGET_USE_NAMESPACE

class NetPlugin : public QObject, public PluginsItemInterface
{
    Q_OBJECT

};

#endif //LFXNET_NETPLUGIN_H
