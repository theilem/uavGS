//
// Created by mirco on 30.06.20.
//



#include <uavGS/ParameterSets/WidgetTreeNode.h>

void
WidgetTreeNode::add(const std::string& id, std::shared_ptr<WidgetTreeNode> node)
{
	nodes_.insert(std::make_pair(id, node));
}

void
WidgetTreeNode::set(QWidget* widget)
{
	widget_ = widget;
}

QWidget*
WidgetTreeNode::getWidget() const
{
	return widget_;
}

QGroupBox*
WidgetTreeNode::getGroupBox() const
{
	return dynamic_cast<QGroupBox*>(widget_);
}

const std::unordered_map<std::string, std::shared_ptr<WidgetTreeNode>>&
WidgetTreeNode::getNodes() const
{
	return nodes_;
}
