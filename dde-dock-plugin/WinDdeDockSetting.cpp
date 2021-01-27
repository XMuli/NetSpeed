#include "WinDdeDockSetting.h"
#include "ui_WinDdeDockSetting.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <QColorDialog>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QStyleFactory>
#include <QApplication>
#include <DGuiApplicationHelper>
#include <QStandardPaths>
#include <QString>
#include <QString>
using namespace std;
DGUI_USE_NAMESPACE

WinDdeDockSetting::WinDdeDockSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WinDdeDockSetting)
    , m_isHorizontal(true)
    , m_path("")
    , m_btnGroupTheme(new QButtonGroup(nullptr))
    , m_doubleClick(2)
    , m_cpuOverNum(0)
    , m_memOverNum(0)
    , m_NetOverNum(0)
    , m_netOverUnit("MB")
{
    ui->setupUi(this);
    init();
}

WinDdeDockSetting::~WinDdeDockSetting()
{
    delete ui;
}

void WinDdeDockSetting::init()
{
    initSigConnectWinDdeDock();
    initSigConnectWinMain();

    // 读入 json 文件到流中
    readConfig();

    // 控件的基本设置，其读写留其它函数完成
//    auto list = this->findChildren<QAbstractSpinBox *>();  // 切换为上下按钮模式
//    for (auto v : list)
//        v->setProperty("_d_dtk_spinBox", true);

    ui->groupBoxShowModel->setEnabled(false);

    ui->labLabTextColor->setAutoFillBackground(true);
    ui->labTextColor->setAutoFillBackground(true);
    ui->labBackgroundColor->setAutoFillBackground(true);

    ui->spinBoxFractionalAccuracy->setRange(0, 100);
    ui->spinBoxFractionalAccuracy->setSingleStep(1);
    ui->spinBoxRefreshInterval->setRange(1000, 2147483647);
    ui->spinBoxRefreshInterval->setSingleStep(1000);
    ui->spinBoxRefreshInterval->setSuffix(tr("ms"));

    ui->labLabTextColor->installEventFilter(this);
    ui->labTextColor->installEventFilter(this);
    ui->labBackgroundColor->installEventFilter(this);

    m_btnGroupTheme->setParent(ui->groupBoxThemeStyle);
    m_btnGroupTheme->addButton(ui->radioButtonSystem);
    m_btnGroupTheme->addButton(ui->radioButtonLight);
    m_btnGroupTheme->addButton(ui->radioButtonDark);
    ui->comboBoxStyle->addItems(QStyleFactory::keys());

    setWindowTitle(QString("lfxMonitorNet"));
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

void WinDdeDockSetting::initSigConnectWinDdeDock()
{
    connect(ui->btnSaveWinDdeDock, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnApplyWinDdeDock);
    connect(ui->btnQuitWinDdeDock, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnQuitWinDdeDock);

    connect(ui->fontComboBox, &QFontComboBox::currentTextChanged, this, &WinDdeDockSetting::sigCurrentFont);
    void (QSpinBox::*pFun)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBoxFontSize, pFun, this, &WinDdeDockSetting::sigFontSize);

    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinDdeDockSetting::sigShowModel);
    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinDdeDockSetting::updateLabelText);
    connect(ui->comboBoxUnitModel, &QComboBox::currentTextChanged, this, &WinDdeDockSetting::sigUnitModel);
    connect(ui->lineLabUpload, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabUploadText);
    connect(ui->lineLabDown, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabDownText);
    connect(ui->lineLabCpu, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabCpuText);
    connect(ui->lineLabMemory, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabMemoryText);
    connect(ui->lineLabDiskRead, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabDiskReadText);
    connect(ui->lineLabDiskWrite, &QLineEdit::textChanged, this, &WinDdeDockSetting::sigLabDiskWriteText);
    connect(ui->checkBoxDisolayNet, &QCheckBox::clicked, this, &WinDdeDockSetting::sigDisolayNet);
    connect(ui->checkBoxDisolayCPUAndMemory, &QCheckBox::clicked, this, &WinDdeDockSetting::sigDisolayCPUAndMemory);
    connect(ui->checkBoxDisolayDisk, &QCheckBox::clicked, this, &WinDdeDockSetting::sigDisolayDisk);
    connect(ui->checkBoxLocationExchangeNet, &QCheckBox::clicked, this, &WinDdeDockSetting::sigLocationExchangeNet);
    connect(ui->checkBoxLocationExchangeCPUAndMenory, &QCheckBox::clicked, this, &WinDdeDockSetting::sigLocationExchangeCPUAndMenory);
    connect(ui->checkBoxLocationExchangeDisk, &QCheckBox::clicked, this, &WinDdeDockSetting::sigLocationExchangeDisk);

    connect(ui->spinBoxFractionalAccuracy, pFun, this, &WinDdeDockSetting::sigFractionalAccuracy);
    void (QComboBox::*pFunDoubleClick)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBoxDoubleClick, pFunDoubleClick, this, &WinDdeDockSetting::onDoubleClick);
    connect(ui->spinBoxRefreshInterval, pFun, this, &WinDdeDockSetting::sigRefreshInterval);
    connect(ui->checkBoxHoverDisplay, &QCheckBox::clicked, this, &WinDdeDockSetting::sigHoverDisplay);
}

