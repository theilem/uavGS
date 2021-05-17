#ifndef SELECTIONVALUE_H
#define SELECTIONVALUE_H

#include <optional>
#include <unordered_set>

#include <QWidget>

#include <cpsCore/Utilities/LinearAlgebra.h>

namespace Ui
{
class SelectionValue;
}

class SelectionValue: public QWidget
{
Q_OBJECT

public:
	explicit SelectionValue(const std::string& key, QWidget* parent = nullptr);

	~SelectionValue();

	std::optional<std::pair<const std::string&, FloatingType>>
	get() const;

signals:

	void
	buttonClicked(const std::string& key, SelectionValue* wid);

private slots:

	void
	on_deleteButton_clicked();

private:
	Ui::SelectionValue* ui;
	std::string key_;
};

#endif // SELECTIONVALUE_H
