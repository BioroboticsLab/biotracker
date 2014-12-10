#ifndef TrackingAlgorithm_H
#define TrackingAlgorithm_H

#include <vector>
#include <memory>

#include <boost/optional.hpp>

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
	{	_currentFrameNumber = frameNumber;	}

	/**
	* receive Signal from GUI to set play mode 
	*/
	void setVideoPaused(bool isPause)
	{	_isVideoPaused = isPause;	}

	/**
	* receive current zoom level from VideoView
	*/
	void setZoomLevel(float zLevel)
	{	_currentZoomLevel = zLevel;	}

	/**
	* receive current image from TrackingThread
	*/
	void setCurrentImage(cv::Mat img)
	{	_currentImage = img;	}
	
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
	float _currentZoomLevel;

	/**
	 * @return either a copy of the current frame image, wrapped in a
	 * boost::optional, or a unintialized boost::optional, if there is no
	 * frame yet
	 */
	boost::optional<cv::Mat> getCurrentImageCopy() const;

	/**
	* will receive QMouseEvent as soon 
	* as mouse is getting moved in video view
	*/
	virtual void mouseMoveEvent		(QMouseEvent * /* e */){}

	/**
	* will receive QMouseEvent as soon
	* as any mouse button is pressed in video view
	*/
	virtual void mousePressEvent	(QMouseEvent * /* e */){}

	/**
	* will receive QMouseEvent as soon
	* as any mouse button is released in video view
	*/
	virtual void mouseReleaseEvent	(QMouseEvent * /* e */){}

	/**
	* will receive QMouseEvent as soon
	* as mouse wheel is activated in video view
	*/
	virtual void mouseWheelEvent	(QWheelEvent * /* e */){}


	/**
	* will receive QKeyEvent as soon
	* as any keyboard key from 'grabbedKeys()' is pressed in video view
	*/
	virtual void keyPressEvent		(QKeyEvent * /* e */){}

private:
	boost::optional<cv::Mat> _currentImage;
};

#endif // !TrackingAlgorithm_H
