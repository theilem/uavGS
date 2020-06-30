//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETTREEPARSER_H
#define UAVGS_WIDGETTREEPARSER_H

#include <cpsCore/Configuration/Parameter.hpp>
#include <cpsCore/Logging/CPSLogger.h>
#include "uavGS/ParameterSets/WidgetTreeNode.h"
#include "NamedCheckbox.h"
#include "NamedLineEdit.h"

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
	static_assert(is_parameter<Type>::value, "Can only handle parameters");
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
			else
			{
				auto lineEdit = dynamic_cast<NamedLineEdit*>(node->getWidget());
				if constexpr (std::is_same<double, ValueType>::value)
					val.setValue(lineEdit->getDouble());
				else if constexpr (std::is_same<float, ValueType>::value)
					val.setValue(lineEdit->getFloat());
				else if constexpr (std::numeric_limits<ValueType>::is_integer)
					val.setValue(lineEdit->getInt());
			}
		}
		return *this;
	}
	else
	{
		if (val.mandatory)
		{
			CPSLOG_ERROR << "Could not find mandatory field: " << val.id;
		}
	}
	return *this;
}


#endif //UAVGS_WIDGETTREEPARSER_H
