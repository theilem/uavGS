//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETTREENODE_H
#define UAVGS_WIDGETTREENODE_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
#include <memory>
#include <unordered_map>

class WidgetTreeNode
{
public:

	void
	set(QWidget* widget);

	QWidget*
	getWidget() const;

	QGroupBox*
	getGroupBox() const;

	void
	add(const std::string& id, std::shared_ptr<WidgetTreeNode> node);


	const std::unordered_map<std::string, std::shared_ptr<WidgetTreeNode>>&
	getNodes() const;

private:

	QWidget* widget_;
	std::unordered_map<std::string, std::shared_ptr<WidgetTreeNode>> nodes_;

};

#endif //UAVGS_WIDGETTREENODE_H
