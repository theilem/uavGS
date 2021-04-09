#ifndef SQUAREWAVEFORM_H
#define SQUAREWAVEFORM_H

#include <QWidget>

namespace Ui {
class SquareWaveform;
}

class SquareWaveform : public QWidget
{
	Q_OBJECT

public:
	explicit SquareWaveform(QWidget *parent = nullptr);
	~SquareWaveform();

private:
	Ui::SquareWaveform *ui;
};

#endif // SQUAREWAVEFORM_H
