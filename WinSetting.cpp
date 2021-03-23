#include "WinSetting.h"
#include "ui_WinSetting.h"
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
#include <QStandardPaths>
#include <QString>
#include <QComboBox>
#include <QDate>
#include <QFileDialog>
#include <QFile>
using namespace std;

WinSetting::WinSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WinSetting)
    , m_isHorizontal(true)
    , m_trans(new QTranslator(this))
{
    ui->setupUi(this);
//    connect(ui->btnApplyPerson, &QPushButton::clicked, this, &WinSetting::onBtnApplyToJson);
//    connect(ui->btnApplyGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnApplyToJson);

    connect(ui->btnApplyPerson, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);
    connect(ui->btnApplyGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);

    auto path = writeDataToConfigPath().toLatin1().data();
//    qDebug()<<"---------1a-读取配置路径"<<path;
    ifstream jfile(path);
    jfile >> m_js;
    init();
}

WinSetting::~WinSetting()
{
    delete ui;
}

void WinSetting::init()
{
    qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
    initSigConnectPersonalization();
    initSigConnectGeneralSetting();
//    onlyFirstEmitSig();

    ui->labLabTextColor->setAutoFillBackground(true);
    ui->labTextColor->setAutoFillBackground(true);
    ui->labBackgroundColor->setAutoFillBackground(true);

    QString current_date = QDate::currentDate().toString("yyyy-MM-dd");
    ui->labCompile->setText(current_date);

    QStringList listLang;
    QStringList listData;
    listLang << "English" << "简体中文" << "繁體中文(台湾)";
    listData << "es_US" << "zh_CN" << "zh_TW";

    for (int i = 0; i < listLang.count(); ++i)
        ui->comboBoxLanguage->addItem(listLang[i], listData[i]);

    QSize size(120, 30);
    ui->labLabTextColor->installEventFilter(this);
    ui->labLabTextColor->setFixedSize(size);
    ui->labTextColor->installEventFilter(this);
    ui->labTextColor->setFixedSize(size);
    ui->labBackgroundColor->installEventFilter(this);
    ui->labBackgroundColor->setFixedSize(size);
    ui->labBackgroundImage->installEventFilter(this);
    ui->labBackgroundImage->setFixedSize(size);
    ui->labBackgroundImage->setScaledContents(true);
    ui->comboBoxLanguage->installEventFilter(this);  // L18N

    // 读入 json 文件到流中
    readConfig();
    m_isHorizontal = ui->radioHorizontal->isChecked();

    // 控件的基本设置，其读写留其它函数完成
//    auto list = this->findChildren<QAbstractSpinBox *>();  // 切换为上下按钮模式
//    for (auto v : list)
//        v->setProperty("_d_dtk_spinBox", true);

//    ui->labLabTextColor->setAutoFillBackground(true);
//    ui->labTextColor->setAutoFillBackground(true);

//    ui->spinBoxFractionalAccuracy->setRange(0, 100);
//    ui->spinBoxFractionalAccuracy->setSingleStep(1);
//    ui->spinBoxRefreshInterval->setRange(1000, 2147483647);
//    ui->spinBoxRefreshInterval->setSingleStep(1000);
//    ui->spinBoxRefreshInterval->setSuffix(tr("ms"));

//    ui->labLabTextColor->installEventFilter(this);
//    ui->labTextColor->installEventFilter(this);

//    m_btnGroupTheme->addButton(ui->radioButtonSystem);  // m_btnGroupTheme 可以移除
//    m_btnGroupTheme->addButton(ui->radioButtonLight);
//    m_btnGroupTheme->addButton(ui->radioButtonDark);

    QStringList list = QStyleFactory::keys();
    ui->comboBoxsystemStyle->addItems(list);
    for (auto v : list) {
        QString style("Fusion");
        if (v == "chameleon") {
            style = "chameleon";
            ui->comboBoxsystemStyle->setCurrentText(style);
            qApp->setStyle(QStyleFactory::create(ui->comboBoxsystemStyle->currentText()));
        }
    }

    setWindowTitle(QString("NetSpeed"));
}

/*!
 * \brief WinSetting::readConfig 读取 json 文件，初始化 UI 的控件初始值
 */
