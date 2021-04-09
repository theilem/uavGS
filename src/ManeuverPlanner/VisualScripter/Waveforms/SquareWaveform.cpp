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
