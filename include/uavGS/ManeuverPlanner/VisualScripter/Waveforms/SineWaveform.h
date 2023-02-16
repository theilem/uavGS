#ifndef SINEWAVEFORM_H
#define SINEWAVEFORM_H

#include <QWidget>
#include <uavGS/ManeuverPlanner/VisualScripter/IGettableJsonObject.h>

namespace Ui {
class SineWaveform;
}

class SineWaveform : public QWidget, public IGettableJsonObject
{
	Q_OBJECT

public:
	explicit SineWaveform(QWidget *parent = nullptr, int index = -1);
	~SineWaveform();

	void setIdx(int index);

	QJsonObject
	get() const override;

private:
	Ui::SineWaveform *ui;
};

#endif // SINEWAVEFORM_H
