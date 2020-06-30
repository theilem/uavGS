//
// Created by mirco on 30.06.20.
//

#include <uavGS/ParameterSets/NamedLineEdit.h>
#include "uavGS/ParameterSets/WidgetGeneric.h"
#include "uavGS/ParameterSets/WidgetPopulator.h"
#include "uavGS/ParameterSets/WidgetTreeParser.h"


#include "ui_WidgetGeneric.h"

WidgetGeneric::WidgetGeneric(QWidget* parent) :
		QWidget(parent), ui(new Ui::WidgetGeneric)
{
	ui->setupUi(this);
	node_ = std::make_shared<WidgetTreeNode>();
	node_->set(ui->groupBox);

	QObject::connect(this, SIGNAL(contentUpdated()), this, SLOT(contentUpdatedSlot()));
}

void
WidgetGeneric::contentUpdatedSlot()
{
	updateHandle_();
}

void
WidgetGeneric::setHandles(const std::function<void(void)>& sendHandle, const std::function<void(void)>& requestHandle,
						  const std::function<void(void)>& updateHandle)
{
	sendHandle_ = sendHandle;
	requestHandle_ = requestHandle;
	updateHandle_ = updateHandle;
}


void
WidgetGeneric::setTitle(const std::string& title)
{
	ui->groupBox->setTitle(QString::fromStdString(title));
}

std::shared_ptr<WidgetTreeNode>
WidgetGeneric::getNode() const
{
	return node_;
}

void
WidgetGeneric::on_sendButton_clicked()
{
	sendHandle_();
}

void
WidgetGeneric::on_requestButton_clicked()
{
	requestHandle_();
}
