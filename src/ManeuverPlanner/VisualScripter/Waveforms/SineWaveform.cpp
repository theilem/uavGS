#include "uavGS/ManeuverPlanner/VisualScripter/Waveforms/SineWaveform.h"
#include "ui_SineWaveform.h"

SineWaveform::SineWaveform(QWidget* parent, int index) :
		QWidget(parent),
		ui(new Ui::SineWaveform)
{
	ui->setupUi(this);
	if (index >= 0)
	{
		setIdx(index);
	}
}


SineWaveform::~SineWaveform()
{
	delete ui;
}

void
SineWaveform::setIdx(int index)
{
	ui->groupBox->setTitle(QString::asprintf("Sine Wave %d", index + 1));
}

QJsonObject
SineWaveform::get() const
{
	// NOTE: almost identical to square, maybe we should write a common
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

	ans["sine"] = config;
	return ans;
}
