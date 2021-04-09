#ifndef WAVEFORMWRAPPER_H
#define WAVEFORMWRAPPER_H

#include <QWidget>

namespace Ui
{
class WaveformWrapper;
}

class WaveformWrapper: public QWidget
{
Q_OBJECT

public:
	explicit WaveformWrapper(const std::string& title, QWidget* parent = nullptr);

	~WaveformWrapper();

private:
	Ui::WaveformWrapper* ui;

signals:

	void
	buttonClicked(const std::string& key, WaveformWrapper* wid);

private slots:
	void
	on_selectWaveform_clicked();

	void
	on_deleteButton_clicked();
};

#endif // WAVEFORMWRAPPER_H