void WinDdeDockSetting::initSigConnectWinMain()
{
    connect(ui->btnApplyWinMain, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnApplyWinMain);
    connect(ui->btnQuitWinMain, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnQuitWinMain);

    connect(ui->checkBoxBootUpUpdate, &QCheckBox::clicked, this, &WinDdeDockSetting::onBootUpUpdate);
    connect(ui->btnExportData, &QPushButton::clicked, this, &WinDdeDockSetting::onChangePath);

    connect(ui->checkBoxCpuOver, &QCheckBox::clicked, this, &WinDdeDockSetting::sigCpuOver);
    connect(ui->checkBoxMemOver, &QCheckBox::clicked, this, &WinDdeDockSetting::sigMemOver);
    connect(ui->checkBoxNetOver, &QCheckBox::clicked, this, &WinDdeDockSetting::sigNetOver);
    void (QSpinBox::*pFun)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBoxCpuOverNum, pFun, this, &WinDdeDockSetting::sigCpuOverNum);
    connect(ui->spinBoxMemOverNum, pFun, this, &WinDdeDockSetting::sigMemOverNum);
    connect(ui->spinBoxNetOverNum, pFun, this, &WinDdeDockSetting::sigNetOverNum);
    connect(ui->comboBoxNetNumUnit, &QComboBox::currentTextChanged, this, &WinDdeDockSetting::sigNetNumUnit);

    void (QButtonGroup::*pFunTheme)(int, bool) = &QButtonGroup::buttonToggled;
    connect(m_btnGroupTheme, pFunTheme, this, &WinDdeDockSetting::onBtnGroupTheme);
    void (QComboBox::*pFuncomboBoxStyle)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBoxStyle, pFuncomboBoxStyle, this, &WinDdeDockSetting::onStyle);
    connect(ui->btnApplyWinMain, &QPushButton::pressed, this, &WinDdeDockSetting::sigBtnApplyWinMain);
}

/*!
 * \brief WinDdeDockSetting::readConfigWinDdeDock 从 config.json 读取 config.json 写入到 UI 控件显示
 */
