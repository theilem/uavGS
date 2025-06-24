//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETTREEPARSER_H
#define UAVGS_WIDGETTREEPARSER_H

#include <cpsCore/Configuration/Parameter.hpp>
#include <cpsCore/Utilities/LinearAlgebra.h>
#include <cpsCore/Logging/CPSLogger.h>

#include "cpsCore/Utilities/NamedValue.hpp"
#include "uavGS/ParameterSets/WidgetTreeNode.h"
#include "uavGS/ParameterSets/NamedCheckbox.h"
#include "uavGS/ParameterSets/NamedLineEdit.h"
#include "uavGS/ParameterSets/NamedVectorEdit.h"

class WidgetTreeParser
{
public:

	WidgetTreeParser(std::shared_ptr<WidgetTreeNode> node);

	template<typename Type>
	WidgetTreeParser&
	operator&(Type& val);

private:

	std::shared_ptr<WidgetTreeNode> node_;

};

template<typename Type>
WidgetTreeParser&
WidgetTreeParser::operator&(Type& val)
{
	static_assert(is_parameter<Type>::value || is_named_value<Type>::value, "Can only handle parameters or named value");
	using ValueType = typename Type::ValueType;

	auto it = node_->getNodes().find(val.id);
	if (it != node_->getNodes().end())
	{
		auto node = it->second;

		if constexpr (is_parameter_set<ValueType>::value)
		{
			WidgetTreeParser parse(node);
			val.value.configure(parse);
		}
		else
		{
			if constexpr (std::is_same<ValueType, bool>::value)
			{
				auto checkBox = dynamic_cast<NamedCheckbox*>(node->getWidget());
				val.setValue(checkBox->get());
			}
			else if constexpr (is_eigen_vector<ValueType>::value)
			{
				auto vectorEdit = dynamic_cast<NamedVectorEdit*>(node->getWidget());
				if constexpr (std::is_same<double, typename ValueType::value_type>::value)
					val.setValue(vectorEdit->getDouble());
				else if constexpr (std::is_same<float,  typename ValueType::value_type>::value)
					val.setValue(vectorEdit->getFloat());
				else if constexpr (std::numeric_limits< typename ValueType::value_type>::is_integer)
					val.setValue(vectorEdit->getInt());
			}
			else
			{
				auto lineEdit = dynamic_cast<NamedLineEdit*>(node->getWidget());
				if constexpr (std::is_same<double, ValueType>::value)
					val.setValue(lineEdit->getDouble());
				else if constexpr (std::is_same<float, ValueType>::value)
					val.setValue(lineEdit->getFloat());
				else if constexpr (std::numeric_limits<ValueType>::is_integer)
					val.setValue(lineEdit->getInt());
				else if constexpr (is_angle<ValueType>::value)
					val.setValue(lineEdit->getAngle<typename ValueType::ValueType>());
			}
		}
		return *this;
	}
	else
	{
		if constexpr (is_parameter<Type>::value)
		{
			if (val.mandatory)
			{
				CPSLOG_ERROR << "Could not find mandatory field: " << val.id;
			}
		}
	}
	return *this;
}


#endif //UAVGS_WIDGETTREEPARSER_H
