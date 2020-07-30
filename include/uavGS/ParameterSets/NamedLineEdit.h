/*
 * LineEdit.h
 *
 *  Created on: Jul 31, 2018
 *      Author: mircot
 */

#ifndef GROUND_STATION_INCLUDE_GROUND_STATION_WIDGETS_GENERICPROTO_NAMEDLINEEDIT_H_
#define GROUND_STATION_INCLUDE_GROUND_STATION_WIDGETS_GENERICPROTO_NAMEDLINEEDIT_H_

#include <string>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <cpsCore/Utilities/Angle.hpp>

class NamedLineEdit : public QWidget
{
Q_OBJECT
public:

	explicit
	NamedLineEdit(const std::string& name, QWidget* parent = 0);

	~NamedLineEdit();

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

	void
	set(double val);

	void
	set(float val);

	void
	set(int val);

	template<typename Type>
	void
	set(const Angle<Type>& val);

	void
	set(const std::string& val);

private:

	QHBoxLayout layout_;

	QLabel* label_;
	QLineEdit* edit_;

};


template<typename Type>
void
NamedLineEdit::set(const Angle<Type>& val)
{
	QString string;
	string.setNum(val.degrees());
	edit_->setText(string);
}

template<typename Type>
Angle<Type>
NamedLineEdit::getAngle()
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


#endif /* GROUND_STATION_INCLUDE_GROUND_STATION_WIDGETS_GENERICPROTO_NAMEDLINEEDIT_H_ */
