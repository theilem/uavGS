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
WidgetCPGrid::onPIDStati(const PIDStati& stati)
{
	pidStati_ = stati;
	emit contentUpdated();
}

void
WidgetCPGrid::on_saveGains_clicked()
{
	QJsonObject config;
	for (auto& it : plots)
	{
		QJsonObject gains;
		gains["kp"] = it.second->getkP();
		gains["ki"] = it.second->getkI();
		gains["kd"] = it.second->getkD();
		gains["ff"] = it.second->getFF();
		gains["imax"] = it.second->getIMax();
		config[it.second->title] = gains;
	}
	//QJsonObject config = cm_->getFlightConfig();
	QJsonDocument d(config);
	QFileDialog dialog;
	dialog.setWindowTitle("Save json configs");
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	QString filename;
	if (dialog.exec())
	{
		filename = dialog.selectedFiles().front();
	}
	else
	{
		CPSLOG_TRACE << "Cancelled json save.";
		return;
	}
	QFile saveFile(filename);
	if (!saveFile.open(QIODevice::WriteOnly))
	{
		CPSLOG_ERROR << "Could not open " << filename.toStdString() << " for saving.";
		return;
	}
	saveFile.write(d.toJson());
	saveFile.close();
}

void
WidgetCPGrid::on_loadGains_clicked()
{
	QFileDialog dialog;
	dialog.setWindowTitle("Load json configs");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	QString filename;
	if (dialog.exec())
	{
		filename = dialog.selectedFiles().front();
	}
	else
	{
		CPSLOG_TRACE << "Cancelled json save.";
		return;
	}
	QFile loadFile(filename);
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		CPSLOG_ERROR << "Could not open " << filename.toStdString() << " for loading.";
		return;
	}
	QByteArray confData = loadFile.readAll();
	loadFile.close();
	QJsonDocument loadDoc(QJsonDocument::fromJson(confData));
	QJsonObject loadData = loadDoc.object();
	for (QString s : loadData.keys())
	{
		for (auto& it : plots)
		{
			if (it.second->title == s)
			{
				it.second->setkP(loadData[s].toObject()["kp"].toDouble());
				it.second->setkI(loadData[s].toObject()["ki"].toDouble());
				it.second->setkD(loadData[s].toObject()["kd"].toDouble());
				it.second->setFF(loadData[s].toObject()["ff"].toDouble());
				it.second->setIMax(loadData[s].toObject()["imax"].toDouble());
			}
		}
	}
}

void
WidgetCPGrid::on_requestParams_clicked()
{
	if (auto pc = get<PIDConfigurator>())
	{
		pc->requestPIDParams();
		plots.clear();
	}
}

void
WidgetCPGrid::on_sendAllParams_clicked()
{
	for (auto& it : plots)
	{
		it.second->sendData();
	}
}

void
WidgetCPGrid::drawPlots()
{
	for (auto it : plots)
	{
		ui->gridLayout->removeWidget(it.second);
		delete it.second;
	}
	plots.clear();
	if (auto pc = get<PIDConfigurator>())
	{
		auto wf = get<GSWidgetFactory>();
		if (!wf)
			return;
		int count = 0;
		for (auto& it : pc->getPIDMap())
		{
			auto cp = wf->createWidget<PIDConfigPlot>(this);
			cp->setParams(it.first, it.second);
			ui->gridLayout->addWidget(cp, 0, count);
			plots.insert(std::make_pair(static_cast<int>(it.first), cp));
			++count;
		}
	}
	else
		CPSLOG_ERROR << "Cannot connect WidgetCPGrid to PIDMap. ConfigManager missing.";
}

WidgetCPGrid::~WidgetCPGrid()
{
	delete ui;
}

void
WidgetCPGrid::connect()
{
	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
	if (auto dh = get<DataHandling>())
	{
		dh->subscribeOnData<PIDStati>(Content::PID_STATUS, [this](const auto& status){onPIDStati(status);});
	}

	drawPlots();
}

void
WidgetCPGrid::contentUpdatedSlot()
{
	if (plots.empty())
	{
		drawPlots();
	}
	if (plots.empty())
		return;
	for (auto& it : pidStati_)
	{
		auto plot = plots.find(static_cast<int>(it.first));
		if (plot == plots.end())
		{
			CPSLOG_WARN << "PID status id " << static_cast<int>(it.first) << " does not match any plot";
			continue;
		}
		plot->second->setData(it.second.value, it.second.target);
	}
	this->update();
}
