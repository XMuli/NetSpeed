#include "WinDdeDockSetting.h"
#include "ui_WinDdeDockSetting.h"
//#include <QFile>
//#include <QByteArray>
//#include <QJsonDocument>
//#include <QJsonParseError>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>
using namespace std;

#define DATA_JSON_PATH "/home/xmuli/project/github/lfxNet/config.json"

WinDdeDockSetting::WinDdeDockSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WinDdeDockSetting)
    , m_isHorizontal(true)
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
    connect(ui->btnSave, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnSave);
    connect(ui->btnQuit, &QPushButton::clicked, this, &WinDdeDockSetting::onBtnQuit);

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
    connect(ui->spinBoxRefreshInterval, pFun, this, &WinDdeDockSetting::sigRefreshInterval);
    connect(ui->checkBoxHoverDisplay, &QCheckBox::clicked, this, &WinDdeDockSetting::sigHoverDisplay);

    // 读入 json 文件到流中
    ifstream jfile(DATA_JSON_PATH);
    jfile >> m_js;

    // 控件的基本设置，其读写留其它函数完成
    ui->labLabTextColor->setAutoFillBackground(true);
    ui->labTextColor->setAutoFillBackground(true);
    ui->labBackgroundColor->setAutoFillBackground(true);

    ui->spinBoxFractionalAccuracy->setRange(0, 100);
    ui->spinBoxFractionalAccuracy->setSingleStep(1);
    ui->spinBoxFractionalAccuracy->setSuffix(tr("位"));
    ui->spinBoxRefreshInterval->setRange(1000, 2147483647);
    ui->spinBoxRefreshInterval->setSingleStep(1000);
    ui->spinBoxRefreshInterval->setSuffix(tr("ms"));

    ui->labLabTextColor->installEventFilter(this);
    ui->labTextColor->installEventFilter(this);
    ui->labBackgroundColor->installEventFilter(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);
}

/*!
 * \brief WinDdeDockSetting::readConfig 从 config.json 读取 config.json 写入到 UI 控件显示
 */
void WinDdeDockSetting::readConfig()
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
    emit ui->checkBoxDisolayNet->clicked(ui->checkBoxDisolayNet->checkState() == Qt::Checked);
    emit ui->checkBoxDisolayCPUAndMemory->clicked(ui->checkBoxDisolayCPUAndMemory->checkState() == Qt::Checked);
//    emit ui->checkBoxDisolayDisk->clicked(ui->checkBoxDisolayDisk->checkState() == Qt::Checked);
//    emit ui->checkBoxLocationExchangeNet->clicked(ui->checkBoxLocationExchangeNet->checkState() == Qt::Checked);
//    emit ui->checkBoxLocationExchangeCPUAndMenory->clicked(ui->checkBoxLocationExchangeCPUAndMenory->checkState() == Qt::Checked);
//    emit ui->checkBoxLocationExchangeDisk->clicked(ui->checkBoxLocationExchangeDisk->checkState() == Qt::Checked);
    emit ui->spinBoxFractionalAccuracy->valueChanged(ui->spinBoxFractionalAccuracy->value());
    emit ui->spinBoxRefreshInterval->valueChanged(ui->spinBoxRefreshInterval->value());
    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->checkState() == Qt::Checked);

    emit ui->comboBoxUnitModel->currentIndexChanged(ui->comboBoxUnitModel->currentIndex());
    emit ui->radioHorizontal->clicked(ui->radioHorizontal->isChecked());
    emit ui->checkBoxHoverDisplay->clicked(ui->checkBoxHoverDisplay->isChecked());
    emit ui->comboBoxDoubleClick->currentIndexChanged(ui->comboBoxDoubleClick->currentIndex());
}

/*!
 * \brief WinDdeDockSetting::saveConfig 将 UI 控件的数值全部覆盖保存到 config.json 文件
 * \note 功能函数
 */
void WinDdeDockSetting::saveConfig()
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

    jsDisplayText[2]["DisolayNet"] = ui->checkBoxDisolayNet->checkState() == Qt::Checked;
    jsDisplayText[2]["DisolayCPUAndMemory"] = ui->checkBoxDisolayCPUAndMemory->checkState() == Qt::Checked;
    jsDisplayText[2]["DisolayDisk"] = ui->checkBoxDisolayDisk->checkState() == Qt::Checked;
    jsDisplayText[3]["LocationExchangeNet"] = ui->checkBoxLocationExchangeNet->checkState() == Qt::Checked;
    jsDisplayText[3]["LocationExchangeCPUAndMenory"] = ui->checkBoxLocationExchangeCPUAndMenory->checkState() == Qt::Checked;
    jsDisplayText[3]["LocationExchangeDisk"] = ui->checkBoxLocationExchangeDisk->checkState() == Qt::Checked;
    jsDisplayText[4]["FractionalAccuracy"] = ui->spinBoxFractionalAccuracy->value();
    jsDisplayText[4]["RefreshInterval"] = ui->spinBoxRefreshInterval->value();

    json &jsDockWindow = m_js["WinDdeDock"]["DockWindow"];
    jsDockWindow["HoverDisplay"] = ui->checkBoxHoverDisplay->checkState() == Qt::Checked;
    jsDockWindow["DoubleClickIndex"] = ui->comboBoxDoubleClick->currentIndex();
    jsDockWindow["DoubleClick"] = ui->comboBoxDoubleClick->currentText().toStdString().c_str();

    // TODO: 2021-01-07 占用图模式未写

    ofstream outFile(DATA_JSON_PATH);
    outFile << setw(2) << m_js << endl;
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
 * \brief WinDdeDockSetting::onBtnSave
 * \param check
 */
void WinDdeDockSetting::onBtnSave(bool check)
{
    Q_UNUSED(check)
    saveConfig();
}

/*!
 * \brief WinDdeDockSetting::onBtnQuit
 * \param check
 */
void WinDdeDockSetting::onBtnQuit(bool check)
{
    Q_UNUSED(check)
    close();
}
