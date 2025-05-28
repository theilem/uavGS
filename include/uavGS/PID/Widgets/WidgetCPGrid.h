#ifndef WIDGETCPGRID_H
#define WIDGETCPGRID_H

#include <QWidget>
#include <memory>
#include <uavGS/PID/PIDConfigurator.h>
#include <uavGS/GSWidgetFactory.h>

namespace Ui
{
    class WidgetCPGrid;
}

class PIDConfigPlot;
template <typename C, typename T>
class DataHandling;

class WidgetCPGrid : public QWidget,
                     public AggregatableObject<PIDConfigurator, GSWidgetFactory, DataHandling<Content, Target>>
{
    Q_OBJECT

public:
    static constexpr auto widgetName = "cp_grid";

    explicit
    WidgetCPGrid(QWidget* parent = nullptr);

    ~WidgetCPGrid() override;

    void
    connect();

public slots:
    void
    on_sendAllParams_clicked();

    void
    on_saveGains_clicked();
    //
    // void
    // on_loadGains_clicked();

    void
    on_requestParams_clicked();

    void
    statusUpdatedSlot();

    void
    clearPlotsSlot();

    void
    createPlotsSlot();

signals:
    void
    statusUpdated();

    void
    clearPlots();

    void
    createPlots();

private:
    void
    onPIDStati(const TimedPIDStati& data);

    Ui::WidgetCPGrid* ui;
    TimedPIDStati pidStati_;
    std::map<PIDs, PIDConfigPlot*> plots_;

    boost::signals2::connection pidsClearedSub_;
    boost::signals2::connection pidsSetSub_;
};

#endif // WIDGETCPGRID_H
