//
// Created by mirco on 30.06.20.
//

#ifndef UAVGS_WIDGETGENERIC_H
#define UAVGS_WIDGETGENERIC_H

#include <QWidget>
#include <memory>
#include <functional>

namespace Ui
{
class WidgetGeneric;
}

class WidgetTreeNode;

class WidgetGeneric : public QWidget
{

Q_OBJECT;

public:

	static constexpr auto widgetName = "generic";

	WidgetGeneric(QWidget* parent = nullptr);

	void
	setHandles(const std::function<void(void)>& sendHandle, const std::function<void(void)>& requestHandle,
			   const std::function<void(void)>& updateHandle);

	void
	setTitle(const std::string& title);

	std::shared_ptr<WidgetTreeNode>
	getNode() const;

private slots:

	void
	contentUpdatedSlot();

	void
	on_sendButton_clicked();

	void
	on_requestButton_clicked();

signals:

	void
	contentUpdated();

private:

	Ui::WidgetGeneric* ui;

	std::shared_ptr<WidgetTreeNode> node_;

	std::function<void(void)> updateHandle_;
	std::function<void(void)> sendHandle_;
	std::function<void(void)> requestHandle_;


};

#endif //UAVGS_WIDGETGENERIC_H