void WinDdeDockSetting::readConfigWinDdeDock()
{
    json jsColorAndFont = m_js["WinDdeDock"]["ColorAndFont"];
    ui->fontComboBox->setCurrentIndex(jsColorAndFont["FontTypeIndex"]);
    ui->spinBoxFontSize->setValue(jsColorAndFont["FontSize"]);
    QPalette palette;
    palette.setColor(QPalette::Background, QString::fromStdString(jsColorAndFont["LabTextColor"]));
    ui->labLabTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jsColorAndFont["TextColor"]));
    ui->labTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jsColorAndFont["BackgroundColor"]));
    ui->labBackgroundColor->setPalette(palette);

    json jsModelSetting = m_js["WinDdeDock"]["ModelSetting"];
    ui->comboBoxUnitModel->setCurrentIndex(jsModelSetting["UnitModelIndex"]);
    if (jsModelSetting["IsHorizontal"]) {
        ui->radioHorizontal->setChecked(true);
        ui->radioVertical->setChecked(false);
    } else {
        ui->radioHorizontal->setChecked(false);
        ui->radioVertical->setChecked(true);
    }

    json jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
    if (jsModelSetting["IsHorizontal"]) {
        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[0]["LabUpload"]));
        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[0]["LabDown"]));
        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[0]["LabCpu"]));
        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[0]["LabMemory"]));
        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[0]["LabDiskRead"]));
        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[0]["LabDiskWrite"]));
    } else {
        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[1]["LabUpload"]));
        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[1]["LabDown"]));
        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[1]["LabCpu"]));
        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[1]["LabMemory"]));
        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[1]["LabDiskRead"]));
        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[1]["LabDiskWrite"]));
    }

    ui->checkBoxDisolayNet->setChecked(jsDisplayText[2]["DisolayNet"]);
    ui->checkBoxDisolayCPUAndMemory->setChecked(jsDisplayText[2]["DisolayCPUAndMemory"]);
    ui->checkBoxDisolayDisk->setChecked(jsDisplayText[2]["DisolayDisk"]);
    ui->checkBoxLocationExchangeNet->setChecked(jsDisplayText[3]["LocationExchangeNet"]);
    ui->checkBoxLocationExchangeCPUAndMenory->setChecked(jsDisplayText[3]["LocationExchangeCPUAndMenory"]);
    ui->checkBoxLocationExchangeDisk->setChecked(jsDisplayText[3]["LocationExchangeDisk"]);
    ui->spinBoxFractionalAccuracy->setValue(jsDisplayText[4]["FractionalAccuracy"]);
    ui->spinBoxRefreshInterval->setValue(jsDisplayText[4]["RefreshInterval"]);

    json jsDockWindow = m_js["WinDdeDock"]["DockWindow"];
    ui->checkBoxHoverDisplay->setChecked(jsDockWindow["HoverDisplay"]);
    ui->comboBoxDoubleClick->setCurrentIndex(jsDockWindow["DoubleClickIndex"]);

    // TODO: 2021-01-07 占用图模式未写

    // 第一次发射信号，加载配置文件
    emit ui->fontComboBox->currentTextChanged(ui->fontComboBox->currentText());
    emit ui->spinBoxFontSize->valueChanged(ui->spinBoxFontSize->value());
    emit sigLabTextColor(ui->labLabTextColor->palette().color(QPalette::Background));
    emit sigTextColor(ui->labTextColor->palette().color(QPalette::Background));
    emit sigBackgroundColor(ui->labBackgroundColor->palette().color(QPalette::Background));
//    emit ui->radioHorizontal->toggled(ui->radioHorizontal->isChecked());
//    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
    emit ui->lineLabUpload->textChanged(ui->lineLabUpload->text());
    emit ui->lineLabDown->textChanged(ui->lineLabDown->text());
    emit ui->lineLabCpu->textChanged(ui->lineLabCpu->text());
    emit ui->lineLabMemory->textChanged(ui->lineLabMemory->text());
//    emit ui->lineLabDiskRead->textChanged(ui->lineLabDiskRead->text());
//    emit ui->lineLabDiskWrite->textChanged(ui->lineLabDiskWrite->text());
    emit ui->checkBoxDisolayNet->clicked(ui->checkBoxDisolayNet->isChecked());
    emit ui->checkBoxDisolayCPUAndMemory->clicked(ui->checkBoxDisolayCPUAndMemory->isChecked());
//    emit ui->checkBoxDisolayDisk->clicked(ui->checkBoxDisolayDisk->isChecked());
//    emit ui->checkBoxLocationExchangeNet->clicked(ui->checkBoxLocationExchangeNet->isChecked());
//    emit ui->checkBoxLocationExchangeCPUAndMenory->clicked(ui->checkBoxLocationExchangeCPUAndMenory->isChecked());
//    emit ui->checkBoxLocationExchangeDisk->clicked(ui->checkBoxLocationExchangeDisk->isChecked());
    emit ui->spinBoxFractionalAccuracy->valueChanged(ui->spinBoxFractionalAccuracy->value());
    emit ui->spinBoxRefreshInterval->valueChanged(ui->spinBoxRefreshInterval->value());
    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());

    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
    emit ui->radioHorizontal->clicked(ui->radioHorizontal->isChecked());
    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());
    emit ui->comboBoxDoubleClick->currentIndexChanged(ui->comboBoxDoubleClick->currentIndex());
}

