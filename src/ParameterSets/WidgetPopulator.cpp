//
// Created by mirco on 30.06.20.
//

#include "uavGS/ParameterSets/WidgetPopulator.h"


WidgetPopulator::WidgetPopulator(std::shared_ptr<WidgetTreeNode> node): node_(node)
{

}

std::shared_ptr<WidgetTreeNode>
WidgetPopulator::getNode() const
{
	return node_;
}
