#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <vector>
#include <memory>

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QMouseEvent>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "source/settings/Messages.h"
#include "source/tracking/trackedObject/TrackedObject.h"

class Settings;

class TrackingAlgorithm : public QObject
{
	Q_OBJECT

public:
	TrackingAlgorithm(Settings& settings, std::string& serializationPath, QWidget *parent);
	virtual	~TrackingAlgorithm();

	/**
	* This function tracks the provided object list within the provided frame.
	*/
	virtual void track(ulong frameNumber, cv::Mat& frame) = 0;

	/**
	* paint will be called by "VideoViews" paintGL method
	* so any picture manipulation stuff goes in here 
	*/
	virtual void paint(cv::Mat& image ) = 0;

	/**
	* Resets the tracker. never called - yet
	*/
	virtual void reset() = 0;

	/**
	* getToolsFrame() will be called once at start up
	* to create a widget for gui with all 
	* buttons needed for interaction 
	*/
	virtual std::shared_ptr<QWidget> getToolsWidget();

	/**
	* getParamsWidget() will be called once at start up
	* to create a widget for gui with all 
	* parameter fields needed 
	*/
	virtual std::shared_ptr<QWidget> getParamsWidget();

	/**
	 * @brief grabbedKeys() has to return a set of all keys that the
	 * algorithms needs access to. All KeyEvents with keys in the set will
	 * be forwarded to the algorithm
	 * @return const reference to the set of keys
	 */
	virtual std::set<Qt::Key> const& grabbedKeys() const;

	/**
	 * @brief prepareSave() is called once before the serialization of
	 * _trackedObjects. It should store or discard all temporary values
	 * that are related to tracked Objects.
	 */
	virtual void prepareSave();

	/**
	 * @brief prepareSave() is called once after the tracked objects are
	 * loaded from serialized data. It should do any postprocessing necessary.
	 *  the serialization of
	 */
	virtual void postLoad();
	
	void loadObjects(std::vector<TrackedObject> &&objects);
	std::vector<TrackedObject> const& getObjects();

public slots:
	/**
	* receive Signal to set current frame number
	*/
	void setCurrentFrameNumber(int frameNumber)
	{
		_currentFrameNumber = frameNumber;
	}

	/**
	* receive Signal from GUI to set play mode 
	*/
	void setVideoPaused(bool isPause)
	{
		_isVideoPaused = isPause;
	}
	
signals:
	/**
	* send a message to the GUI.
	*/
	void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);
	
	/**
	* send signal to VideoView and update display
	*/
	void update();

	cv::Mat requestCurrentScreen();

	void forceTracking();

protected:
	Settings & _settings;
	std::vector<TrackedObject> _trackedObjects;
	std::string _serializationPathName;


	bool event(QEvent* event) override;

	bool _isVideoPaused;
	int _currentFrameNumber;

private:
	virtual void mouseMoveEvent(QMouseEvent * e){}
	virtual void mousePressEvent(QMouseEvent * e){}
	virtual void mouseReleaseEvent(QMouseEvent * e){}
	virtual void mouseWheelEvent(QWheelEvent * e){}
	virtual void handleKeyPress(QKeyEvent * e){}
};

#endif // !TrackingAlgorithm_H
