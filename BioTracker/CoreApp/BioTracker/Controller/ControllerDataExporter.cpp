#include "ControllerDataExporter.h"
#include "Model/DataExporterCSV.h"
#include "settings/Settings.h"
#include "util/types.h"


ControllerDataExporter::ControllerDataExporter(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
}

ControllerDataExporter::~ControllerDataExporter()
{
}

void ControllerDataExporter::connectControllerToController() {
	//IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	//QPointer< MainWindow > mainWin = dynamic_cast<MainWindow*>(ctrM->getView());
	//mainWin->addVideoControllWidget(m_View);
}

void ControllerDataExporter::createModel() {

	//Grab the codec from config file
	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string exporter = exporterList[set->getValueOrDefault<int>(CFG_EXPORTER, 0)];
	if (exporter == "CSV")
		m_Model = new DataExporterCSV(this);
	else
		m_Model = 0;
}

void ControllerDataExporter::createView() {

	m_View = 0;
}

void ControllerDataExporter::connectModelToController() {
	IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	MediaPlayer* mplay = dynamic_cast<MediaPlayer*>(ctrM->getModel());

	QObject::connect(mplay, &MediaPlayer::fwdPlayerParameters, this, &ControllerDataExporter::rcvPlayerParameters);
}

void ControllerDataExporter::rcvPlayerParameters(playerParameters* parameters) {
	qobject_cast<IModelDataExporter*>(m_Model)->setFps(parameters->m_fpsSourceVideo);
	qobject_cast<IModelDataExporter*>(m_Model)->setTitle(parameters->m_CurrentTitle);
}