void WinSetting::readConfig(bool initLanguage)
{
    json jPersona = m_js["Personalization"];   // 个性化
    json jGeneralSet = m_js["GeneralSetting"]; // 常规配置

    //--------------------------个性化初始化--------------------------------
    if (initLanguage) {
        json jLanguage = jPersona["Language"];
        ui->comboBoxLanguage->setCurrentIndex(jLanguage["LanguageIndex"]);
        ui->comboBoxUnitModel->setCurrentIndex(jLanguage["UnitModelIndex"]);
    }

    json jDisplayText;
    json jText;
    if (jGeneralSet["IsHorizontal"]) {
        jDisplayText = jPersona["DisplayTextHor"];
        jText = jDisplayText["TextHor"];

            ui->radioHorizontal->setChecked(true);
    } else {
        jDisplayText = jPersona["DisplayTextVer"];
        jText = jDisplayText["TextVer"];

            ui->radioVertical->setChecked(true);
    }

    bool themeIsLight = jPersona["ThemeIsLight"];
    json jTextColor;
    if (themeIsLight) {
        jTextColor = jDisplayText["TextColorLight"];
        ui->radioButtonLight->setChecked(true);
    } else {
        jTextColor = jDisplayText["TextColorDark"];
        ui->radioButtonDark->setChecked(true);
    }

    ui->lineLabUpload->setText(QString::fromStdString(jText["LabUpload"]));
    ui->lineLabDown->setText(QString::fromStdString(jText["LabDown"]));
    ui->lineLabCpu->setText(QString::fromStdString(jText["LabCpu"]));
    ui->lineLabMemory->setText(QString::fromStdString(jText["LabMemory"]));
    ui->fontComboBox->setCurrentIndex(jText["FontTypeIndex"]);
    ui->spinBoxFontSize->setValue(jText["FontSize"]);

    QPalette palette;
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["LabTextColor"]));
    ui->labLabTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["TextColor"]));
    ui->labTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["LabBackgroundColor"]));
    ui->labBackgroundColor->setPalette(palette);
    ui->labBackgroundImage->setPixmap(QPixmap(":/backGround.png"));  // TODO: 2021-02-24 图片后面设置为可自选，用作背景图片

    //--------------------------常规配置初始化--------------------------------
    json jSystemStyle = jGeneralSet["systemStyle"];
    ui->checkBoxWindowTop->setChecked(jSystemStyle["WindowTop"]);
    ui->comboBoxsystemStyle->setCurrentIndex(jSystemStyle["SystemStyleIndex"]);

    json jDisolayText = jGeneralSet["DisolayText"];
    ui->checkBoxDisolayCPUAndMemory->setChecked(jDisolayText["DisolayCPUAndMemory"]);
    ui->checkBoxDisolayNet->setChecked(jDisolayText["DisolayNet"]);
    ui->checkBoxLocationExchangeCPUAndMenory->setChecked(jDisolayText["LocationExchangeCPUAndMenory"]);
    ui->checkBoxLocationExchangeNet->setChecked(jDisolayText["LocationExchangeNet"]);
    ui->spinBoxFractionalAccuracy->setValue(jDisolayText["FractionalAccuracy"]);
    ui->spinBoxRefreshInterval->setValue(jDisolayText["RefreshInterval"]);

    if (jGeneralSet["IsExportSystenPath"])
        ui->radioSystemPath->setChecked(true);
    else
        ui->radioCustomPath->setChecked(true);
}

