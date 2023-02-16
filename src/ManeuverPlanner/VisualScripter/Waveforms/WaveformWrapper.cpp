#include <cpsCore/Logging/CPSLogger.h>
#include "uavGS/ManeuverPlanner/VisualScripter/Waveforms/SineWaveform.h"
#include "uavGS/ManeuverPlanner/VisualScripter/Waveforms/SquareWaveform.h"

#include "uavGS/ManeuverPlanner/VisualScripter/Waveforms/WaveformWrapper.h"
#include "ui_WaveformWrapper.h"

WaveformWrapper::WaveformWrapper(const std::string& title, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WaveformWrapper),
	widget_(nullptr)
{
	ui->setupUi(this);
	ui->waveformGroupBox->setTitle(QString::fromStdString(title));
}

WaveformWrapper::~WaveformWrapper()
{
	delete ui;
}

void
WaveformWrapper::on_selectWaveform_clicked()
{
	const auto& op = ui->conditionOptions->currentText().toStdString();

	while(ui->waveformLayout->count() > 1) {
		auto wid = ui->waveformLayout->itemAt(1)->widget();
		ui->waveformLayout->removeWidget(wid);
		delete wid;
	}

	if (op == "Sine Wave") {
		auto sw = new SineWaveform(this);
		ui->waveformLayout->addWidget(sw);
		widget_ = sw;
	} else if (op == "Square Wave") {
		auto sw = new SquareWaveform(this);
		ui->waveformLayout->addWidget(sw);
		widget_ = sw;
	} else {
		CPSLOG_ERROR << "Unknown option" << op;
	}
}

void
WaveformWrapper::on_deleteButton_clicked()
{
	emit buttonClicked(ui->waveformGroupBox->title().toStdString(), this);
}

QJsonObject
WaveformWrapper::get() const
{
	if(!widget_)
	{
		CPSLOG_WARN << "Waveform wrapper with missing config";
		return QJsonObject();
	}
	return widget_->get();
}
