//
// Created by mirco on 28.08.20.
//

#ifndef UAVGS_LABELEDVALUE_H
#define UAVGS_LABELEDVALUE_H

#include <string>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <cpsCore/Utilities/Angle.hpp>

class LabeledValue : public QWidget
{
Q_OBJECT
public:

	explicit
	LabeledValue(const std::string& name, QWidget* parent = 0);

	~LabeledValue();

	double
	getDouble();

	float
	getFloat();

	int
	getInt();

	template<typename Type>
	Angle<Type>
	getAngle();

	std::string
	getString();

	bool
	isEmpty();

	template <typename Type>
	void
	set(const Type& val);

	// void
	// set(double val);
	//
	// void
	// set(float val);
	//
	// void
	// set(int val);
	//
	// void
	// set(bool val);

	template<typename Type>
	void
	set(const Angle<Type>& val);

	void
	set(const std::string& val);

private:

	QHBoxLayout layout_;

	QLabel* label_;
	QLabel* value_;

};


template<typename Type>
void
LabeledValue::set(const Angle<Type>& val)
{
	QString string;
	string.setNum(val.degrees());
	value_->setText(string);
}

template<typename Type>
Angle<Type>
LabeledValue::getAngle()
{
	if constexpr (std::is_same<Type, float>::value)
	{
		return Angle<Type>(getFloat());
	}
	else if constexpr(std::is_same<Type, double>::value)
	{
		return Angle<Type>(getDouble());
	}
	else
	{
		static_assert(std::is_same<Type, double>::value || std::is_same<Type, float>::value,
					  "Can only handle float and double for angle");
	}
}

template <typename Type>
void
LabeledValue::set(const Type& val)
{
	QString string;
	string.setNum(val);
	value_->setText(string);
}

#endif //UAVGS_LABELEDVALUE_H