/*!
 * \brief WinDdeDockSetting::saveConfigWinDdeDock 将 UI 控件的数值全部覆盖保存到 config.json 文件
 * \note 功能函数
 */
void WinDdeDockSetting::saveConfigWinDdeDock()
{
    json &jsColorAndFont = m_js["WinDdeDock"]["ColorAndFont"];
    jsColorAndFont["FontSize"] = ui->spinBoxFontSize->value();
    jsColorAndFont["FontType"] = ui->fontComboBox->currentText().toStdString().c_str();
    jsColorAndFont["FontTypeIndex"] = ui->fontComboBox->currentIndex();
    jsColorAndFont["LabTextColor"] = ui->labLabTextColor->palette().color(QPalette::Background).name().toStdString().c_str();
    jsColorAndFont["TextColor"] = ui->labTextColor->palette().color(QPalette::Background).name().toStdString().c_str();
    jsColorAndFont["BackgroundColor"] = ui->labBackgroundColor->palette().color(QPalette::Background).name().toStdString().c_str();

    json &jsModelSetting = m_js["WinDdeDock"]["ModelSetting"];
    jsModelSetting["UnitModel"] = ui->comboBoxUnitModel->currentText().toStdString().c_str();
    jsModelSetting["UnitModelIndex"] = ui->comboBoxUnitModel->currentIndex();
    bool isHorizontal = ui->radioHorizontal->isChecked();
    if (isHorizontal)
        jsModelSetting["IsHorizontal"] = true;
    else
        jsModelSetting["IsHorizontal"] = false;

    json &jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
    if (isHorizontal) {
        jsDisplayText[0]["LabUpload"] = ui->lineLabUpload->text().toStdString().c_str();
        jsDisplayText[0]["LabDown"] = ui->lineLabDown->text().toStdString().c_str();
        jsDisplayText[0]["LabCpu"] = ui->lineLabCpu->text().toStdString().c_str();
        jsDisplayText[0]["LabMemory"] = ui->lineLabMemory->text().toStdString().c_str();
        jsDisplayText[0]["LabDiskRead"] = ui->lineLabDiskRead->text().toStdString().c_str();
        jsDisplayText[0]["LabDiskWrite"] = ui->lineLabDiskWrite->text().toStdString().c_str();
    } else {
        jsDisplayText[1]["LabUpload"] = ui->lineLabUpload->text().toStdString().c_str();
        jsDisplayText[1]["LabDown"] = ui->lineLabDown->text().toStdString().c_str();
        jsDisplayText[1]["LabCpu"] = ui->lineLabCpu->text().toStdString().c_str();
        jsDisplayText[1]["LabMemory"] = ui->lineLabMemory->text().toStdString().c_str();
        jsDisplayText[1]["LabDiskRead"] = ui->lineLabDiskRead->text().toStdString().c_str();
        jsDisplayText[1]["LabDiskWrite"] = ui->lineLabDiskWrite->text().toStdString().c_str();
    }

    jsDisplayText[2]["DisolayNet"] = ui->checkBoxDisolayNet->isChecked();
    jsDisplayText[2]["DisolayCPUAndMemory"] = ui->checkBoxDisolayCPUAndMemory->isChecked();
    jsDisplayText[2]["DisolayDisk"] = ui->checkBoxDisolayDisk->isChecked();
    jsDisplayText[3]["LocationExchangeNet"] = ui->checkBoxLocationExchangeNet->isChecked();
    jsDisplayText[3]["LocationExchangeCPUAndMenory"] = ui->checkBoxLocationExchangeCPUAndMenory->isChecked();
    jsDisplayText[3]["LocationExchangeDisk"] = ui->checkBoxLocationExchangeDisk->isChecked();
    jsDisplayText[4]["FractionalAccuracy"] = ui->spinBoxFractionalAccuracy->value();
    jsDisplayText[4]["RefreshInterval"] = ui->spinBoxRefreshInterval->value();

    json &jsDockWindow = m_js["WinDdeDock"]["DockWindow"];
    jsDockWindow["HoverDisplay"] = ui->checkBoxHoverDisplay->isChecked();
    jsDockWindow["DoubleClickIndex"] = ui->comboBoxDoubleClick->currentIndex();
    jsDockWindow["DoubleClick"] = ui->comboBoxDoubleClick->currentText().toStdString().c_str();

    // TODO: 2021-01-07 占用图模式未写

    saveConfig();
}

