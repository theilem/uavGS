/*
 * LineEdit.h
 *
 *  Created on: Jul 31, 2018
 *      Author: mircot
 */

#ifndef UAVGS_PARAMETER_SETS_NAMED_VECTOR_EDIT_H
#define UAVGS_PARAMETER_SETS_NAMED_VECTOR_EDIT_H

#include <string>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include <Eigen/Dense>

class NamedVectorEdit: public QWidget
{
Q_OBJECT
public:

	explicit
	NamedVectorEdit(const std::string& name, QWidget* parent = 0);

	~NamedVectorEdit();

	Eigen::Vector3d
	getDouble();

	Eigen::Vector3f
	getFloat();

	Eigen::Vector3i
	getInt();

	bool
	isEmpty();

	void
	set(const Eigen::Vector3d& val);

	void
	set(const Eigen::Vector3f& val);

	void
	set(const Eigen::Vector3i& val);

private:

	QHBoxLayout layout_;

	QLabel* label_;
	QLineEdit* editX_;
	QLineEdit* editY_;
	QLineEdit* editZ_;

};

#endif /* UAVGS_PARAMETER_SETS_NAMED_VECTOR_EDIT_H */
