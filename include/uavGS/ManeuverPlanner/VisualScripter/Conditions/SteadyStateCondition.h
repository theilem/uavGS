#ifndef STEADYSTATECONDITION_H
#define STEADYSTATECONDITION_H

#include <QWidget>
#include <set>

namespace Ui
{
class SteadyStateCondition;
}

class SelectionValue;

class SteadyStateCondition: public QWidget
{
Q_OBJECT

public:
	explicit SteadyStateCondition(QWidget* parent = nullptr);

	~SteadyStateCondition();

private:
	Ui::SteadyStateCondition* ui;

	std::map<std::string, SelectionValue*> values_;

private slots:

	void
	on_addButton_clicked();

	void
	onDeleteClicked(const std::string& key, SelectionValue* wid);
};

#endif // STEADYSTATECONDITION_H
