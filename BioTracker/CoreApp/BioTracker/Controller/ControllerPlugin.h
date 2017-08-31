/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef CONTROLLERPLUGIN_H
#define CONTROLLERPLUGIN_H

#include "Interfaces/IController/IController.h"
#include "Interfaces/IBioTrackerPlugin.h"
#include "QThread"

/**
 * This is the controller class of the Plugin Loader Component. This component is responsible for loading and managing BioTracker Plugins
 * The ControllerPlugin class is responsible for loading and connecting to BioTracker Plugins.
 */
class ControllerPlugin : public IController {
    Q_OBJECT
  public:
    ControllerPlugin(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
    ~ControllerPlugin();

    /**
     * (This function
     * This function delivers a file path of a Plugin to the IModel class PluginLoader. If the Plugin could be loaded,
     * the functions additionally requests the name of the Plugin and sets it as an entry into an IView widget in the MainWindow.
     */
    void loadPluginFromFileName(QString str);
    /**
     * This function hands the received cv::Mat pointer and the current frame number to the PluginLoader.
     */
    void sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number);

	void selectPlugin(QString str);

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;

    void createPlugin(QString instance);

    void connectPlugin();
    void disconnectPlugin();

  private Q_SLOTS:
    /**
     *
     * If Tracking is active and the tracking process was finished, the Plugin is able to emit a Signal that triggers this SLOT.
     */
    void receiveTrackingDone();

  private:
	void loadPluginsFromPluginSubfolder();

    IBioTrackerPlugin* m_BioTrackerPlugin;


    QPointer< QThread >  m_TrackingThread;



};

#endif // CONTROLLERPLUGIN_H
