#include "ControllerPlugin.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "QDebug"
#include "Model/PluginLoader.h"

ControllerPlugin::ControllerPlugin(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerPlugin::loadPluginFromFileName(QString str)
{
    PluginLoader *loader = qobject_cast<PluginLoader *>(m_Model);
    if( loader->loadPluginFromFilename(str))
        createPlugin();
}

void ControllerPlugin::createModel()
{
    m_Model = new PluginLoader(this);
}

void ControllerPlugin::createView()
{
}

void ControllerPlugin::connectModelController()
{
}

void ControllerPlugin::connectController()
{

}

void ControllerPlugin::createPlugin()
{
    m_BioTrackerPlugin = qobject_cast<PluginLoader *>(m_Model)->getPluginInstance();
    m_BioTrackerPlugin->createPlugin();

    connectPlugin();
}

void ControllerPlugin::connectPlugin()
{
    IController * ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer *>(ctrA);

    IModel *model = ctrPlayer->getModel();
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);


    IController * ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject *ctrTexture = qobject_cast<ControllerTextureObject *>(ctrB);

    QObject *obj = dynamic_cast<QObject *>(m_BioTrackerPlugin);

    QObject::connect(obj, SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>,QString)),
                     ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>,QString)));
    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, this, &ControllerPlugin::receiveCurrentFrame);
    QObject::connect(obj, SIGNAL(emitTrackingDone()), ctrPlayer, SLOT(receiveTrackingOperationDone()));
}

void ControllerPlugin::disconnectPlugin()
{
    IController * ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer *>(ctrA);

    IModel *model = ctrPlayer->getModel();
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);


    IController * ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTexture = qobject_cast<ControllerTextureObject *>(ctrB);


//    QObject::disconnect(qobject_cast<QObject*> (m_BioTrackerPlugin), SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>,QString)), ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>,QString)));


//    QObject::disconnect(player, SIGNAL(emitCurrentFrame(std::shared_ptr<cv::Mat>)), qobject_cast<QObject*> (m_BioTrackerPlugin), SLOT(receiveCvMat(std::shared_ptr<cv::Mat>)));

}

void ControllerPlugin::receiveCurrentFrame(std::shared_ptr<cv::Mat> mat)
{
    m_BioTrackerPlugin->receiveCvMat(mat);
}

void ControllerPlugin::receiveTrackingDone()
{

}
