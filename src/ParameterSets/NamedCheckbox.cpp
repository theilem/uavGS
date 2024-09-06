/*
 * NamedCheckbox.cpp
 *
 *  Created on: Jul 31, 2018
 *      Author: mircot
 */
#include <uavGS/ParameterSets/NamedCheckbox.h>


NamedCheckbox::NamedCheckbox(const std::string& name, QWidget* parent) :
		QWidget(parent), layout_(this)
{
	label_ = new QLabel(QString::fromStdString(name), parent);
	checkbox_ = new QCheckBox(parent);
	checkbox_->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	layout_.addWidget(label_);
	layout_.addWidget(checkbox_);
}

NamedCheckbox::~NamedCheckbox()
{
	delete label_;
	delete checkbox_;
}

bool
NamedCheckbox::get()
{
	return checkbox_->isChecked();
}

void
NamedCheckbox::set(bool val)
{
	checkbox_->setChecked(val);
}

void
NamedCheckbox::setFontColor(const QColor& color)
{
	label_->setStyleSheet("QLabel { color : " + color.name() + "; }");
}

QColor
NamedCheckbox::getFontColor() const
{
	return label_->palette().color(QPalette::WindowText);
}
