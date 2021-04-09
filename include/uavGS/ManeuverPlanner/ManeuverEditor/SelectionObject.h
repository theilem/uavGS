//
// Created by seedship on 4/6/21.
//

#ifndef UAVGS_SELECTIONOBJECT_H
#define UAVGS_SELECTIONOBJECT_H


#include <QWidget>
#include <unordered_set>
#include <QVBoxLayout>
#include <uavGS/ManeuverPlanner/ManeuverEditor/SelectionElement/EnumElement.h>
#include <cpsCore/Configuration/Configuration.hpp>

class SelectionObject: public QWidget
{
Q_OBJECT
public:
	SelectionObject(const std::string& name, const std::unordered_set<std::string>& enum_keys,
					const std::unordered_map<std::string, std::string>& mappings,
					const std::unordered_map<std::string, Configuration>& defs, bool closeable = false,
					QWidget* parent = nullptr, int level = 0);

signals:

	void
	on_deleteButtonClicked();

private:
	QVBoxLayout* layout_;
	EnumElement* selector_;
	int level_;

	const std::unordered_map<std::string, std::string>& mappings_;
	const std::unordered_map<std::string, Configuration>& defs_;

private slots:

	void
	on_deleteButton_clicked(bool clicked);

	void
	conditionSelected(const std::string& op);
};


#endif //UAVGS_SELECTIONOBJECT_H