void WinSetting::changeOriePreviewUI()
{
    json jPersona = m_js["Personalization"];   // 个性化
    json jGeneralSet = m_js["GeneralSetting"]; // 常规配置

    //--------------------------个性化初始化--------------------------------
    json jLanguage = jPersona["Language"];
    ui->comboBoxLanguage->setCurrentIndex(jLanguage["LanguageIndex"]);
    ui->comboBoxUnitModel->setCurrentIndex(jLanguage["UnitModelIndex"]);

    json jDisplayText;
    json jText;
    bool isHorizontal = ui->radioHorizontal->isChecked();
    if (isHorizontal) {
        jDisplayText = jPersona["DisplayTextHor"];
        jText = jDisplayText["TextHor"];
    } else {
        jDisplayText = jPersona["DisplayTextVer"];
        jText = jDisplayText["TextVer"];
    }

    bool themeIsLight = jPersona["ThemeIsLight"];
    json jTextColor;
    if (themeIsLight) {
        jTextColor = jDisplayText["TextColorLight"];
        ui->radioButtonLight->setChecked(true);
    } else {
        jTextColor = jDisplayText["TextColorDark"];
        ui->radioButtonDark->setChecked(true);
    }

    ui->lineLabUpload->setText(QString::fromStdString(jText["LabUpload"]));
    ui->lineLabDown->setText(QString::fromStdString(jText["LabDown"]));
    ui->lineLabCpu->setText(QString::fromStdString(jText["LabCpu"]));
    ui->lineLabMemory->setText(QString::fromStdString(jText["LabMemory"]));
    ui->fontComboBox->setCurrentIndex(jText["FontTypeIndex"]);
    ui->spinBoxFontSize->setValue(jText["FontSize"]);

    QPalette palette;
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["LabTextColor"]));
    ui->labLabTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["TextColor"]));
    ui->labTextColor->setPalette(palette);
    palette.setColor(QPalette::Background, QString::fromStdString(jTextColor["LabBackgroundColor"]));
    ui->labBackgroundColor->setPalette(palette);
    ui->labBackgroundImage->setPixmap(QPixmap(":/backGround.png"));  // TODO: 2021-02-24 图片后面设置为可自选，用作背景图片

    //--------------------------常规配置初始化--------------------------------
    json jSystemStyle = jGeneralSet["systemStyle"];
    ui->checkBoxWindowTop->setChecked(jSystemStyle["WindowTop"]);
    ui->comboBoxsystemStyle->setCurrentIndex(jSystemStyle["SystemStyleIndex"]);

    json jDisolayText = jGeneralSet["DisolayText"];
    ui->checkBoxDisolayCPUAndMemory->setChecked(jDisolayText["DisolayCPUAndMemory"]);
    ui->checkBoxDisolayNet->setChecked(jDisolayText["DisolayNet"]);
    ui->checkBoxLocationExchangeCPUAndMenory->setChecked(jDisolayText["LocationExchangeCPUAndMenory"]);
    ui->checkBoxLocationExchangeNet->setChecked(jDisolayText["LocationExchangeNet"]);
    ui->spinBoxFractionalAccuracy->setValue(jDisolayText["FractionalAccuracy"]);
    ui->spinBoxRefreshInterval->setValue(jDisolayText["RefreshInterval"]);

    if (jGeneralSet["IsExportSystenPath"])
        ui->radioSystemPath->setChecked(true);
    else
        ui->radioCustomPath->setChecked(true);
}

/*!
 * \brief WinSetting::saveConfig 将当前 UI: WinSetting 界面的数值保存到 json 文件中
 */