/*!
 * \brief WinDdeDockSetting::readConfigWinMain 读取 json 的 WinMain 对象内容，对应 UI 窗口的 “常规设置”
 */
void WinDdeDockSetting::readConfigWinMain()
{
    json jsAppSetting = m_js["WinMain"]["AppSetting"];
    ui->checkBoxBootUpUpdate->setChecked(jsAppSetting["BootUpUpdate"]);
//    ui->btnCheckUpdate  // 检查更新，Outdated 判断是否需要更新
    ui->comboBoxLanguage->setCurrentIndex(jsAppSetting["LanguageIndex"]);

    json jsAppNotification = m_js["WinMain"]["Notification"];
    ui->checkBoxCpuOver->setChecked(jsAppNotification["CpuOver"]);
    ui->spinBoxCpuOverNum->setValue(jsAppNotification["CpuOverNum"]);
    ui->checkBoxMemOver->setChecked(jsAppNotification["MemoryOver"]);
    ui->spinBoxMemOverNum->setValue(jsAppNotification["MemoryOverNum"]);
    ui->checkBoxNetOver->setChecked(jsAppNotification["NetOver"]);
    ui->spinBoxNetOverNum->setValue(jsAppNotification["NetOverNum"]);
    ui->comboBoxNetNumUnit->setCurrentIndex(jsAppNotification["NetOverNumUnitIndex"]);

    json jsThemeStyle = m_js["WinMain"]["ThemeStyle"];
    ui->comboBoxStyle->setCurrentIndex(ui->comboBoxStyle->currentIndex());
    // themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-为止
    int themeIndex = jsThemeStyle["themeIndex"];
    if (themeIndex == 0)
        ui->radioButtonSystem->setChecked(true);
    else if (themeIndex == 1)
        ui->radioButtonLight->setChecked(true);
    else if (themeIndex == 2)
        ui->radioButtonDark->setChecked(true);
    else
        QMessageBox::warning(nullptr, tr("主题选择数值错误"), tr("json 的 themeIndex 值错误，此处采用约定：themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-未知"));

    emit ui->checkBoxBootUpUpdate->clicked(ui->checkBoxBootUpUpdate->isChecked());
    emit ui->radioDefaultPath->toggled(ui->radioDefaultPath->isChecked());
    emit ui->checkBoxCpuOver->clicked(ui->checkBoxCpuOver->isChecked());
    emit ui->checkBoxMemOver->clicked(ui->checkBoxMemOver->isChecked());
    emit ui->checkBoxNetOver->clicked(ui->checkBoxNetOver->isChecked());
    emit ui->radioButtonSystem->toggled(ui->radioButtonSystem->isChecked());
    emit ui->comboBoxStyle->currentIndexChanged(ui->comboBoxStyle->currentIndex());
}

/*!
 * \brief WinDdeDockSetting::saveConfigWinMain 保存 json 的 WinMain 对象内容，对应 UI 窗口的 “常规设置”
 */
void WinDdeDockSetting::saveConfigWinMain()
{
    json &jsAppSetting = m_js["WinMain"]["AppSetting"];
    jsAppSetting["BootUpUpdate"] = ui->checkBoxBootUpUpdate->isChecked();
    jsAppSetting["LanguageIndex"] = ui->comboBoxLanguage->currentIndex();
    jsAppSetting["Language"] = ui->comboBoxLanguage->currentText().toStdString().c_str();

    json &jsAppNotification = m_js["WinMain"]["Notification"];
    jsAppNotification["CpuOver"] = ui->checkBoxCpuOver->isChecked();
    jsAppNotification["CpuOverNum"] = ui->spinBoxCpuOverNum->value();
    jsAppNotification["MemoryOver"] = ui->checkBoxMemOver->isChecked();
    jsAppNotification["MemoryOverNum"] = ui->spinBoxMemOverNum->value();
    jsAppNotification["NetOver"] = ui->checkBoxNetOver->isChecked();
    jsAppNotification["NetOverNum"] = ui->spinBoxNetOverNum->value();
    jsAppNotification["NetOverNumUnitIndex"] = ui->comboBoxNetNumUnit->currentIndex();
    jsAppNotification["NetOverNumUnit"] = ui->comboBoxNetNumUnit->currentText().toStdString().c_str();

    json &jsThemeStyle = m_js["WinMain"]["ThemeStyle"];
    // themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-为止
    if (ui->radioButtonSystem->isChecked())
        jsThemeStyle["themeIndex"] = 0;
    else if (ui->radioButtonLight->isChecked())
        jsThemeStyle["themeIndex"] = 1;
    else if (ui->radioButtonDark->isChecked())
        jsThemeStyle["themeIndex"] = 2;
    else
        QMessageBox::warning(nullptr, tr("主题选择数值错误"), tr("json 的 themeIndex 值错误，此处采用约定：themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-未知"));

    jsThemeStyle["SystemStyleIndex"] = ui->comboBoxStyle->currentIndex();
    jsThemeStyle["SystemStyle"] = ui->comboBoxStyle->currentText().toStdString().c_str();

    saveConfig();
}

