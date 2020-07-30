/*
 * LineEdit.cpp
 *
 *  Created on: Jul 31, 2018
 *      Author: mircot
 */


#include <uavGS/ParameterSets/NamedVectorEdit.h>

NamedVectorEdit::NamedVectorEdit(const std::string& name, QWidget* parent):
QWidget(parent), layout_(this)
{
	label_ = new QLabel(QString::fromStdString(name), parent);
	editX_ = new QLineEdit(parent);
	editY_ = new QLineEdit(parent);
	editZ_ = new QLineEdit(parent);

	if (parent)
	{
		label_->setFont(parent->font());
		editX_->setFont(parent->font());
		editY_->setFont(parent->font());
		editZ_->setFont(parent->font());
	}

	layout_.addWidget(label_);
	layout_.addWidget(editX_);
	layout_.addWidget(editY_);
	layout_.addWidget(editZ_);
	layout_.setMargin(2);
}

NamedVectorEdit::~NamedVectorEdit()
{
	delete editX_;
	delete editY_;
	delete editZ_;
	delete label_;
}

Eigen::Vector3d
NamedVectorEdit::getDouble()
{
	return Eigen::Vector3d(editX_->text().toDouble(), editY_->text().toDouble(), editZ_->text().toDouble());
}

Eigen::Vector3f
NamedVectorEdit::getFloat()
{
	return Eigen::Vector3f(editX_->text().toFloat(), editY_->text().toFloat(), editZ_->text().toFloat());
}

Eigen::Vector3i
NamedVectorEdit::getInt()
{
	return Eigen::Vector3i(editX_->text().toInt(), editY_->text().toInt(), editZ_->text().toInt());
}

void
NamedVectorEdit::set(const Eigen::Vector3d&  val)
{
	QString string;
	string.setNum(val.x());
	editX_->setText(string);
	string.setNum(val.y());
	editY_->setText(string);
	string.setNum(val.z());
	editZ_->setText(string);
}

void
NamedVectorEdit::set(const Eigen::Vector3f&  val)
{
	QString string;
	string.setNum(val.x());
	editX_->setText(string);
	string.setNum(val.y());
	editY_->setText(string);
	string.setNum(val.z());
	editZ_->setText(string);
}

void
NamedVectorEdit::set(const Eigen::Vector3i&  val)
{
	QString string;
	string.setNum(val.x());
	editX_->setText(string);
	string.setNum(val.y());
	editY_->setText(string);
	string.setNum(val.z());
	editZ_->setText(string);
}

bool
NamedVectorEdit::isEmpty()
{
	// Treat as empty if not all values are set
	return editX_->text().isEmpty() || editY_->text().isEmpty() || editZ_->text().isEmpty();
}
