#ifndef SINEWAVEFORM_H
#define SINEWAVEFORM_H

#include <QWidget>

namespace Ui {
class SineWaveform;
}

class SineWaveform : public QWidget
{
	Q_OBJECT

public:
	explicit SineWaveform(QWidget *parent = nullptr, int index = -1);
	~SineWaveform();

	void setIdx(int index);

private:
	Ui::SineWaveform *ui;
};

#endif // SINEWAVEFORM_H
