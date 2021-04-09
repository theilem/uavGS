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
