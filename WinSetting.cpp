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
using namespace std;

WinSetting::WinSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WinSetting)
    , m_isHorizontal(true)
    , m_path("")
    , m_trans(new QTranslator(this))
{
    ui->setupUi(this);
//    connect(ui->btnApplyPerson, &QPushButton::clicked, this, &WinSetting::onBtnApplyToJson);
//    connect(ui->btnApplyGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnApplyToJson);

    // 高级- 垂直- 修改 label -居然数据没有保存下？？？
    connect(ui->btnApplyPerson, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);
    connect(ui->btnApplyGeneralSet, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinSetting);


    auto path = writeDataToConfigPath().toLatin1().data();
    qDebug()<<"---------1a-读取配置路径"<<path;
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

    setWindowTitle(QString("lfxNet"));
    //    setWindowFlags(Qt::WindowStaysOnTopHint);
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
    ui->checkBoxHoverDisplay->setChecked(jSystemStyle["IsHoverDisplay"]);
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
    ui->checkBoxHoverDisplay->setChecked(jSystemStyle["IsHoverDisplay"]);
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
    jSystemStyle["IsHoverDisplay"] = ui->checkBoxHoverDisplay->isChecked();
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
    cout<<"------------------2a-保存配置路径"<<path;
    cout.flush();
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
//    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());

//    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
//    emit ui->radioHorizontal->clicked(ui->radioHorizontal->isChecked());
//    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());
////    emit ui->comboBoxDoubleClick->currentIndexChanged(ui->comboBoxDoubleClick->currentIndex());
}

bool WinSetting::isHorizontal()
{
    return m_isHorizontal;
}

bool WinSetting::isLightTheme()
{

}


//void WinSetting::initSigConnectWinDdeDock()
//{
//    connect(ui->btnSaveWinDdeDock, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinDdeDock);
//    connect(ui->btnQuitWinDdeDock, &QPushButton::clicked, this, &WinSetting::onBtnQuitWinDdeDock);

//    connect(ui->fontComboBox, &QFontComboBox::currentTextChanged, this, &WinSetting::sigCurrentFont);
//    void (QSpinBox::*pFun)(int) = &QSpinBox::valueChanged;
//    connect(ui->spinBoxFontSize, pFun, this, &WinSetting::sigFontSize);

//    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinSetting::sigShowModel);
//    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinSetting::updateLabelText);
//    void (QComboBox::*pFunIndex)(int) = &QComboBox::currentIndexChanged;
//    connect(ui->comboBoxUnitModel, pFunIndex, this, &WinSetting::sigUnitModelIndex);
//    connect(ui->comboBoxUnitModel, &QComboBox::currentTextChanged, this, &WinSetting::sigUnitModel);
//    connect(ui->lineLabUpload, &QLineEdit::textChanged, this, &WinSetting::sigLabUploadText);
//    connect(ui->lineLabDown, &QLineEdit::textChanged, this, &WinSetting::sigLabDownText);
//    connect(ui->lineLabCpu, &QLineEdit::textChanged, this, &WinSetting::sigLabCpuText);
//    connect(ui->lineLabMemory, &QLineEdit::textChanged, this, &WinSetting::sigLabMemoryText);
//    connect(ui->lineLabDiskRead, &QLineEdit::textChanged, this, &WinSetting::sigLabDiskReadText);
//    connect(ui->lineLabDiskWrite, &QLineEdit::textChanged, this, &WinSetting::sigLabDiskWriteText);
//    connect(ui->checkBoxDisolayDisk, &QCheckBox::clicked, this, &WinSetting::sigDisolayDisk);
//    connect(ui->checkBoxLocationExchangeNet, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeNet);
//    connect(ui->checkBoxLocationExchangeCPUAndMenory, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeCPUAndMenory);
//    connect(ui->checkBoxLocationExchangeDisk, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeDisk);

//    connect(ui->spinBoxFractionalAccuracy, pFun, this, &WinSetting::sigFractionalAccuracy);
//    void (QComboBox::*pFunDoubleClick)(int) = &QComboBox::currentIndexChanged;
//    connect(ui->comboBoxDoubleClick, pFunDoubleClick, this, &WinSetting::onDoubleClick);
//    connect(ui->spinBoxRefreshInterval, pFun, this, &WinSetting::sigRefreshInterval);
//    connect(ui->checkBoxHoverDisplay, &QCheckBox::clicked, this, &WinSetting::sigHoverDisplay);
//}

