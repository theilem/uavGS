#ifndef SENSORDATACONDITION_H
#define SENSORDATACONDITION_H

#include <QWidget>
#include <uavGS/ManeuverPlanner/VisualScripter/IGettableJsonObject.h>

namespace Ui {
class SensorDataCondition;
}

class SensorDataCondition : public QWidget, public IGettableJsonObject
{
	Q_OBJECT

public:
	explicit SensorDataCondition(QWidget *parent = nullptr);
	~SensorDataCondition();

	QJsonObject
	get() const override;

private:
	Ui::SensorDataCondition *ui;

private slots:
	void
	on_useTolerance_clicked(bool checked);
};

#endif // SENSORDATACONDITION_H
