#ifndef SELECTIONVALUE_H
#define SELECTIONVALUE_H

#include <QWidget>
#include <unordered_set>

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

	std::pair<const std::string&, FloatingType>
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