/*!
 * \brief WinDdeDockSetting::readConfig 读取配置文件
 * \note 优先读取用户目录下的配置文件，其次去寻找系统级别下的配置文件
 */
void WinDdeDockSetting::readConfig()
{
    int index = -1;
    ifstream jfile(configPath(index));
    jfile >> m_js;
}

/*!
 * \brief WinDdeDockSetting::saveConfig 奖修改好的配置文件，保存到文本文件中
 */
void WinDdeDockSetting::saveConfig()
{
    int index = -1;
    configPath(index);
    if (index == 2)
        writeDataToConfigPath();

    configPath(index);
    ofstream outFile(configPath(index));
    outFile << setw(2) << m_js << endl;
}

/*!
 * \brief WinDdeDockSetting::creatorConfigPath 创建配置文件

 * \return 配置文件的保存路径
 * \note 依据：当前用户创建创建后写入的文件，其是属于当前用户（即使程序本身是属于 root 用户的，但是在 xmuli 用户中执行
 *       ，创建了修改后的文件，用来保存配置文件在 home/xmuli 下，则其是属于 xmuli 用户的，而对于 chmod 可以在程序中赋值）
 */
QString WinDdeDockSetting::creatorConfigPath(QString path)
{
    QDir dir(path);
    if (!dir.exists()) {
        if (!dir.mkpath(path)) {
            qDebug()<< "路径不存在且创建失败：" + path;
            return "";
        }
    }

    return path;
}

/*!
 * \brief WinDdeDockSetting::writeDataToConfigPath 将 sour 路径下的 file 文件，读取之后，写入一份新的到 dest 路径下
 * \param[in] sour 源路径
 * \param[in] dest 目标路径
 * \param[in] file 文件名
 * \return true 成功； false 失败
 */
bool WinDdeDockSetting::writeDataToConfigPath(QString sour, QString dest, QString sourName, QString destName)
{
    QString sourPath = sour + "/" + sourName;
    QString destPath = dest + "/" + destName;
    QFileInfo fileFile(sourPath);
    QFileInfo fileNewFile(destPath);

    if (fileFile.isFile() && !fileNewFile.isFile()) {
        QDir dirDest(dest);
        if (!dirDest.exists())
            dirDest.mkpath(dest);

        if (QFile::copy(sourPath, destPath)) {
            bool ok = QFile::setPermissions(destPath, QFile::ReadOwner | QFile::WriteOwner
                                              | QFile::ReadUser | QFile::WriteUser
                                              | QFile::ReadGroup | QFile::WriteGroup
                                              | QFile::ReadOther | QFile::WriteOther);
            if (!ok)
                qDebug() << "文件设置读写权限失败："<< dest + sourName;
            return true;
        } else {
            qDebug() << "文件拷贝失败"<< sourPath << "---->" << destPath;
            return false;
        }
    } else {
        qDebug() << "文件"<< sourPath << "不存在; 或" << destPath << "已经存在！";
        return false;
    }
}

/*!
 * \brief WinDdeDockSetting::writeDataToConfigPath 保存配置文件
 */
void WinDdeDockSetting::writeDataToConfigPath()
{
    int index = -1;
    QString name("/lfxNet/MonitorNetConfig.json");
    QString sour = QString("/usr/share") + name;
    QString dest = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + name;
    configPath(sour, dest, index);

    sour = sour.left(sour.lastIndexOf("/"));
    dest = dest.left(dest.lastIndexOf("/"));
    name = name.right(name.size() - name.lastIndexOf("/") - 1);
    if (index == 2)
        writeDataToConfigPath(sour, dest, name, name);
}

