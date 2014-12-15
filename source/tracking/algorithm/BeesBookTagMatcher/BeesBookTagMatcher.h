#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include <chrono>
#include <cstdint>
#include <memory>

#include <opencv2/opencv.hpp>

#include <boost/optional.hpp>

#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/Grid.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/Grid3D.h"

#include "ui_BeesBookTagMatcherToolWidget.h"

//Class used to generate ground truth data for BeesBook
class BeesBookTagMatcher : public TrackingAlgorithm {
	Q_OBJECT
private:
	std::shared_ptr<Grid3D>	_activeGrid; // points to active grid (grid must be active to be altered)
	boost::optional<ulong>	_activeFrameNumber;




	enum class State : uint8_t {
		Ready = 0, // Ready for a new tag --Ctrl + LCM--
		SetTag,    // a new set of points is being configured
		SetP0,     // Set P0 --Left Click and drag--
		SetP1,     // Set P1 --Left Click and drag--
		SetP2,     // Set P2 --Left Click and drag--
		SetBit,		//  Set single bits
	};

	State _currentState;
	bool _setOnlyOrient; // to modify exclusively the tag orientation.

	cv::Point2f				_rotationAxis;
	cv::Point2f				_tempPoint;

	std::vector<cv::Point> _orient; // auxiliar variable for drawing the orientation while setting the Tag

	std::chrono::system_clock::time_point _lastMouseEventTime;

	Ui::TagMatcherToolWidget _UiToolWidget;

	std::shared_ptr<QWidget> _toolWidget;
	std::shared_ptr<QWidget> _paramWidget;

	std::set<size_t> _idCopyBuffer;
	boost::optional<size_t> _copyFromFrame;

	/**
	* TEST CODE START
	* ------------------------------------------
	*/
	Grid3D _testGrid3d;
	/**
	* ------------------------------------------
	* TEST CODE END
	*/


	// function that draws the Tags set so far calling instances of Grid.
	void drawSetTags(cv::Mat &image) const;

	// function that draws the orientation vector while being set.
	void drawOrientation(cv::Mat& image, const std::vector<cv::Point>& _orient) const;

	// function that draws an active tag calling an instance of Grid
	void drawActiveTag(cv::Mat& image) const;

	// function that draws the tag while being rotated in space
	void drawSettingTheta(cv::Mat &img) const;

	// function called while setting the tag (it initializes orient vector)
	void setTag(const cv::Point& location);

	// function that allows P1 and P2 to be modified to calculate the tag's angle in space.
	void setTheta(const cv::Point& location);

	// function that checks if one of the set Points is selected, returns true when one of the points is selected
	bool selectPoint(const cv::Point& location);

	// function that checks if one of the already set Tags is selected.
	void selectTag(const cv::Point& location);

	// set no tas as currently active
	void cancelTag();

	void removeCurrentActiveTag();

	// function that calculates the distance between two points
	double dist(const cv::Point& p1, const cv::Point& p2) const;

	double getAlpha() const;

	void setNumTags();

	std::set<Qt::Key> const& grabbedKeys() const override;

	void mouseMoveEvent		(QMouseEvent * e) override;
	void mousePressEvent	(QMouseEvent * e) override;
	void mouseReleaseEvent	(QMouseEvent * e) override;
	void keyPressEvent		(QKeyEvent * e) override;

protected:
	bool event(QEvent* event) override;

public:
	BeesBookTagMatcher(Settings &settings, QWidget *parent);
	~BeesBookTagMatcher();

	void track(ulong frameNumber, cv::Mat& frame) override;
	void paint(cv::Mat& image) override;
	void reset() override;
	void postLoad() override;

	std::shared_ptr<QWidget> getToolsWidget() override { return _toolWidget; }
	std::shared_ptr<QWidget> getParamsWidget() override { return _paramWidget; }
};

#endif
