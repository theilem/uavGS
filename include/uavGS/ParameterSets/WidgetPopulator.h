//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETPOPULATOR_H
#define UAVGS_WIDGETPOPULATOR_H

#include <cpsCore/Configuration/Parameter.hpp>
#include <cpsCore/Utilities/LinearAlgebra.h>
#include <QtWidgets/QLayout>
#include <memory>
#include <cpsCore/Utilities/NamedValue.hpp>

#include "LabeledValue.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/WidgetTreeNode.h"
#include "uavGS/ParameterSets/NamedVectorEdit.h"


template<bool Editable>
class WidgetPopulator
{
public:
	static constexpr bool isEditable = Editable;

	WidgetPopulator(std::shared_ptr<WidgetTreeNode> node): node_(node)
	{
	}

	template<typename Type>
	WidgetPopulator&
	operator&(Type& val)
	{
		static_assert(is_parameter<Type>::value || is_named_value<Type>::value,
		              "Can only handle parameters and named values");
		if constexpr (isEditable)
			return populateEditable(val);
		else
			return populateNonEditable(val);
	}

	template<typename Type>
	WidgetPopulator&
	operator&(const Type& val)
	{
		static_assert(is_parameter<Type>::value || is_named_value<Type>::value,
		              "Can only handle parameters and named values");
		return populateNonEditable(val);
	}

	std::shared_ptr<WidgetTreeNode>
	getNode() const
	{
		return node_;
	}

private:
	template<typename Type>
	WidgetPopulator&
	populateEditable(Type& val)
	{
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
				else if constexpr (is_eigen_vector<ValueType>::value)
				{
					auto vectorEdit = dynamic_cast<NamedVectorEdit*>(node->getWidget());
					vectorEdit->set(val.value);
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
			if constexpr (std::is_same<ValueType, bool>::value)
			{
				auto checkBox = new NamedCheckbox(val.id, node_->getGroupBox());
				checkBox->set(val.value);
				node_->getGroupBox()->layout()->addWidget(checkBox);

				auto node = std::make_shared<WidgetTreeNode>();
				node->set(checkBox);
				node_->add(val.id, node);
			}
			else if constexpr (is_eigen_vector<ValueType>::value)
			{
				auto vectorEdit = new NamedVectorEdit(val.id, node_->getGroupBox());
				vectorEdit->set(val.value);
				node_->getGroupBox()->layout()->addWidget(vectorEdit);

				auto node = std::make_shared<WidgetTreeNode>();
				node->set(vectorEdit);
				node_->add(val.id, node);
			}
			else
			{
				static_assert(std::is_pod<ValueType>::value || is_angle<ValueType>::value,
				              "Can only handle Eigen Vector, Angle, and POD so far");
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

	template<typename Type>
	WidgetPopulator&
	populateNonEditable(const Type& val)
	{
		using ValueType = typename Type::ValueType;

		auto it = node_->getNodes().find(val.id);
		if (it != node_->getNodes().end())
		{
			auto node = it->second;

			if constexpr (is_named_value_set<ValueType>::value)
			{
				WidgetPopulator pop(node->getGroupBox(), node);
				val.value.parse(pop);
			}
			else
			{
				auto labeledValue = dynamic_cast<LabeledValue*>(node->getWidget());
				labeledValue->set(val.value);
			}
			return *this;
		}

		if constexpr (is_named_value_set<ValueType>::value || is_parameter_set<ValueType>::value)
		{
			auto groupBox = new QGroupBox(node_->getGroupBox());
			groupBox->setLayout(new QVBoxLayout());
			groupBox->setTitle(QString::fromStdString(val.id));

			auto node = std::make_shared<WidgetTreeNode>();
			node->set(groupBox);
			node_->add(val.id, node);

			WidgetPopulator<isEditable> pop(node);
			val.value.parse(pop);
		}
		else
		{
			static_assert(std::is_pod<ValueType>::value || is_angle<ValueType>::value,
						  "Can only handle Angle, and POD so far");
			auto labeledValue = new LabeledValue(val.id, node_->getGroupBox());
			labeledValue->set(val.value);
			node_->getGroupBox()->layout()->addWidget(labeledValue);

			auto node = std::make_shared<WidgetTreeNode>();
			node->set(labeledValue);
			node_->add(val.id, node);
		}

		return *this;
	}

	std::shared_ptr<WidgetTreeNode> node_;
};

#endif //UAVGS_WIDGETPOPULATOR_H
