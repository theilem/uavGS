#ifndef SQUAREWAVEFORM_H
#define SQUAREWAVEFORM_H

#include <QWidget>
#include <uavGS/ManeuverPlanner/VisualScripter/IGettableJsonObject.h>

namespace Ui {
class SquareWaveform;
}

class SquareWaveform : public QWidget, public IGettableJsonObject
{
	Q_OBJECT

public:
	explicit SquareWaveform(QWidget *parent = nullptr);
	~SquareWaveform();

	QJsonObject
	get() const override;

private:
	Ui::SquareWaveform *ui;
};

#endif // SQUAREWAVEFORM_H