void WinSetting::saveConfig()
{
    json& jPersona = m_js["Personalization"];   // 个性化
    json& jGeneralSet = m_js["GeneralSetting"]; // 常规配置

    //--------------------------保存个性化的数据到 JSON 中--------------------------------
    json& jLanguage = jPersona["Language"];
    jLanguage["LanguageIndex"] = ui->comboBoxLanguage->currentIndex();
    jLanguage["Language"] = ui->comboBoxLanguage->currentText().toStdString().c_str();
    jLanguage["UnitModelIndex"] = ui->comboBoxUnitModel->currentIndex();
    jLanguage["UnitModel"] = ui->comboBoxUnitModel->currentText().toStdString().c_str();

    bool isHorizontal = ui->radioHorizontal->isChecked();
    bool isThemeLight = ui->radioButtonLight->isChecked();

    json *jDisplayText;
    char* text = "";
    if (isHorizontal) {
        jDisplayText = &jPersona["DisplayTextHor"];
        text = "TextHor";
    } else {
        jDisplayText = &jPersona["DisplayTextVer"];
        text = "TextVer";
    }

    char* textColor = "";
    if (isThemeLight)
        textColor = "TextColorLight";
    else
        textColor = "TextColorDark";

    jDisplayText->at(text)["LabUpload"] = ui->lineLabUpload->text().toStdString().c_str();
    jDisplayText->at(text)["LabDown"] = ui->lineLabDown->text().toStdString().c_str();
    jDisplayText->at(text)["LabCpu"] = ui->lineLabCpu->text().toStdString().c_str();
    jDisplayText->at(text)["LabMemory"] = ui->lineLabMemory->text().toStdString().c_str();
    jDisplayText->at(text)["FontSize"] = ui->spinBoxFontSize->value();
    jDisplayText->at(text)["FontType"] = ui->fontComboBox->currentText().toStdString().c_str();
    jDisplayText->at(text)["FontTypeIndex"] = ui->fontComboBox->currentIndex();

    jDisplayText->at(textColor)["LabTextColor"] = ui->labLabTextColor->palette().color(QPalette::Background).name().toStdString().c_str();
    jDisplayText->at(textColor)["TextColor"] = ui->labTextColor->palette().color(QPalette::Background).name().toStdString().c_str();
    jDisplayText->at(textColor)["LabBackgroundColor"] = ui->labBackgroundColor->palette().color(QPalette::Background).name().toStdString().c_str();
    // jTextColor["labBackgroundImage"] = 某路径

    //--------------------------保存常规配置的数据到 JSON 中--------------------------------
    json& jSystemStyle = jGeneralSet["systemStyle"];
    jSystemStyle["WindowTop"] = ui->checkBoxWindowTop->isChecked();
    jSystemStyle["SystemStyleIndex"] = ui->comboBoxsystemStyle->currentIndex();
    jSystemStyle["SystemStyle"] = ui->comboBoxsystemStyle->currentText().toStdString().c_str();

    json& jDisolayText = jGeneralSet["DisolayText"];
    jDisolayText["DisolayCPUAndMemory"] = ui->checkBoxDisolayCPUAndMemory->isChecked();
    jDisolayText["DisolayNet"] = ui->checkBoxDisolayNet->isChecked();
    jDisolayText["LocationExchangeCPUAndMenory"] = ui->checkBoxLocationExchangeCPUAndMenory->isChecked();
    jDisolayText["LocationExchangeNet"] = ui->checkBoxLocationExchangeNet->isChecked();
    jDisolayText["FractionalAccuracy"] = ui->spinBoxFractionalAccuracy->value();
    jDisolayText["RefreshInterval"] = ui->spinBoxRefreshInterval->value();

    jGeneralSet["IsHorizontal"] = ui->radioHorizontal->isChecked();
    jGeneralSet["IsExportSystenPath"] = ui->radioSystemPath->isChecked();

    auto path = writeDataToConfigPath().toLatin1().data();
//    cout<<"------------------2a-保存配置路径"<<path;
//    cout.flush();
    ofstream outFile(path);
    outFile << setw(2) << m_js << endl;
}

/*!
 * \brief WinSetting::saveConfig 将改好的配置文件，保存到文本文件中
 */
void WinSetting::saveConfig(bool& isHomePath)
{
    QString path = getConfigPath(isHomePath);

    if (!isHomePath)
        writeDataToConfigPath();

    char * pathC = const_cast<char *>(path.toLatin1().data());
    ofstream outFile(pathC);
    outFile << setw(2) << m_js << endl;
}

void WinSetting::saveConfigThemeIsLight(bool isLight)
{
    m_js["Personalization"]["ThemeIsLight"] = isLight;
}

/*!
 * \brief WinSetting::onlyFirstEmitSig 仅仅第一次初始化,发射信号,用来初始化控件文字使用
 */