//void WinSetting::initSigConnectWinMain()
//{
//    connect(ui->btnApplyWinMain, &QPushButton::clicked, this, &WinSetting::onBtnApplyWinMain);
//    connect(ui->btnQuitWinMain, &QPushButton::clicked, this, &WinSetting::onBtnQuitWinMain);
//    connect(ui->btnExportData, &QPushButton::clicked, this, &WinSetting::onChangePath);



//    void (QButtonGroup::*pFunTheme)(int, bool) = &QButtonGroup::buttonToggled;
//    connect(m_btnGroupTheme, pFunTheme, this, &WinSetting::onBtnGroupTheme);
//    void (QComboBox::*pFuncomboBoxStyle)(int) = &QComboBox::currentIndexChanged;
//    connect(ui->comboBoxStyle, pFuncomboBoxStyle, this, &WinSetting::onStyle);
//    connect(ui->btnApplyWinMain, &QPushButton::pressed, this, &WinSetting::sigBtnApplyWinMain);
//}

///*!
// * \brief WinSetting::readConfig 从 config.json 读取 config.json 写入到 UI 控件显示
// */
//void WinSetting::readConfig()
//{
//    json jsColorAndFont = m_js["Personalization"]["ColorAndFont"];
//    ui->fontComboBox->setCurrentIndex(jsColorAndFont["FontTypeIndex"]);
//    ui->spinBoxFontSize->setValue(jsColorAndFont["FontSize"]);
//    QPalette palette;
//    palette.setColor(QPalette::Background, QString::fromStdString(jsColorAndFont["LabTextColor"]));
//    ui->labLabTextColor->setPalette(palette);
//    palette.setColor(QPalette::Background, QString::fromStdString(jsColorAndFont["TextColor"]));
//    ui->labTextColor->setPalette(palette);

//    json jsModelSetting = m_js["WinDdeDock"]["ModelSetting"];
//    ui->comboBoxUnitModel->setCurrentIndex(jsModelSetting["UnitModelIndex"]);
//    if (jsModelSetting["IsHorizontal"]) {
//        ui->radioHorizontal->setChecked(true);
//        ui->radioVertical->setChecked(false);
//    } else {
//        ui->radioHorizontal->setChecked(false);
//        ui->radioVertical->setChecked(true);
//    }

//    json jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
//    if (jsModelSetting["IsHorizontal"]) {
//        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[0]["LabUpload"]));
//        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[0]["LabDown"]));
//        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[0]["LabCpu"]));
//        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[0]["LabMemory"]));
//        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[0]["LabDiskRead"]));
//        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[0]["LabDiskWrite"]));
//    } else {
//        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[1]["LabUpload"]));
//        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[1]["LabDown"]));
//        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[1]["LabCpu"]));
//        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[1]["LabMemory"]));
//        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[1]["LabDiskRead"]));
//        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[1]["LabDiskWrite"]));
//    }

//    ui->checkBoxDisolayNet->setChecked(jsDisplayText[2]["DisolayNet"]);
//    ui->checkBoxDisolayCPUAndMemory->setChecked(jsDisplayText[2]["DisolayCPUAndMemory"]);
//    ui->checkBoxDisolayDisk->setChecked(jsDisplayText[2]["DisolayDisk"]);
//    ui->checkBoxLocationExchangeNet->setChecked(jsDisplayText[3]["LocationExchangeNet"]);
//    ui->checkBoxLocationExchangeCPUAndMenory->setChecked(jsDisplayText[3]["LocationExchangeCPUAndMenory"]);
//    ui->checkBoxLocationExchangeDisk->setChecked(jsDisplayText[3]["LocationExchangeDisk"]);
//    ui->spinBoxFractionalAccuracy->setValue(jsDisplayText[4]["FractionalAccuracy"]);
//    ui->spinBoxRefreshInterval->setValue(jsDisplayText[4]["RefreshInterval"]);

