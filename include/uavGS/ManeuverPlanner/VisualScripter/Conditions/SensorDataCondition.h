#ifndef SENSORDATACONDITION_H
#define SENSORDATACONDITION_H

#include <QWidget>

namespace Ui {
class SensorDataCondition;
}

class SensorDataCondition : public QWidget
{
	Q_OBJECT

public:
	explicit SensorDataCondition(QWidget *parent = nullptr);
	~SensorDataCondition();

private:
	Ui::SensorDataCondition *ui;
};

#endif // SENSORDATACONDITION_H
