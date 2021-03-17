//
// Created by mirco on 11.03.21.
//

#include "uavGS/SensorData/Recording/Recording.h"
#include "ui_Recording.h"
#include "uavGS/SensorData/SensorDataManager.h"

Recording::Recording(QWidget* parent) :
		QWidget(parent), ui(new Ui::Recording)
{
	ui->setupUi(this);
}

Recording::~Recording()
{
	delete ui;
}

void
Recording::on_recordButton_clicked()
{
	ui->stopButton->setEnabled(true);
	ui->recordButton->setEnabled(false);

	auto sdm = get<SensorDataManager>();
	auto sensorData = sdm->getSensorDataLocal();

	recordingFile_.open(
			humanReadableTimeOfDay(sensorData.timestamp) + "_" + ui->lineEdit->text().toStdString() + ".log");
	recordingFile_ << sensorData.sequenceNumber << ",";
}

void
Recording::on_stopButton_clicked()
{
	ui->stopButton->setEnabled(false);
	ui->recordButton->setEnabled(true);

	auto sdm = get<SensorDataManager>();
	auto sensorData = sdm->getSensorDataLocal();
	recordingFile_ << sensorData.sequenceNumber;
	recordingFile_.close();

}

void
Recording::connect()
{
	if (!checkIsSetAll())
	{
		CPSLOG_ERROR << "Missing deps in recording";
	}
}
