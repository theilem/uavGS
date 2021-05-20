//
// Created by seedship on 5/17/21.
//

#ifndef UAVGS_MODELBASEDHELPERWIDGET_H
#define UAVGS_MODELBASEDHELPERWIDGET_H

#include <QWidget>
#include <cpsCore/Aggregation/AggregatableObject.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
class ModelBasedHelperWidget;
}
QT_END_NAMESPACE
class DataHandling;

class ModelBasedHelperWidget: public QWidget, public AggregatableObject<DataHandling>
{
Q_OBJECT

public:
	explicit ModelBasedHelperWidget(QWidget* parent = nullptr);

	~ModelBasedHelperWidget() override;

	inline void
	connect() {}

private slots:
	void
	on_clearLQRIntegrators_clicked();

	void
	on_stopSafetyController_clicked();

private:
	Ui::ModelBasedHelperWidget* ui;
};

#endif //UAVGS_MODELBASEDHELPERWIDGET_H