/*!
 * \brief WinDdeDockSetting::configPath 获取系统配置文件的路径
 * \param[in] systemPath 系统目录的配置文件路径，优先级低
 * \param[in] homePath 家目录的配置文件路径，优先级高（先去查找）
 * \param[out] index 0 输出 homePath； 1 输出 systemPath；-1 输出 “”
 * \return 文件的路径
 * \note 先判断家目录下的文件是否存在，若有则返回；无则去尝试返回系统目录下文件路径
 */
QString WinDdeDockSetting::configPath(QString systemPath, QString homePath, int &index)
{
    QFileInfo fileSystem(systemPath);
    QFileInfo fileHome(homePath);

    if (fileHome.isFile()) {
        index = 1;
        return homePath;
    } else if (fileSystem.isFile()) {
        index = 2;
        return systemPath;
    } else {
        index = -1;
        return "";
    }
}

/*!
 * \brief WinDdeDockSetting::configPath 重载 configPath 函数
 * \param path “相对”路径（包含文件名）
 * \return 配置文件的路径
 */
char *WinDdeDockSetting::configPath(int &index, QString path)
{
    QString name("/lfxNet/MonitorNetConfig.json");
    if (!path.isEmpty())
        name = path;

    QString homePath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + name;
    QString systemPath("/usr/share");
    systemPath += name;

    QString ret = configPath(systemPath, homePath, index);

    char *dataPath = const_cast<char *>(ret.toLatin1().data());  // 成功
//    char *dataPath = const_cast<char *>(ret.toStdString().c_str());  // 会强制转换失败，
    return dataPath;
}

QString WinDdeDockSetting::configPath()
{
    return "";
}

/*!
 * \brief WinDdeDockSetting::isHorizontal 该网速插件现实模式是水平还是垂直
 * \return true 水平； false 垂直
 * \note 单独给初始化时候读取使用的
 */
bool WinDdeDockSetting::isHorizontal()
{
    return ui->radioHorizontal->isChecked();
}

int WinDdeDockSetting::doubleClick()
{
    return m_doubleClick;
}

/*!
 * \brief WinDdeDockSetting::updateLabelText 根据当前 UI 界面的 radioHorizontal 控件是否被中（临时），来更新
 *                                           下面 网速、CPU、内存、磁盘的 QLineEdit 的内容
 * \param isHorizontal true 临时为水平布局； false 临时为垂直布局
 */
void WinDdeDockSetting::updateLabelText(bool isHorizontal)
{
    if (m_js.is_null())
        return;

    json jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
    if (isHorizontal) {
        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[0]["LabUpload"]));
        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[0]["LabDown"]));
        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[0]["LabCpu"]));
        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[0]["LabMemory"]));
        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[0]["LabDiskRead"]));
        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[0]["LabDiskWrite"]));
    } else {
        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[1]["LabUpload"]));
        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[1]["LabDown"]));
        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[1]["LabCpu"]));
        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[1]["LabMemory"]));
        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[1]["LabDiskRead"]));
        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[1]["LabDiskWrite"]));
    }
}

/*!
 * \brief WinDdeDockSetting::eventFilter 事件监视器
 * \param watched 被监视的控件
 * \param event 发生的事件
 * \return 是否成功
 * \note 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
 */
bool WinDdeDockSetting::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->labLabTextColor) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QColor labLabTextColor = QColorDialog::getColor(ui->labLabTextColor->palette().color(QPalette::Background), this, tr("选择文本颜色"));
            QPalette palette;
            palette.setColor(QPalette::Background, labLabTextColor);
            ui->labLabTextColor->setPalette(palette);
            emit sigLabTextColor(labLabTextColor);
            return true;
        }
    } else if (watched == ui->labTextColor) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QColor labTextColor = QColorDialog::getColor(ui->labTextColor->palette().color(QPalette::Background), this, tr("选择文本颜色"));
            QPalette palette;
            palette.setColor(QPalette::Background, labTextColor);
            ui->labTextColor->setPalette(palette);
            emit sigTextColor(labTextColor);
            return true;
        }
    } else if (watched == ui->labBackgroundColor) {
        if (event->type() == QEvent::MouseButtonRelease) {
            QColor labBackgroundColor = QColorDialog::getColor(ui->labTextColor->palette().color(QPalette::Background), this, tr("选择背景颜色"));

            QPalette palette;
            palette.setColor(QPalette::Background, labBackgroundColor);
            ui->labBackgroundColor->setPalette(palette);
            emit sigBackgroundColor(labBackgroundColor);
            return true;
        }
    } else {

    }

    return QWidget::eventFilter(watched, event);
}

