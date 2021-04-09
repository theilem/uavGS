#ifndef BOOLEANELEMENT_H
#define BOOLEANELEMENT_H

#include <QWidget>
#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/ISelectionElement.h"

namespace Ui {
class BooleanElement;
}

class BooleanElement : public QWidget, public ISelectionElement<bool>
{
	Q_OBJECT

public:
	BooleanElement(const std::string& name, QWidget* parent = nullptr, int level = 0);
	~BooleanElement();

	std::pair<std::string, bool>
	get() override;

private:
	Ui::BooleanElement *ui;
};

#endif // BOOLEANELEMENT_H
