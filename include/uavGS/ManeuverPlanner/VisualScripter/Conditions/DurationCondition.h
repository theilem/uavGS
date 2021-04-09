#ifndef DURATIONCONDITION_H
#define DURATIONCONDITION_H

#include <QWidget>

namespace Ui {
class DurationCondition;
}

class DurationCondition : public QWidget
{
	Q_OBJECT

public:
	explicit DurationCondition(QWidget *parent = nullptr);
	~DurationCondition();

private:
	Ui::DurationCondition *ui;
};

#endif // DURATIONCONDITION_H
