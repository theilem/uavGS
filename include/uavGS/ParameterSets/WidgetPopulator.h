//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETPOPULATOR_H
#define UAVGS_WIDGETPOPULATOR_H

#include <cpsCore/Configuration/TypeTraits.hpp>
#include <cpsCore/Configuration/Parameter.hpp>
#include <cpsCore/Logging/CPSLogger.h>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLayout>
#include <memory>
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/WidgetTreeNode.h"


class WidgetPopulator
{

public:

	WidgetPopulator(std::shared_ptr<WidgetTreeNode> node);

	template<typename Type>
	WidgetPopulator&
	operator&(Type& val);

	std::shared_ptr<WidgetTreeNode>
	getNode() const;

private:

	std::shared_ptr<WidgetTreeNode> node_;
};

template<typename Type>
WidgetPopulator&
WidgetPopulator::operator&(Type& val)
{
	static_assert(is_parameter<Type>::value, "Can only handle parameters");
	using ValueType = typename Type::ValueType;

	auto it = node_->getNodes().find(val.id);
	if (it != node_->getNodes().end())
	{
		auto node = it->second;

		if constexpr (is_parameter_set<ValueType>::value)
		{
			WidgetPopulator pop(node->getGroupBox(), node);
			val.value.configure(pop);
		}
		else
		{
			if constexpr (std::is_same<ValueType, bool>::value)
			{
				auto checkBox = dynamic_cast<NamedCheckbox*>(node->getWidget());
				checkBox->set(val.value);
			}
			else
			{
				auto lineEdit = dynamic_cast<NamedLineEdit*>(node->getWidget());
				lineEdit->set(val.value);
			}
		}
		return *this;
	}

	if constexpr (is_parameter_set<ValueType>::value)
	{
		auto groupBox = new QGroupBox(node_->getGroupBox());
		groupBox->setLayout(new QVBoxLayout());
		groupBox->setTitle(QString::fromStdString(val.id));

		auto node = std::make_shared<WidgetTreeNode>();
		node->set(groupBox);
		node_->add(val.id, node);

		WidgetPopulator pop(node);
		val.value.configure(pop);
	}
	else
	{
		static_assert(std::is_pod<ValueType>::value, "Can only handle POD so far");
		if constexpr (std::is_same<ValueType, bool>::value)
		{
			auto checkBox = new NamedCheckbox(val.id, node_->getGroupBox());
			checkBox->set(val.value);
			node_->getGroupBox()->layout()->addWidget(checkBox);

			auto node = std::make_shared<WidgetTreeNode>();
			node->set(checkBox);
			node_->add(val.id, node);
		}
		else
		{
			auto lineEdit = new NamedLineEdit(val.id, node_->getGroupBox());
			lineEdit->set(val.value);
			node_->getGroupBox()->layout()->addWidget(lineEdit);

			auto node = std::make_shared<WidgetTreeNode>();
			node->set(lineEdit);
			node_->add(val.id, node);
		}
	}

	return *this;
}

#endif //UAVGS_WIDGETPOPULATOR_H
