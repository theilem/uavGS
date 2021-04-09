#ifndef ENUMELEMENT_H
#define ENUMELEMENT_H

#include <QWidget>
#include <unordered_set>
#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/ISelectionElement.h"

namespace Ui
{
class EnumElement;
}

//NOTE, template classes not supported by MOC (which is invoked if the Q_OBJECT macro is present)
class EnumElement: public QWidget, public ISelectionElement<std::string>
{
Q_OBJECT

public:
	explicit EnumElement(const std::string& name, const std::unordered_set<std::string>& enum_keys,
						 QWidget* parent = nullptr, int level = 0);

	~EnumElement();

	std::pair<std::string, std::string>
	get() override;

signals:

	void
	conditionSelected(const std::string& op);


private:
	Ui::EnumElement* ui;

private slots:

	void
	on_conditionSelected(const QString& op);

};

#endif // ENUMELEMENT_H