//    json jsDockWindow = m_js["WinDdeDock"]["DockWindow"];
//    ui->checkBoxHoverDisplay->setChecked(jsDockWindow["HoverDisplay"]);
//    ui->comboBoxDoubleClick->setCurrentIndex(jsDockWindow["DoubleClickIndex"]);

//    // TODO: 2021-01-07 占用图模式未写

//    // 第一次发射信号，加载配置文件
//    emit ui->fontComboBox->currentTextChanged(ui->fontComboBox->currentText());
//    emit ui->spinBoxFontSize->valueChanged(ui->spinBoxFontSize->value());
//    emit sigLabTextColor(ui->labLabTextColor->palette().color(QPalette::Background));
//    emit sigTextColor(ui->labTextColor->palette().color(QPalette::Background));
////    emit ui->radioHorizontal->toggled(ui->radioHorizontal->isChecked());
////    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
//    emit ui->lineLabUpload->textChanged(ui->lineLabUpload->text());
//    emit ui->lineLabDown->textChanged(ui->lineLabDown->text());
//    emit ui->lineLabCpu->textChanged(ui->lineLabCpu->text());
//    emit ui->lineLabMemory->textChanged(ui->lineLabMemory->text());
////    emit ui->lineLabDiskRead->textChanged(ui->lineLabDiskRead->text());
////    emit ui->lineLabDiskWrite->textChanged(ui->lineLabDiskWrite->text());
//    emit ui->checkBoxDisolayNet->clicked(ui->checkBoxDisolayNet->isChecked());
//    emit ui->checkBoxDisolayCPUAndMemory->clicked(ui->checkBoxDisolayCPUAndMemory->isChecked());
////    emit ui->checkBoxDisolayDisk->clicked(ui->checkBoxDisolayDisk->isChecked());
////    emit ui->checkBoxLocationExchangeNet->clicked(ui->checkBoxLocationExchangeNet->isChecked());
////    emit ui->checkBoxLocationExchangeCPUAndMenory->clicked(ui->checkBoxLocationExchangeCPUAndMenory->isChecked());
////    emit ui->checkBoxLocationExchangeDisk->clicked(ui->checkBoxLocationExchangeDisk->isChecked());
//    emit ui->spinBoxFractionalAccuracy->valueChanged(ui->spinBoxFractionalAccuracy->value());
//    emit ui->spinBoxRefreshInterval->valueChanged(ui->spinBoxRefreshInterval->value());
//    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());

//    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
//    emit ui->radioHorizontal->clicked(ui->radioHorizontal->isChecked());
//    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());
//    emit ui->comboBoxDoubleClick->currentIndexChanged(ui->comboBoxDoubleClick->currentIndex());
//}

///*!
// * \brief WinSetting::saveConfigWinDdeDock 将 UI 控件的数值全部覆盖保存到 config.json 文件
// * \note 功能函数
// */
//void WinSetting::saveConfigWinDdeDock()
//{
//    json &jsColorAndFont = m_js["WinDdeDock"]["ColorAndFont"];
//    jsColorAndFont["FontSize"] = ui->spinBoxFontSize->value();
//    jsColorAndFont["FontType"] = ui->fontComboBox->currentText().toStdString().c_str();
//    jsColorAndFont["FontTypeIndex"] = ui->fontComboBox->currentIndex();
//    jsColorAndFont["LabTextColor"] = ui->labLabTextColor->palette().color(QPalette::Background).name().toStdString().c_str();
//    jsColorAndFont["TextColor"] = ui->labTextColor->palette().color(QPalette::Background).name().toStdString().c_str();

//    json &jsModelSetting = m_js["WinDdeDock"]["ModelSetting"];
//    jsModelSetting["UnitModel"] = ui->comboBoxUnitModel->currentText().toStdString().c_str();
//    jsModelSetting["UnitModelIndex"] = ui->comboBoxUnitModel->currentIndex();
//    bool isHorizontal = ui->radioHorizontal->isChecked();
//    if (isHorizontal)
//        jsModelSetting["IsHorizontal"] = true;
//    else
//        jsModelSetting["IsHorizontal"] = false;

