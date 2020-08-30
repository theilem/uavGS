/*
 * LineEdit.cpp
 *
 *  Created on: Jul 31, 2018
 *      Author: mircot
 */


#include <uavGS/ParameterSets/LabeledValue.h>

LabeledValue::LabeledValue(const std::string& name, QWidget* parent):
QWidget(parent), layout_(this)
{
	label_ = new QLabel(QString::fromStdString(name), parent);
	value_ = new QLabel(parent);

	if (parent)
	{
		label_->setFont(parent->font());
		value_->setFont(parent->font());
	}

	layout_.addWidget(label_);
	layout_.addWidget(value_);
	layout_.setMargin(2);
}

LabeledValue::~LabeledValue()
{
	delete value_;
	delete label_;
}

double
LabeledValue::getDouble()
{
	return value_->text().toDouble();
}

float
LabeledValue::getFloat()
{
	return value_->text().toFloat();
}

int
LabeledValue::getInt()
{
	return value_->text().toInt();
}

std::string
LabeledValue::getString()
{
	return value_->text().toStdString();
}

void
LabeledValue::set(double val)
{
	QString string;
	string.setNum(val);
	value_->setText(string);
}

void
LabeledValue::set(float val)
{
	QString string;
	string.setNum(val);
	value_->setText(string);
}

void
LabeledValue::set(int val)
{
	QString string;
	string.setNum(val);
	value_->setText(string);
}

void
LabeledValue::set(const std::string& val)
{
	value_->text() = QString::fromStdString(val);
}

bool
LabeledValue::isEmpty()
{
	return value_->text().isEmpty();
}
