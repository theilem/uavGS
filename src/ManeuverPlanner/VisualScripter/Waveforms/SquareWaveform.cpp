#include "uavGS/ManeuverPlanner/VisualScripter/Waveforms/SquareWaveform.h"
#include "ui_SquareWaveform.h"

SquareWaveform::SquareWaveform(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SquareWaveform)
{
	ui->setupUi(this);
}

SquareWaveform::~SquareWaveform()
{
	delete ui;
}

QJsonObject
SquareWaveform::get() const
{
	QJsonObject ans;
	QJsonObject config;

	const auto &amplitude = ui->amplitude->text();
	if (!amplitude.isEmpty()) config["amplitude"] = amplitude.toDouble();

	const auto &frequency = ui->frequency->text();
	if (!frequency.isEmpty()) config["frequency"] = frequency.toDouble();

	const auto &offset = ui->offset->text();
	if (!offset.isEmpty()) config["offset"] = offset.toDouble();

	const auto &phase = ui->phase->text();
	if (!phase.isEmpty()) config["phase"] = phase.toDouble();

	ans["square"] = config;
	return ans;
}
