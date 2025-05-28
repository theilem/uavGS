#include <QJsonDocument>
#include <QFileDialog>
#include <QDebug>
#include <uavGS/PID/Widgets/WidgetCPGrid.h>
#include <uavGS/PID/Widgets/PIDConfigPlot.h>
#include <uavGS/GSWidgetFactory.h>
#include <uavGS/PID/PIDConfigurator.h>
#include <QtCore/QJsonObject>

#include "ui_WidgetCPGrid.h"

WidgetCPGrid::WidgetCPGrid(QWidget* parent) :
    QWidget(parent), ui(new Ui::WidgetCPGrid)
{
    ui->setupUi(this);
}

void
WidgetCPGrid::onPIDStati(const TimedPIDStati& stati)
{
    pidStati_ = stati;
    statusUpdated();
}

void
WidgetCPGrid::on_saveGains_clicked()
{
	QFileDialog dialog;
	dialog.setWindowTitle("Save json configs");
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	std::string filename;
	if (dialog.exec())
	{
		filename = dialog.selectedFiles().front().toStdString();
	}
	else
	{
		CPSLOG_TRACE << "Cancelled json save.";
		return;
	}

	auto pc = get<PIDConfigurator>();
	if (!pc)
	{
		CPSLOG_ERROR << "Could not get PIDConfigurator.";
		return;
	}
	pc->savePIDConfig(filename);

}

// void
// WidgetCPGrid::on_loadGains_clicked()
// {
// 	QFileDialog dialog;
// 	dialog.setWindowTitle("Load json configs");
// 	dialog.setFileMode(QFileDialog::ExistingFile);
// 	dialog.setAcceptMode(QFileDialog::AcceptOpen);
// 	QString filename;
// 	if (dialog.exec())
// 	{
// 		filename = dialog.selectedFiles().front();
// 	}
// 	else
// 	{
// 		CPSLOG_TRACE << "Cancelled json save.";
// 		return;
// 	}
// 	QFile loadFile(filename);
// 	if (!loadFile.open(QIODevice::ReadOnly))
// 	{
// 		CPSLOG_ERROR << "Could not open " << filename.toStdString() << " for loading.";
// 		return;
// 	}
// 	QByteArray confData = loadFile.readAll();
// 	loadFile.close();
// 	QJsonDocument loadDoc(QJsonDocument::fromJson(confData));
// 	QJsonObject loadData = loadDoc.object();
// 	for (QString s : loadData.keys())
// 	{
// 		for (auto& it : plots)
// 		{
// 			if (it.second->title == s)
// 			{
// 				it.second->setkP(loadData[s].toObject()["kp"].toDouble());
// 				it.second->setkI(loadData[s].toObject()["ki"].toDouble());
// 				it.second->setkD(loadData[s].toObject()["kd"].toDouble());
// 				it.second->setFF(loadData[s].toObject()["ff"].toDouble());
// 				it.second->setIMax(loadData[s].toObject()["imax"].toDouble());
// 			}
// 		}
// 	}
// }

void
WidgetCPGrid::on_requestParams_clicked()
{
    if (auto pc = get<PIDConfigurator>())
    {
        pc->requestPIDParams();
    }
}

void
WidgetCPGrid::on_sendAllParams_clicked()
{
    auto pc = get<PIDConfigurator>();

    for (const auto& it : plots_)
    {
        pc->applyPID(it.first);
    }
}

void
WidgetCPGrid::createPlotsSlot()
{
    if (!plots_.empty())
    {
        CPSLOG_ERROR << "Requested to create plots, but plots already exist.";
        return;
    }
    if (auto pc = get<PIDConfigurator>())
    {
        auto wf = get<GSWidgetFactory>();
        auto pids = pc->getPIDs();
        int count = 0;
        for (const auto& pid : pids)
        {
            auto plot = new PIDConfigPlot(this, pid);
            wf->connectWidget(plot);
            ui->gridLayout->addWidget(plot, 0, count++);
            plots_.insert({pid, plot});
        }
    }
}

WidgetCPGrid::~WidgetCPGrid()
{
    delete ui;
}

void
WidgetCPGrid::connect()
{
    QObject::connect(this, SIGNAL(statusUpdated()), this, SLOT(statusUpdatedSlot()));
    QObject::connect(this, SIGNAL(clearPlots()), this, SLOT(clearPlotsSlot()));
    QObject::connect(this, SIGNAL(createPlots()), this, SLOT(createPlotsSlot()));
    if (auto dh = get<EnumBasedDataHandling>())
    {
        dh->subscribeOnData<TimedPIDStati>(Content::PID_STATUS, [this](const auto& status) { onPIDStati(status); });
    }

    auto pc = get<PIDConfigurator>();
    pidsClearedSub_ = pc->subscribeOnLocalPIDsCleared([this] { clearPlots(); });
    pidsSetSub_ = pc->subscribeOnLocalPIDsSet(
        [this](PIDParams& params, std::map<PIDs, PIDConfigurator::PIDSyncStatus>& syncStatus)
        {
            createPlots();
        }, true);
}

void
WidgetCPGrid::statusUpdatedSlot()
{
    if (plots_.empty())
        return;
    const auto& time = pidStati_.first;
    const auto& stati = pidStati_.second;
    for (auto& it : stati)
    {
        auto plot = plots_.find(it.first);
        if (plot == plots_.end())
        {
            CPSLOG_WARN << "PID status id " << static_cast<int>(it.first) << " does not match any plot";
            continue;
        }
        plot->second->setData(time, it.second.value, it.second.target, it.second.integrator);
    }
    this->update();
}

void
WidgetCPGrid::clearPlotsSlot()
{
    for (auto& it : plots_)
    {
        ui->gridLayout->removeWidget(it.second);
        delete it.second;
    }
    plots_.clear();
}