void WinSetting::onlyFirstEmitSig()
{
    // 第一次发射信号，加载配置文件
//    emit ui->fontComboBox->currentTextChanged(ui->fontComboBox->currentText());
//    emit ui->spinBoxFontSize->valueChanged(ui->spinBoxFontSize->value());
////    emit sigLabTextColor(ui->labLabTextColor->palette().color(QPalette::Background));
////    emit sigTextColor(ui->labTextColor->palette().color(QPalette::Background));
////    emit ui->radioHorizontal->toggled(ui->radioHorizontal->isChecked());
////    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
    // 个性化
    emit ui->lineLabUpload->textChanged(ui->lineLabUpload->text());
    emit ui->lineLabDown->textChanged(ui->lineLabDown->text());
    emit ui->lineLabCpu->textChanged(ui->lineLabCpu->text());
    emit ui->lineLabMemory->textChanged(ui->lineLabMemory->text());

    emit sigLabTextColor(ui->labLabTextColor->palette().color(QPalette::Background));
    emit sigTextColor(ui->labTextColor->palette().color(QPalette::Background));
    emit sigBackgroundColor(ui->labBackgroundColor->palette().color(QPalette::Background));
//    emit sigBackgroundImage(ui->labTextColor->palette().color(QPalette::Background));

    // 常规配置
//    emit ui->lineLabDiskRead->textChanged(ui->lineLabDiskRead->text());
//    emit ui->lineLabDiskWrite->textChanged(ui->lineLabDiskWrite->text());
//    emit ui->checkBoxDisolayNet->clicked(ui->checkBoxDisolayNet->isChecked());
//    emit ui->checkBoxDisolayCPUAndMemory->clicked(ui->checkBoxDisolayCPUAndMemory->isChecked());
////    emit ui->checkBoxDisolayDisk->clicked(ui->checkBoxDisolayDisk->isChecked());
////    emit ui->checkBoxLocationExchangeNet->clicked(ui->checkBoxLocationExchangeNet->isChecked());
////    emit ui->checkBoxLocationExchangeCPUAndMenory->clicked(ui->checkBoxLocationExchangeCPUAndMenory->isChecked());
////    emit ui->checkBoxLocationExchangeDisk->clicked(ui->checkBoxLocationExchangeDisk->isChecked());
//    emit ui->spinBoxFractionalAccuracy->valueChanged(ui->spinBoxFractionalAccuracy->value());
//    emit ui->spinBoxRefreshInterval->valueChanged(ui->spinBoxRefreshInterval->value());
//    emit ui->checkBoxWindowTop->clicked(ui->checkBoxWindowTop->isChecked());

//    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
//    emit ui->radioHorizontal->clicked(ui->radioHorizontal->isChecked());
//    emit ui->checkBoxWindowTop->clicked(ui->checkBoxWindowTop->isChecked());
////    emit ui->comboBoxDoubleClick->currentIndexChanged(ui->comboBoxDoubleClick->currentIndex());
}

bool WinSetting::isHorizontal()
{
    return m_isHorizontal;
}

bool WinSetting::isLightTheme()
{

}

QString WinSetting::getConfigPath(bool& isHomePath)
{
    QString name("/NetSpeed/NetSpeed.json");

    QString homePath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + name;
    QString systemPath("/usr/share");
    homePath += name;
    systemPath += name;

    return getConfigPath(homePath, systemPath, isHomePath);
}

/*!
 * \brief WinSetting::getConfigPath 获取系统配置文件的路径
 * \param[in] homePath 家目录的配置文件路径，优先级高（先去查找）
 * \param[in] systemPath 系统目录的配置文件路径，优先级低
 * \param[out] true true 输出 homePath； false 输出 systemPath；
 * \return 文件的路径
 * \note 先判断家目录下的文件是否存在，若有则返回；无则去尝试返回系统目录下文件路径
 */
QString WinSetting::getConfigPath(QString systemPath, QString homePath, bool& isHomePath)
{
    QFileInfo fileSystem(systemPath);
    QFileInfo fileHome(homePath);

    if (fileHome.isFile()) {
        isHomePath = true;
        return homePath;
    }

    if (fileSystem.isFile()) {
        isHomePath = false;
        return systemPath;
    } else {    // 如果连 systemPath 中也没有，就需要先拷贝一份过去，或者按照 README.md 运行也可
        QMessageBox::critical(nullptr, tr("Error"), tr("Both \"/usr/share/NetSpeed\" and \"~/.config/NetSpeed\" are missing the NetSpeed.json file, please copy it to either path"));
    }
}

/*!
 * \brief WinSetting::writeDataToConfigPath 保存配置文件
 */