//    json &jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
//    if (isHorizontal) {
//        jsDisplayText[0]["LabUpload"] = ui->lineLabUpload->text().toStdString().c_str();
//        jsDisplayText[0]["LabDown"] = ui->lineLabDown->text().toStdString().c_str();
//        jsDisplayText[0]["LabCpu"] = ui->lineLabCpu->text().toStdString().c_str();
//        jsDisplayText[0]["LabMemory"] = ui->lineLabMemory->text().toStdString().c_str();
//        jsDisplayText[0]["LabDiskRead"] = ui->lineLabDiskRead->text().toStdString().c_str();
//        jsDisplayText[0]["LabDiskWrite"] = ui->lineLabDiskWrite->text().toStdString().c_str();
//    } else {
//        jsDisplayText[1]["LabUpload"] = ui->lineLabUpload->text().toStdString().c_str();
//        jsDisplayText[1]["LabDown"] = ui->lineLabDown->text().toStdString().c_str();
//        jsDisplayText[1]["LabCpu"] = ui->lineLabCpu->text().toStdString().c_str();
//        jsDisplayText[1]["LabMemory"] = ui->lineLabMemory->text().toStdString().c_str();
//        jsDisplayText[1]["LabDiskRead"] = ui->lineLabDiskRead->text().toStdString().c_str();
//        jsDisplayText[1]["LabDiskWrite"] = ui->lineLabDiskWrite->text().toStdString().c_str();
//    }

//    jsDisplayText[2]["DisolayNet"] = ui->checkBoxDisolayNet->isChecked();
//    jsDisplayText[2]["DisolayCPUAndMemory"] = ui->checkBoxDisolayCPUAndMemory->isChecked();
//    jsDisplayText[2]["DisolayDisk"] = ui->checkBoxDisolayDisk->isChecked();
//    jsDisplayText[3]["LocationExchangeNet"] = ui->checkBoxLocationExchangeNet->isChecked();
//    jsDisplayText[3]["LocationExchangeCPUAndMenory"] = ui->checkBoxLocationExchangeCPUAndMenory->isChecked();
//    jsDisplayText[3]["LocationExchangeDisk"] = ui->checkBoxLocationExchangeDisk->isChecked();
//    jsDisplayText[4]["FractionalAccuracy"] = ui->spinBoxFractionalAccuracy->value();
//    jsDisplayText[4]["RefreshInterval"] = ui->spinBoxRefreshInterval->value();

//    json &jsDockWindow = m_js["WinDdeDock"]["DockWindow"];
//    jsDockWindow["HoverDisplay"] = ui->checkBoxHoverDisplay->isChecked();
//    jsDockWindow["DoubleClickIndex"] = ui->comboBoxDoubleClick->currentIndex();
//    jsDockWindow["DoubleClick"] = ui->comboBoxDoubleClick->currentText().toStdString().c_str();

//    // TODO: 2021-01-07 占用图模式未写

//    saveConfig();
//}

///*!
// * \brief WinSetting::readConfigWinMain 读取 json 的 WinMain 对象内容，对应 UI 窗口的 “常规设置”
// */
//void WinSetting::readConfigWinMain()
//{
//    json jsAppSetting = m_js["WinMain"]["AppSetting"];
//    ui->comboBoxLanguage->setCurrentIndex(jsAppSetting["LanguageIndex"]);

//    json jsAppNotification = m_js["WinMain"]["Notification"];
//    ui->checkBoxCpuOver->setChecked(jsAppNotification["CpuOver"]);
//    ui->spinBoxCpuOverNum->setValue(jsAppNotification["CpuOverNum"]);
//    ui->checkBoxMemOver->setChecked(jsAppNotification["MemoryOver"]);
//    ui->spinBoxMemOverNum->setValue(jsAppNotification["MemoryOverNum"]);

//    json jsThemeStyle = m_js["WinMain"]["ThemeStyle"];
//    ui->comboBoxStyle->setCurrentIndex(ui->comboBoxStyle->currentIndex());
//    // themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-为止
//    int themeIndex = jsThemeStyle["themeIndex"];
//    if (themeIndex == 0)
//        ui->radioButtonSystem->setChecked(true);
//    else if (themeIndex == 1)
//        ui->radioButtonLight->setChecked(true);
//    else if (themeIndex == 2)
//        ui->radioButtonDark->setChecked(true);
//    else
//        QMessageBox::warning(nullptr, tr("主题选择数值错误"), tr("json 的 themeIndex 值错误，此处采用约定：themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-未知"));

