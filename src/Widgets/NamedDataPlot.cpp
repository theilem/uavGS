//
// Created by Mirco Theile on 3/9/24.
//

#include "uavGS/Widgets/NamedDataPlot.h"

NamedDataPlot::NamedDataPlot(QWidget* parent):
    QWidget(parent)
{
    layout_ = new QHBoxLayout(this);
    plot_ = new QCustomPlot(this);
    scrollArea_ = new QScrollArea(this);
    groupBox_ = new QGroupBox("Data", this);
    checkboxLayout_ = new QVBoxLayout(groupBox_);

    deselectAllButton_ = new QPushButton("Deselect All", groupBox_);
    selectAllButton_ = new QPushButton("Select All", groupBox_);
    checkboxLayout_->addWidget(deselectAllButton_);
    checkboxLayout_->addWidget(selectAllButton_);

    scrollArea_->setWidget(groupBox_);
    scrollArea_->setWidgetResizable(true);
    // Make it as wide as needed for the underlying elements
    scrollArea_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    groupBox_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    groupBox_->setAlignment(Qt::AlignRight);
    scrollArea_->setAlignment(Qt::AlignRight);

    layout_->addWidget(plot_);
    layout_->addWidget(scrollArea_);

    plot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plot_->setBackground(QColor(10, 10, 10));
    plot_->axisRect()->setBackground(QColor(10, 10, 10));
    plot_->xAxis->setBasePen(QPen(Qt::white));
    plot_->yAxis->setBasePen(QPen(Qt::white));
    plot_->xAxis->setTickPen(QPen(Qt::white));
    plot_->yAxis->setTickPen(QPen(Qt::white));
    plot_->xAxis->setSubTickPen(QPen(Qt::white));
    plot_->yAxis->setSubTickPen(QPen(Qt::white));
    //xAxis->setTickLabelColor(Qt::white);
    plot_->xAxis->setLabelPadding(0);
    plot_->yAxis->setTickLabelColor(Qt::white);
}

void
NamedDataPlot::connect()
{
    QObject::connect(this, &NamedDataPlot::dataUpdated, this, &NamedDataPlot::plotData);
    QObject::connect(deselectAllButton_, &QPushButton::clicked, this, &NamedDataPlot::deselectAll);
    QObject::connect(selectAllButton_, &QPushButton::clicked, this, &NamedDataPlot::selectAll);
    QObject::connect(plot_, &QCustomPlot::mouseRelease, this, &NamedDataPlot::plotMouseReleaseEvent);
}

void
NamedDataPlot::plotData()
{
    const auto& [time, data] = dataPoint_;

    auto t = static_cast<double>(std::chrono::duration_cast<Milliseconds>(time.time_since_epoch()).count());
    double resolution = 50.; // Pixels per second TODO: make this configurable
    // Get width of plot in pixels
    auto width = plot_->width();
    // Calculate time span of plot in seconds
    double timeSpan = width / resolution;

    double earliestTime = t - timeSpan * 1000;

    // Clear graphs that are not updated
    std::vector<std::string> remove;
    for (const auto& [name, graph] : graphMap_)
    {
        if (data.find(name) == data.end())
        {
            plot_->removeGraph(graph);
            remove.push_back(name);
        }
    }
    for (const auto& name : remove)
    {
        graphMap_.erase(name);
    }

    // Erase old data
    for (const auto& [name, graph] : graphMap_)
    {
        graph->data()->removeBefore(earliestTime);
    }

    for (const auto& [name, value] : data)
    {
        auto graphIt = graphMap_.find(name);
        if (graphIt == graphMap_.end())
        {
            auto graph = plot_->addGraph();
            graph->setName(QString::fromStdString(name));
            auto color = checkboxMap_[name]->getFontColor();
            graph->setPen(QPen(color, 2));
            graphMap_[name] = graph;
        }
        auto graph = graphMap_[name];
        graph->addData(t, value);
        maxValue_ = std::max(value, maxValue_);
        minValue_ = std::min(value, minValue_);
    }
    auto range = maxValue_ - minValue_;
    plot_->yAxis->setRange(minValue_ - range * 0.05, maxValue_ + range * 0.05);
    plot_->xAxis->setRange(earliestTime, t);
    plot_->replot();
}

void
NamedDataPlot::deselectAll()
{
    for (const auto& [name, checkbox] : checkboxMap_)
    {
        checkbox->set(false);
    }
}

void
NamedDataPlot::selectAll()
{
    for (const auto& [name, checkbox] : checkboxMap_)
    {
        checkbox->set(true);
    }
}

std::map<std::string, bool>
NamedDataPlot::getCheckboxStates() const
{
    std::map<std::string, bool> states;
    for (const auto& [name, checkbox] : checkboxMap_)
    {
        states.insert({name, checkbox->get()});
    }
    return states;
}

void
NamedDataPlot::plotMouseReleaseEvent(QMouseEvent*)
{
    if (graphMap_.empty())
        return;

    minValue_ = std::numeric_limits<double>::max();
    maxValue_ = std::numeric_limits<double>::min();

    for (const auto& [name, graph] : graphMap_)
    {
        auto data = graph->data();
        if (!data->size())
            continue;
        for (const auto& x : *data)
        {
            double val = x.value;
            maxValue_ = std::max(val, maxValue_);
            minValue_ = std::min(val, minValue_);
        }
    }

    double range = maxValue_ - minValue_;
    plot_->yAxis->setRange(minValue_ - range * 0.05, maxValue_ + range * 0.05);
    plot_->replot();
}
