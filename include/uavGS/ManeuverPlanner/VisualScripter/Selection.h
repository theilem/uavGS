#ifndef SELECTION_H
#define SELECTION_H

#include <QWidget>

namespace Ui
{
class Selection;
}

class Selection: public QWidget
{
Q_OBJECT

public:
	explicit Selection(const std::string& key, QWidget* parent = nullptr);

	~Selection();

	const std::string&
	get() const;

signals:

	void
	buttonClicked(const std::string& key, Selection* wid);

private slots:

	void
	on_deleteButton_clicked();

private:
	Ui::Selection* ui;
	std::string key_;
};

#endif // SELECTION_H