//    emit ui->radioDefaultPath->toggled(ui->radioDefaultPath->isChecked());
//    emit ui->checkBoxCpuOver->clicked(ui->checkBoxCpuOver->isChecked());
//    emit ui->checkBoxMemOver->clicked(ui->checkBoxMemOver->isChecked());
//    emit ui->radioButtonSystem->toggled(ui->radioButtonSystem->isChecked());
//    emit ui->comboBoxStyle->currentIndexChanged(ui->comboBoxStyle->currentIndex());
//}

///*!
// * \brief WinSetting::saveConfigWinMain 保存 json 的 WinMain 对象内容，对应 UI 窗口的 “常规设置”
// */
//void WinSetting::saveConfigWinMain()
//{
//    json &jsAppSetting = m_js["WinMain"]["AppSetting"];
//    jsAppSetting["LanguageIndex"] = ui->comboBoxLanguage->currentIndex();
//    jsAppSetting["Language"] = ui->comboBoxLanguage->currentText().toStdString().c_str();

//    json &jsAppNotification = m_js["WinMain"]["Notification"];
//    jsAppNotification["CpuOver"] = ui->checkBoxCpuOver->isChecked();
//    jsAppNotification["CpuOverNum"] = ui->spinBoxCpuOverNum->value();
//    jsAppNotification["MemoryOver"] = ui->checkBoxMemOver->isChecked();
//    jsAppNotification["MemoryOverNum"] = ui->spinBoxMemOverNum->value();

//    json &jsThemeStyle = m_js["WinMain"]["ThemeStyle"];
//    // themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-为止
//    if (ui->radioButtonSystem->isChecked())
//        jsThemeStyle["themeIndex"] = 0;
//    else if (ui->radioButtonLight->isChecked())
//        jsThemeStyle["themeIndex"] = 1;
//    else if (ui->radioButtonDark->isChecked())
//        jsThemeStyle["themeIndex"] = 2;
//    else
//        QMessageBox::warning(nullptr, tr("主题选择数值错误"), tr("json 的 themeIndex 值错误，此处采用约定：themeIndex 为 0-跟随系统； 1-浅色模式； 2-暗色模式； 其它-未知"));

//    jsThemeStyle["SystemStyleIndex"] = ui->comboBoxStyle->currentIndex();
//    jsThemeStyle["SystemStyle"] = ui->comboBoxStyle->currentText().toStdString().c_str();

//    saveConfig();
//}

///*!
// * \brief WinSetting::readConfig 读取配置文件
// * \note 优先读取用户目录下的配置文件，其次去寻找系统级别下的配置文件
// */
//void WinSetting::readConfig(bool isHomePath)
//{
//    char * path = const_cast<char *>(getConfigPath(isHomePath).toLatin1().data());
//    ifstream jfile(path);
//    jfile >> m_js;
//}



///*!
// * \brief WinSetting::creatorConfigPath 创建配置文件

// * \return 配置文件的保存路径
// * \note 依据：当前用户创建创建后写入的文件，其是属于当前用户（即使程序本身是属于 root 用户的，但是在 xmuli 用户中执行
// *       ，创建了修改后的文件，用来保存配置文件在 home/xmuli 下，则其是属于 xmuli 用户的，而对于 chmod 可以在程序中赋值）
// */
//QString WinSetting::creatorConfigPath(QString path)
//{
//    QDir dir(path);
//    if (!dir.exists()) {
//        if (!dir.mkpath(path)) {
//            qDebug()<< "路径不存在且创建失败：" + path;
//            return "";
//        }
//    }

//    return path;
//}



QString WinSetting::getConfigPath(bool& isHomePath)
{
    QString name("/lfxNet/lfxNet.json");

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
    } else {    // 如果连 systemPath 中也没有，就先拷贝一份过去

    }

    // TODO: 两者都没会失败，有风险
}

/*!
 * \brief WinSetting::writeDataToConfigPath 保存配置文件
 */
