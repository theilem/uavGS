#include <uavGS/ManeuverPlanner/VisualScripter/WidgetMotionPrimitive.h>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "uavGS/ManeuverPlanner/VisualScripter/WidgetVisualScripter.h"
#include "ui_WidgetVisualScripter.h"

WidgetVisualScripter::WidgetVisualScripter(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WidgetVisualScripter)
{
	ui->setupUi(this);
}

WidgetVisualScripter::~WidgetVisualScripter()
{
	delete ui;
}

void
WidgetVisualScripter::on_add_clicked()
{
	if (auto pm = get<IOverridesProvider>()){
		auto wmp = new WidgetMotionPrimitive(*pm, primitives_.size(), this);
		primitives_.push_back(wmp);
		QObject::connect(wmp, &WidgetMotionPrimitive::closeClicked, this, &WidgetVisualScripter::deletePrimitive);
		ui->scrollLayout->addWidget(wmp);
	} else {
		CPSLOG_ERROR << "Missing Planning Manager!";
	}
}

void
WidgetVisualScripter::on_save_clicked()
{
	/* NOTE: THis utilitizes some copied code from WidgetCPSGrid, namely in asking for json. Maybe we could make a json
	 * and file I/O utils helper
	 */

	// Ask for Flight Analysis
	QFileDialog dialog;
	dialog.setWindowTitle("Select Target FLight Analysis JSON");
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	QString filename;
	if (dialog.exec())
	{
		filename = dialog.selectedFiles().front();
	}
	else
	{
		CPSLOG_TRACE << "Cancelled json save.";
		return;
	}
	QFile loadFile(filename);
	if (!loadFile.open(QIODevice::ReadOnly))
	{
		CPSLOG_ERROR << "Could not open " << filename.toStdString() << " for loading.";
		return;
	}

	// Load Flight Analysis json data
	QByteArray confData = loadFile.readAll();
	loadFile.close();
	QJsonDocument loadDoc(QJsonDocument::fromJson(confData));
	QJsonObject loadData = loadDoc.object();

	// Get correct fields for maneuver sets
	auto mp = loadData["maneuver_planner"].toObject();
	auto ms = mp["maneuver_sets"].toObject();

	// Create new json maneuver set data from primitive widgets
	QJsonArray primitives;
	for (const auto &p : primitives_)
	{
		primitives.push_back(p->get());
	}

	// Create new maneuver set json object
	QJsonObject maneuver;
	maneuver["maneuvers"] = primitives;
	maneuver["save_as"] = ui->name->text() + ".txt";

	// Insert into correct place in flight analysis json
	ms[ui->name->text()] = maneuver;
	mp["maneuver_sets"] = ms;
	loadData["maneuver_planner"] = mp;

	// Write back to flight analysis json
	QJsonDocument d(loadData);
	if (!loadFile.open(QIODevice::WriteOnly))
	{
		CPSLOG_ERROR << "Could not open " << filename.toStdString() << " for writing.";
		return;
	}
	loadFile.write(d.toJson());
	loadFile.close();
}


void
WidgetVisualScripter::deletePrimitive(WidgetMotionPrimitive* toDelete)
{
	ui->scrollLayout->removeWidget(toDelete);
	auto deleteIdx = std::find(primitives_.begin(), primitives_.end(), toDelete);
	if (deleteIdx != primitives_.end())
	{
		primitives_.erase(deleteIdx);
		delete toDelete;

	}
	else
		CPSLOG_ERROR << "deletePrimitive called on unknown entry";

	unsigned renumIdx = deleteIdx - primitives_.begin();
	for (auto idx = renumIdx; idx < primitives_.size(); idx++)
	{
		primitives_[idx]->setIdx(idx);
	}
}