QString WinSetting::writeDataToConfigPath()
{
    QString name("/NetSpeed/NetSpeed.json");
    QString sour = QString("/usr/share") + name;
    QString dest = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + name;
    QString sourName = sour;
    QString destName = dest;
    bool isHomePath = false;  // 家目录该配置文件是否存在
    getConfigPath(sour, dest, isHomePath);

    sour = sour.left(sour.lastIndexOf("/"));
    dest = dest.left(dest.lastIndexOf("/"));
    name = name.right(name.size() - name.lastIndexOf("/") - 1);
    if (isHomePath) {
        return destName;
    } else {
        if (writeDataToConfigPath(sour, dest, name, name))
            return destName;
        else
            return sourName;
    }
}

/*!
 * \brief WinSetting::writeDataToConfigPath 将 sour 路径下的 file 文件，读取之后，写入一份新的到 dest 路径下
 * \param[in] sour 源路径
 * \param[in] dest 目标路径
 * \param[in] file 文件名
 * \return true 成功； false 失败
 */
bool WinSetting::writeDataToConfigPath(QString sour, QString dest, QString sourName, QString destName)
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

void WinSetting::initSigConnectPersonalization()
{
    void (QComboBox::*pFunIndex)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBoxLanguage, pFunIndex, this, &WinSetting::onComboBoxLanguage);
    connect(ui->comboBoxUnitModel, pFunIndex, this, &WinSetting::sigUnitModelIndex);
    connect(ui->comboBoxUnitModel, &QComboBox::currentTextChanged, this, &WinSetting::sigUnitModel);

    connect(ui->lineLabUpload, &QLineEdit::textChanged, this, &WinSetting::sigLabUploadText);
    connect(ui->lineLabDown, &QLineEdit::textChanged, this, &WinSetting::sigLabDownText);
    connect(ui->lineLabCpu, &QLineEdit::textChanged, this, &WinSetting::sigLabCpuText);
    connect(ui->lineLabMemory, &QLineEdit::textChanged, this, &WinSetting::sigLabMemoryText);

    connect(ui->fontComboBox, &QFontComboBox::currentTextChanged, this, &WinSetting::sigCurrentFont);
    void (QSpinBox::*pFun)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBoxFontSize, pFun, this, &WinSetting::sigFontSize);

    connect(ui->radioButtonLight, &QRadioButton::toggled, this, &WinSetting::onTheme);
    connect(ui->radioButtonLight, &QRadioButton::toggled, this, &WinSetting::sigTheme);
    connect(ui->btnApplyPerson, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);
    connect(ui->btnQuitPerson, &QPushButton::clicked, this, &WinSetting::onBtnQuitWinSetting);

//    connect(this, &WinSetting::sigLabTextColor, this, &WinSetting::sigLabMemoryText);
}

void WinSetting::initSigConnectGeneralSetting()
{
    void (QComboBox::*pFun)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBoxsystemStyle, pFun, this, &WinSetting::onCurrystemStyle);
    connect(ui->comboBoxsystemStyle, &QComboBox::currentTextChanged, this, &WinSetting::onCurrystemStyleText);
    connect(ui->checkBoxWindowTop, &QCheckBox::clicked, this, &WinSetting::sigWindowTop);

    // 悬浮窗口 暂时空
    connect(ui->checkBoxDisolayNet, &QCheckBox::clicked, this, &WinSetting::sigDisolayNet);
    connect(ui->checkBoxDisolayCPUAndMemory, &QCheckBox::clicked, this, &WinSetting::sigDisolayCPUAndMemory);
    connect(ui->checkBoxLocationExchangeNet, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeNet);
    connect(ui->checkBoxLocationExchangeCPUAndMenory, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeCPUAndMenory);
    void (QSpinBox::*pFunSpinxBox)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBoxFractionalAccuracy, pFunSpinxBox, this, &WinSetting::sigFractionalAccuracy);
    connect(ui->spinBoxRefreshInterval, pFunSpinxBox, this, &WinSetting::sigRefreshInterval);

    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinSetting::sigShowModel);
    connect(ui->btnExportData, &QRadioButton::clicked, this, &WinSetting::onExportData);

    connect(ui->btnApplyGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);
    connect(ui->btnQuitGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnQuitWinSetting);
}

/*!
 * \brief WinSetting::eventFilter 事件监视器
 * \param watched 被监视的控件
 * \param event 发生的事件
 * \return 是否成功
 * \note 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
 */
