//
// Created by seedship on 4/6/21.
//

#include <QPushButton>
#include <cpsCore/Logging/CPSLogger.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/BooleanElement.h>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/FloatingElement.h>
#include "uavGS/ManeuverPlanner/ManeuverEditor/SelectionObject.h"

SelectionObject::SelectionObject(const std::string& name, const std::unordered_set<std::string>& enum_keys,
								 const std::unordered_map<std::string, std::string>& mappings,
								 const std::unordered_map<std::string, Configuration>& defs, bool closeable,
								 QWidget* parent, int level) : QWidget(parent), mappings_(mappings), defs_(defs),
								 level_(level)
{
	selector_ = new EnumElement(name, enum_keys, this, level_);
	QObject::connect(selector_, &EnumElement::conditionSelected, this, &SelectionObject::conditionSelected);
	if (closeable) {
		auto hlayout = new QHBoxLayout(this);
		hlayout->addWidget(selector_);
		auto closeButton = new QPushButton(this);
		closeButton->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
		QObject::connect(closeButton, &QPushButton::clicked, this, &SelectionObject::on_deleteButton_clicked);
		hlayout->addWidget(closeButton);
		layout_->addLayout(hlayout);
	} else {
		layout_->addWidget(selector_);
	}
}

void
SelectionObject::on_deleteButton_clicked(bool clicked)
{
	if (clicked)
	{
		emit on_deleteButtonClicked();
	}
}


void
SelectionObject::conditionSelected(const std::string& op)
{
	// Clear layout
	while (layout_->count() > 1)
	{
		QWidget* toDelete = layout_->itemAt(1)->widget();
		layout_->removeWidget(toDelete);
		delete toDelete;
	}

	// getConfiguration definition of selected condition
	const auto& it = mappings_.find(op);
	if (it != mappings_.end())
	{
		if (const auto& configEntry = defs_.find(it->second); configEntry != defs_.end())
		{
			for (const auto& item : configEntry->second)
			{
				const auto& typeString = item.second.get_value<std::string>();
				if (typeString == "bool")
				{
					layout_->addWidget(new BooleanElement(item.first, this, level_ + 1));
				}
				else if (typeString == "double" || typeString == "int")
				{
					layout_->addWidget(new FloatingElement(item.first, this, level_ + 1));
				}
				else if (const auto mapEntry = defs_.find(typeString); mapEntry != defs_.end())
				{
					layout_->addWidget(new EnumElement(item.first, mapEntry->second, this, level_ + 1));
				}
				else if (const auto&[enumMap, mapEntry] = std::tuple{maneuverEditor_->getEnumDefs(),
																	 maneuverEditor_->getEnumDefs().find(typeString)};
						mapEntry != enumMap.end())
				{
					layout_->addWidget(new EnumElement(item.first, mapEntry->second, this, level_ + 1));
				}
			}
		}
	}
	else
	{
		CPSLOG_ERROR << "No reflection information found for option" << op.toStdString();
	}
}