QString WinSetting::writeDataToConfigPath()
{
    QString name("/lfxNet/lfxNet.json");
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

    // 悬浮窗口 暂时空
    connect(ui->checkBoxDisolayNet, &QCheckBox::clicked, this, &WinSetting::sigDisolayNet);
    connect(ui->checkBoxDisolayCPUAndMemory, &QCheckBox::clicked, this, &WinSetting::sigDisolayCPUAndMemory);
    connect(ui->checkBoxLocationExchangeNet, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeNet);
    connect(ui->checkBoxLocationExchangeCPUAndMenory, &QCheckBox::clicked, this, &WinSetting::sigLocationExchangeCPUAndMenory);
    void (QSpinBox::*pFunSpinxBox)(int) = &QSpinBox::valueChanged;
    connect(ui->spinBoxFractionalAccuracy, pFunSpinxBox, this, &WinSetting::sigFractionalAccuracy);
    connect(ui->spinBoxRefreshInterval, pFunSpinxBox, this, &WinSetting::sigRefreshInterval);

    connect(ui->radioHorizontal, &QRadioButton::toggled, this, &WinSetting::sigShowModel);
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
    bool ok = m_trans->load("./" + language + ".qm");
//    qDebug()<< "---------@1--->" << language << ok ;
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
//    bool isHomePath = true;
//    saveConfig(isHomePath);
}