bool WinSetting::eventFilter(QObject *watched, QEvent *event)
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
            QColor backgroundColor = QColorDialog::getColor(ui->labBackgroundColor->palette().color(QPalette::Background), this, tr("选择文本颜色"));
            QPalette palette;
            palette.setColor(QPalette::Background, backgroundColor);
            ui->labBackgroundColor->setPalette(palette);
            emit sigBackgroundColor(backgroundColor);
            return true;
        }
    } else if (watched == ui->labBackgroundImage) {
        if (event->type() == QEvent::MouseButtonRelease) {
            // TODO: 替换 QLabel 的图片
            QString path = QDir::currentPath();
            QString fileter = "图片文件(*.jpg *.png *.gif);;所有文件(*.*)";

            QString fileNmae = QFileDialog::getOpenFileName(this, "选择一个文件", path, fileter);
//            qDebug()<<"==============================<<"<<fileNmae;
            ui->labBackgroundImage->setPixmap(QPixmap(fileNmae));
            return true;
        }
    } if (watched == ui->comboBoxLanguage) {
        if (event->type() == QEvent::LanguageChange) {
//            qDebug()<< "---------event()--->" << m_trans;
            ui->retranslateUi(this);
            readConfig(false);        // 因 QLineEidt 在切换语言，会清空内容，就顺便将所有全部读一遍
            return true;
        }
    } else {

    }

    return QWidget::eventFilter(watched, event);
}

/*!
 * \brief WinSetting::onComboBoxLanguage 多语言国际化
 * \param index 当前选中语言
 */
void WinSetting::onComboBoxLanguage(int index)
{
    QString language(QLocale().name());
    language = ui->comboBoxLanguage->itemData(index).toString();
    bool ok = m_trans->load("/usr/share/NetSpeed/translations/" + language + ".qm"); // /usr/share/NetSpeed/translations/
//    qDebug()<< "----@1--->" << "/usr/share/NetSpeed/translations/" + language + ".qm" << ok ;
    QCoreApplication::installTranslator(m_trans);
}

/*!
 * \brief WinSetting::onTheme 切换浅色/亮色主题时候,自动更换对应颜色
 * \param checked true 此时为亮色; false 此时为暗色
 */
void WinSetting::onTheme(bool checked)
{
    saveConfigThemeIsLight(checked);
    readConfig();

    emit sigLabTextColor(ui->labLabTextColor->palette().color(QPalette::Background));
    emit sigTextColor(ui->labTextColor->palette().color(QPalette::Background));
    emit sigBackgroundColor(ui->labBackgroundColor->palette().color(QPalette::Background));
}

void WinSetting::onBtnApplyToJson()
{
    saveConfig();
}

/*!
 * \brief WinSetting::onExportData 导出数据配置
 */
void WinSetting::onExportData(bool checked)
{
    Q_UNUSED(checked)

    QString name("/NetSpeed/NetSpeed.json");
    QString path("");
    if (ui->radioCustomPath->isChecked()) {
        path = QString(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first()) + name;
    } else {
        path = QString("/usr/share") + name;
    }

    if (!QFile::exists(path)) {
        QMessageBox::warning(nullptr, tr("This path does not exist"), path);
        return;
    }

    QString filePathName = QFileDialog::getSaveFileName(this
                                                        ,tr("Select the export save path")
                                                        , path
                                                        , tr("Exporting files(*.json);;All documents(*.*)"));

    if (!QFile::copy(path, filePathName))
        QMessageBox::critical(nullptr, tr("Error"), tr("File export failed"));
}

void WinSetting::onBtnApplyWinSetting()
{
    saveConfig();
}

void WinSetting::onBtnQuitWinSetting()
{
    close();
}

void WinSetting::onCurrystemStyle(int index)
{
    json& jStyle = m_js["GeneralSetting"]["systemStyle"];
    jStyle["SystemStyleIndex"] = index;
}

void WinSetting::onCurrystemStyleText(const QString style)
{
    json& jStyle = m_js["GeneralSetting"]["systemStyle"];
    jStyle["SystemStyle"] = style.toStdString().c_str();

    qApp->setStyle(QStyleFactory::create(style));
}