/*!
 * \brief WinDdeDockSetting::onBtnApplyWinDdeDock
 * \param check
 */
void WinDdeDockSetting::onBtnApplyWinDdeDock(bool check)
{
    Q_UNUSED(check)
    saveConfigWinDdeDock();
    close();
}

/*!
 * \brief WinDdeDockSetting::onBtnQuitWinDdeDock
 * \param check
 */
void WinDdeDockSetting::onBtnQuitWinDdeDock(bool check)
{
    Q_UNUSED(check)
    close();
}

void WinDdeDockSetting::onBtnApplyWinMain(bool check)
{
    Q_UNUSED(check)
    saveConfigWinMain();
    close();
}

void WinDdeDockSetting::onBtnQuitWinMain(bool check)
{
    Q_UNUSED(check)
    close();
}

void WinDdeDockSetting::onDoubleClick(int index)
{
    if (m_doubleClick != index)
        m_doubleClick = index;
}

/*!
 * \brief WinDdeDockSetting::onBootUpUpdate 设置开机检查更新
 * \param check
 */
void WinDdeDockSetting::onBootUpUpdate(bool check)
{
    // TODO: 2021-01-12
}

void WinDdeDockSetting::onChangePath()
{
    QString sour = "";
    QString sourName("/lfxNet/MonitorNetConfig.json");
    QString systemPath = QString("/usr/share") + sourName;
    QString homePath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + sourName;
    QString filePathAndName = QFileDialog::getSaveFileName(this, tr("配置导出路径"), homePath, tr("导出配置文件(*.json);;所有文件(*.*)"));
    sourName = sourName.right(sourName.size() - sourName.lastIndexOf("/") - 1);
    QString destName = filePathAndName.right(filePathAndName.size() - filePathAndName.lastIndexOf("/") - 1);
    m_path = filePathAndName.left(filePathAndName.lastIndexOf("/"));

    if (!filePathAndName.isEmpty()) {
//        qDebug()<<"[导出配置]============================>"<< homePath << filePathAndName << m_path;
        QFileInfo fileHomePath(homePath);
        QFileInfo fileSystemPath(systemPath);

        if (ui->radioDefaultPath->isChecked()) {
            if (fileSystemPath.isFile())
                sour = systemPath.left(systemPath.lastIndexOf("/"));
            else
                QMessageBox::information(this, tr("提示"), "源配置不存在：" + fileSystemPath.filePath());
        } else {
            if (fileHomePath.isFile())
                sour = homePath.left(homePath.lastIndexOf("/"));
            else
                QMessageBox::information(this, tr("提示"), "源配置不存在：" + fileHomePath.filePath());
        }

        writeDataToConfigPath(sour, m_path, sourName, destName);
    } else {
        QMessageBox::information(this, "导出配置错误", "源配置不存在：" + filePathAndName);
    }
}

void WinDdeDockSetting::onBtnGroupTheme(int index, bool checked)
{
    Q_UNUSED(index)
    Q_UNUSED(checked)

    if (ui->radioButtonSystem->isChecked()) {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::UnknownType);  // UnknownType 枚举就是自动模式
    } else if (ui->radioButtonLight->isChecked()) {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
    } else if (ui->radioButtonDark->isChecked()) {
        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
    }
}

/*!
 * \brief WinDdeDockSetting::onStyle 设置系统风格
 * \param[in] index 当前选中项的索引
 */
void WinDdeDockSetting::onStyle(int index)
{
    if (index != ui->comboBoxStyle->currentIndex())
        ui->comboBoxStyle->setCurrentIndex(index);

    qApp->setStyle(QStyleFactory::create(ui->comboBoxStyle->currentText()));
}

