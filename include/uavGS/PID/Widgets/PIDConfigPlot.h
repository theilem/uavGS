#ifndef PIDCONFIGPLOT_H
#define PIDCONFIGPLOT_H

#include <QWidget>
#include "uavAP/FlightControl/Controller/PIDController/PIDHandling.h"
#include <cpsCore/Aggregation/AggregatableObject.hpp>

#include <uavGS/PID/PIDConfigurator.h>

namespace Ui
{
    class PIDConfigPlot;
}

class PIDConfigPlot : public QWidget, public AggregatableObject<PIDConfigurator>
{
    Q_OBJECT
public:
    PIDConfigPlot(QWidget* parent, PIDs key);

    ~PIDConfigPlot() override;

    void
    setData(const TimePoint& t, FloatingType current, FloatingType target, FloatingType integrator);

    void
    connect();

    void
    resetGraph();

    void
    updateParams();

private slots:
    void
    onApplyClicked();

    void
    updateSyncStatusDisplay();

    void
    updateParamsDisplay();

    void
    onEditEvent();


private:
    Ui::PIDConfigPlot* ui;

    std::vector<QMetaObject::Connection> connections_;
    boost::signals2::connection syncUpdateSub_;

    PIDs key_;
    Control::PIDParameters* params_ = nullptr;
    PIDConfigurator::PIDSyncStatus* syncStatus_ = nullptr;
};

#endif // PIDCONFIGPLOT_H
