#include "SettingModel.h"

/*!
 * \brief SettingModel::SettingModel 模型类，用来中转信号再传递的；
 * \note 作用：WinDdeDockSetting 与 WinDockNet 两个窗口的子控件之间互相操作
 *            前者子控件发出信号，然后传递到本类的新信号，再由本类信号专递给后者类的槽函数
 */
SettingModel::SettingModel()
{

}
