#include <uavGS/PID/Widgets/PIDConfigPlot.h>
#include <uavGS/PID/PIDConfigurator.h>
#include "ui_PIDConfigPlot.h"


PIDConfigPlot::PIDConfigPlot(QWidget* parent, PIDs key) :
    QWidget(parent),
    ui(new Ui::PIDConfigPlot), key_(key)
{
    ui->setupUi(this);
    auto name = EnumMap<PIDs>::convert(key_);
    ui->customPlot->setTitle(name);

    ui->kP->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
    ui->kI->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
    ui->kD->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
    ui->IMax->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
    ui->FF->setStyleSheet(QString("border: 1px solid gray; width: 60px; height:25px;"));
}

PIDConfigPlot::~PIDConfigPlot()
{
    delete ui;
    for (auto& c : connections_)
    {
        QObject::disconnect(c);
    }
    syncUpdateSub_.disconnect();
}

void
PIDConfigPlot::setData(const TimePoint& t, FloatingType current, FloatingType target, FloatingType integrator)
{
    ui->customPlot->addData(t, current, target);
    ui->IntegratorValue->setText(QString::number(std::round(integrator * 1000) / 1000.));
}

void
PIDConfigPlot::connect()
{
    connections_.push_back(QObject::connect(ui->kP, &QLineEdit::editingFinished, this, &PIDConfigPlot::onEditEvent));
    connections_.push_back(QObject::connect(ui->kI, &QLineEdit::editingFinished, this, &PIDConfigPlot::onEditEvent));
    connections_.push_back(QObject::connect(ui->kD, &QLineEdit::editingFinished, this, &PIDConfigPlot::onEditEvent));
    connections_.push_back(QObject::connect(ui->IMax, &QLineEdit::editingFinished, this, &PIDConfigPlot::onEditEvent));
    connections_.push_back(QObject::connect(ui->FF, &QLineEdit::editingFinished, this, &PIDConfigPlot::onEditEvent));
    connections_.push_back(QObject::connect(ui->applyButton, &QPushButton::clicked, this,
                                            &PIDConfigPlot::onApplyClicked));

    auto pc = get<PIDConfigurator>();
    if (!pc)
        return;

    params_ = pc->getPIDParams(key_);
    syncStatus_ = pc->getSyncStatus(key_);

    updateParamsDisplay();
    updateSyncStatusDisplay();

    syncUpdateSub_ = pc->subscribeOnSyncUpdated([this]()
    {
        updateSyncStatusDisplay();
    });
}

void
PIDConfigPlot::resetGraph()
{
    ui->customPlot->resetGraph();
}

void
PIDConfigPlot::updateParams()
{
    params_->kp = ui->kP->text().toFloat();
    params_->ki = ui->kI->text().toFloat();
    params_->kd = ui->kD->text().toFloat();
    params_->imax = ui->IMax->text().toFloat();
    params_->ff = ui->FF->text().toFloat();
}

void
PIDConfigPlot::onApplyClicked()
{
    if (auto pc = get<PIDConfigurator>())
    {
        pc->applyPID(key_);
    }
}

void
PIDConfigPlot::updateSyncStatusDisplay()
{
    if (syncStatus_ == nullptr)
        return;
    updateParamsDisplay();
    switch (*syncStatus_)
    {
    case PIDConfigurator::PIDSyncStatus::MATCHING:
        ui->syncStatus->setText("Matching");
        ui->syncStatus->setStyleSheet("QLabel { background-color : green; color : white; }");
        break;
    case PIDConfigurator::PIDSyncStatus::NOT_MATCHING:
        ui->syncStatus->setText("Mismatch");
        ui->syncStatus->setStyleSheet("QLabel { background-color : red; color : white; }");
        break;
    case PIDConfigurator::PIDSyncStatus::UNKNOWN:
        ui->syncStatus->setText("Unknown");
        ui->syncStatus->setStyleSheet("QLabel { background-color : yellow; color : black; }");
        break;
    default: break;
    }
}

void
PIDConfigPlot::updateParamsDisplay()
{
    if (params_ == nullptr)
        return;
    ui->kP->setText(QString::number(params_->kp()));
    ui->kI->setText(QString::number(params_->ki()));
    ui->kD->setText(QString::number(params_->kd()));
    ui->IMax->setText(QString::number(params_->imax()));
    ui->FF->setText(QString::number(params_->ff()));
}

void
PIDConfigPlot::onEditEvent()
{
    auto pc = get<PIDConfigurator>();
    if (!pc)
        return;
    updateParams();
    pc->localChanged(key_);
}
