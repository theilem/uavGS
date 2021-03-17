//
// Created by mirco on 11.03.21.
//

#ifndef UAVGS_RECORDING_H
#define UAVGS_RECORDING_H

#include <fstream>
#include <QWidget>
#include <cpsCore/cps_object>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Recording;
}
QT_END_NAMESPACE

class SensorDataManager;

class Recording : public QWidget, public AggregatableObject<SensorDataManager>
{
Q_OBJECT

public:

	static constexpr auto widgetName = "recording";

	explicit Recording(QWidget* parent = nullptr);

	~Recording() override;

	void
	connect();

private slots:
	void
	on_recordButton_clicked();
	void
	on_stopButton_clicked();
private:
	Ui::Recording* ui;

	std::ofstream recordingFile_;
};

#endif //UAVGS_RECORDING_H