void WinSetting::onBtnApplyWinSetting()
{
    saveConfig();
//    readConfig();
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


///*!
// * \brief WinSetting::isHorizontal 该网速插件现实模式是水平还是垂直
// * \return true 水平； false 垂直
// * \note 单独给初始化时候读取使用的
// */
//bool WinSetting::isHorizontal()
//{
//    return ui->radioHorizontal->isChecked();
//}

//int WinSetting::doubleClick()
//{
//    return m_doubleClick;
//}

///*!
// * \brief WinSetting::updateLabelText 根据当前 UI 界面的 radioHorizontal 控件是否被中（临时），来更新
// *                                           下面 网速、CPU、内存、磁盘的 QLineEdit 的内容
// * \param isHorizontal true 临时为水平布局； false 临时为垂直布局
// */
//void WinSetting::updateLabelText(bool isHorizontal)
//{
//    if (m_js.is_null())
//        return;

//    json jsDisplayText = m_js["WinDdeDock"]["DisplayText"];
//    if (isHorizontal) {
//        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[0]["LabUpload"]));
//        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[0]["LabDown"]));
//        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[0]["LabCpu"]));
//        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[0]["LabMemory"]));
//        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[0]["LabDiskRead"]));
//        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[0]["LabDiskWrite"]));
//    } else {
//        ui->lineLabUpload->setText(QString::fromStdString(jsDisplayText[1]["LabUpload"]));
//        ui->lineLabDown->setText(QString::fromStdString(jsDisplayText[1]["LabDown"]));
//        ui->lineLabCpu->setText(QString::fromStdString(jsDisplayText[1]["LabCpu"]));
//        ui->lineLabMemory->setText(QString::fromStdString(jsDisplayText[1]["LabMemory"]));
//        ui->lineLabDiskRead->setText(QString::fromStdString(jsDisplayText[1]["LabDiskRead"]));
//        ui->lineLabDiskWrite->setText(QString::fromStdString(jsDisplayText[1]["LabDiskWrite"]));
//    }
//}

///*!
// * \brief WinSetting::eventFilter 事件监视器
// * \param watched 被监视的控件
// * \param event 发生的事件
// * \return 是否成功
// * \note 用法 https://blog.csdn.net/xiezhongyuan07/article/details/79992099
// */
//bool WinSetting::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == ui->labLabTextColor) {
//        if (event->type() == QEvent::MouseButtonRelease) {
//            QColor labLabTextColor = QColorDialog::getColor(ui->labLabTextColor->palette().color(QPalette::Background), this, tr("选择文本颜色"));
//            QPalette palette;
//            palette.setColor(QPalette::Background, labLabTextColor);
//            ui->labLabTextColor->setPalette(palette);
//            emit sigLabTextColor(labLabTextColor);
//            return true;
//        }
//    } else if (watched == ui->labTextColor) {
//        if (event->type() == QEvent::MouseButtonRelease) {
//            QColor labTextColor = QColorDialog::getColor(ui->labTextColor->palette().color(QPalette::Background), this, tr("选择文本颜色"));
//            QPalette palette;
//            palette.setColor(QPalette::Background, labTextColor);
//            ui->labTextColor->setPalette(palette);
//            emit sigTextColor(labTextColor);
//            return true;
//        }
//    } else {

//    }

//    return QWidget::eventFilter(watched, event);
//}

///*!
// * \brief WinSetting::onBtnApplyWinDdeDock
// * \param check
// */
//void WinSetting::onBtnApplyWinDdeDock(bool check)
//{
//    Q_UNUSED(check)
//    saveConfigWinDdeDock();
//    close();
//}

///*!
// * \brief WinSetting::onBtnQuitWinDdeDock
// * \param check
// */
//void WinSetting::onBtnQuitWinDdeDock(bool check)
//{
//    Q_UNUSED(check)
//    close();
//}

//void WinSetting::onBtnApplyWinMain(bool check)
//{
//    Q_UNUSED(check)
//    saveConfigWinMain();
//    close();
//}

//void WinSetting::onBtnQuitWinMain(bool check)
//{
//    Q_UNUSED(check)
//    close();
//}

//void WinSetting::onDoubleClick(int index)
//{
//    if (m_doubleClick != index)
//        m_doubleClick = index;
//}

///*!
// * \brief WinSetting::onBootUpUpdate 设置开机检查更新
// * \param check
// */
//void WinSetting::onBootUpUpdate(bool check)
//{
//    // TODO: 2021-01-12
//}

//void WinSetting::onChangePath()
//{
//    QString sour = "";
//    QString sourName("/lfxNet/lfxNet.json");
//    QString systemPath = QString("/usr/share") + sourName;
//    QString homePath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + sourName;
//    QString filePathAndName = QFileDialog::getSaveFileName(this, tr("配置导出路径"), homePath, tr("导出配置文件(*.json);;所有文件(*.*)"));
//    sourName = sourName.right(sourName.size() - sourName.lastIndexOf("/") - 1);
//    QString destName = filePathAndName.right(filePathAndName.size() - filePathAndName.lastIndexOf("/") - 1);
//    m_path = filePathAndName.left(filePathAndName.lastIndexOf("/"));

//    if (!filePathAndName.isEmpty()) {
////        qDebug()<<"[导出配置]============================>"<< homePath << filePathAndName << m_path;
//        QFileInfo fileHomePath(homePath);
//        QFileInfo fileSystemPath(systemPath);

//        if (ui->radioDefaultPath->isChecked()) {
//            if (fileSystemPath.isFile())
//                sour = systemPath.left(systemPath.lastIndexOf("/"));
//            else
//                QMessageBox::information(this, tr("提示"), "源配置不存在：" + fileSystemPath.filePath());
//        } else {
//            if (fileHomePath.isFile())
//                sour = homePath.left(homePath.lastIndexOf("/"));
//            else
//                QMessageBox::information(this, tr("提示"), "源配置不存在：" + fileHomePath.filePath());
//        }

//        writeDataToConfigPath(sour, m_path, sourName, destName);
//    } else {
//        QMessageBox::information(this, "导出配置错误", "源配置不存在：" + filePathAndName);
//    }
//}

//void WinSetting::onBtnGroupTheme(int index, bool checked)
//{
//    Q_UNUSED(index)
//    Q_UNUSED(checked)

//    if (ui->radioButtonSystem->isChecked()) {
//        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::UnknownType);  // UnknownType 枚举就是自动模式
//    } else if (ui->radioButtonLight->isChecked()) {
//        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
//    } else if (ui->radioButtonDark->isChecked()) {
//        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
//    }
//}

///*!
// * \brief WinSetting::onStyle 设置系统风格
// * \param[in] index 当前选中项的索引
// */
//void WinSetting::onStyle(int index)
//{
//    if (index != ui->comboBoxStyle->currentIndex())
//        ui->comboBoxStyle->setCurrentIndex(index);

//    qApp->setStyle(QStyleFactory::create(ui->comboBoxStyle->currentText()));
//}

