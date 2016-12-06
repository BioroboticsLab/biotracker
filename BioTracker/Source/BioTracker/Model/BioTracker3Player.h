#ifndef BIOTRACKER3PLAYER_H
#define BIOTRACKER3PLAYER_H

#include <QObject>
#include "Interfaces/IModel/imodel.h"
#include "Interfaces/IModel/imodel.h"

#include "BioTracker3ImageStream.h"
#include <memory>
#include "QString"
#include "QMap"
#include "QThread"
#include "opencv2/core/core.hpp"

#include "View/BioTracker3VideoControllWidget.h"
#include "View/BioTracker3VideoView.h"

#include "IStates/IPlayerState.h"
#include "QSharedPointer"


class BioTracker3Player : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3Player(QObject *parent = 0);

    void setNextState(IPlayerState::PLAYER_STATES state);

  public Q_SLOTS:    
    void runPlayerOperation();

    void receiveLoadVideoCommand(QString fileDir);
    void receiveLoadPictures(std::vector<boost::filesystem::path> files);
    void receiveLoadCameraDevice(int i);
    void receiveActivateTracking();
    void receiveDeaktivateTracking();

    void receivePrevFrameCommand();
    void receiveNextFramCommand();
    void receivePauseCommand();
    void receiveStopCommand();
    void receivePlayCommand();

    void receiveStateDone();

    void receiveTrackingDone();


  Q_SIGNALS:
    void emitMediaType(GuiParam::MediaType type);
    void emitTotalNumbFrames(size_t num);
    void emitCurrentFrameNumber(size_t num);
    void emitFPS(double fps);
    void emitCurrentFileName(QString name);
    void emitCurrentFrameStr(std::shared_ptr<cv::Mat> mat, QString name);
    void emitCurrentFrame(std::shared_ptr<cv::Mat> mat);
    void emitVideoControllsStates(QVector<bool> states);

    void emitTrackingIsActiveState(bool state);

    void emitPlayerOperationDone();

  private:
    void updatePlayerParameter();
    void emitSignals();


  private:
    IPlayerState *m_CurrentPlayerState;
    IPlayerState *m_NextPlayerState;
    QThread m_StateThread;
    QMap<IPlayerState::PLAYER_STATES, IPlayerState *> m_States;
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;

    GuiParam::MediaType m_MediaType;
    size_t m_TotalNumbFrames;
    size_t m_CurrentFrameNumber;
    double m_fps;
    QString m_CurrentFilename;
    std::shared_ptr<cv::Mat> m_CurrentFrame;
    QVector<bool> m_VideoControllsStates;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;

    bool m_IsTrackingActive;

    QString m_NameOfCvMat = "Original";
};


#endif // BIOTRACKER3PLAYER_H
