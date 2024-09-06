#ifndef WIDGETS_NAMEDDATAPLOT_H
#define WIDGETS_NAMEDDATAPLOT_H

#include <iostream>

#include "QCustomPlot/QCustomPlot.h"
#include <cpsCore/Utilities/NamedValue.hpp>
#include <cpsCore/Utilities/Time.hpp>

#include "uavGS/ParameterSets/NamedCheckbox.h"

class NamedDataPlot : public QWidget
{
Q_OBJECT
public:
    explicit
    NamedDataPlot(QWidget* parent);

    void
    connect();

    template <typename Type>
    void
    initialize();

    template <typename Type>
    void
    onTimedData(const Type& timedData);

signals:
    void
    dataUpdated();

private slots:
    void
    plotData();

    void
    deselectAll();

    void
    selectAll();

    void
    plotMouseReleaseEvent(QMouseEvent* event) ;

private:
    std::map<std::string, bool>
    getCheckboxStates() const;


    // Static Widget elements
    QHBoxLayout* layout_;
    QCustomPlot* plot_;
    QScrollArea* scrollArea_;
    QGroupBox* groupBox_;
    QVBoxLayout* checkboxLayout_;

    QPushButton* deselectAllButton_;
    QPushButton* selectAllButton_;

    // Dynamic Widget elements
    std::map<std::string, QCPGraph*> graphMap_;
    std::map<std::string, NamedCheckbox*> checkboxMap_;

    // Data to update
    TimedValue<std::map<std::string, double>> dataPoint_;

    double maxValue_{0};
    double minValue_{0};
};

template <typename Type>
void
NamedDataPlot::initialize()
{
    using ValueType = typename Type::ValueType;
    static_assert(is_named_value_set<ValueType>::value,
                  "ValueType in TimedValue must be a NamedValueSet, i.e., a struct with named values");

    auto parser = NamedValueParser<double>();
    const auto& values = parser.parse(ValueType());

    auto colorNames = QColor::colorNames();
    int colorIndex = 0;
    for (const auto& [name, value] : values)
    {
        auto checkbox = new NamedCheckbox(name, groupBox_);
        checkbox->setFontColor(QColor(colorNames[colorIndex++]));
        checkbox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        checkboxLayout_->addWidget(checkbox);
        checkboxMap_.insert({name, checkbox});
    }
}

template <typename Type>
void
NamedDataPlot::onTimedData(const Type& timedData)
{
    static_assert(is_timed_value<Type>::value, "Type must be a TimedValue, i.e., a pair with a time point and a value");
    static_assert(is_named_value_set<typename Type::ValueType>::value,
                  "ValueType in TimedValue must be a NamedValueSet, i.e., a struct with named values");

    const auto& time = timedData.first;
    const auto& data = timedData.second;

    auto checkboxStates = getCheckboxStates();

    NamedValueParser<double> parser;
    const auto& values = parser.parse(data);

    auto updates = std::map<std::string, double>();
    for (const auto& [name, value] : values)
    {
        auto checkboxState = checkboxStates[name];
        if (not checkboxState)
            continue;

        updates[name] = value;
    }
    dataPoint_ = {time, updates};
    emit dataUpdated();
}


#endif // WIDGETS_NAMEDDATAPLOT_H
