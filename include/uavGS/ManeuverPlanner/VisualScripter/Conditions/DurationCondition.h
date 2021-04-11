#ifndef DURATIONCONDITION_H
#define DURATIONCONDITION_H

#include <QWidget>
#include <uavGS/ManeuverPlanner/VisualScripter/IGettableJsonObject.h>

namespace Ui {
class DurationCondition;
}

class DurationCondition : public QWidget, public IGettableJsonObject
{
	Q_OBJECT

public:
	explicit DurationCondition(QWidget *parent = nullptr);
	~DurationCondition();

	QJsonObject
	get() const override;

private:
	Ui::DurationCondition *ui;
};

#endif // DURATIONCONDITION_H
