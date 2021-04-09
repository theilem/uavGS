#ifndef FLOATINGELEMENT_H
#define FLOATINGELEMENT_H

#include <QWidget>
#include <cpsCore/Utilities/LinearAlgebra.h>
#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/ISelectionElement.h"

namespace Ui {
class FloatingElement;
}

class FloatingElement : public QWidget, public ISelectionElement<FloatingType>
{
	Q_OBJECT

public:
	explicit FloatingElement(const std::string& name, QWidget* parent = nullptr, int level = 0);
	~FloatingElement();

	std::pair<std::string, FloatingType>
	get() override;

private:
	Ui::FloatingElement *ui;
};

#endif // FLOATINGELEMENT_H
