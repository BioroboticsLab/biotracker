//Class used to generate ground truth data for BeesBook
// _newGrid			-- CTRL + RIGHT Click
// _modPosGrid		-- ALT + LEFT Button
// _modSizeGrid		-- SHIFT + LEFT Button
// _modAngleTag		-- CTRL + LEFT Button
// _modTiltGrid		-- SHIFT + RIGHT Button
// _modAngleGrid	-- ALT + RIGHT Button

#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include "cv.h"
#include "source/settings/Settings.h"
#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myGrid.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/myNewGrid.h"

class BeesBookTagMatcher : public TrackingAlgorithm
{
	private:
				
		myNewGrid			g; //active Grid
		myNewGrid			gtemp; //Grids already in vector

		std::vector<myNewGrid> _Grids; //vector of already set grids
		

		bool _newGrid; //ready to generate a new grid

		bool _activeGrid; //a new Grid has been set and can now be modified
		bool _activeTag; //a new Grid has been set and the Tag perimeter can now be modified

		bool _modPosGrid; //modify position of active Grid
		bool _modPosTag; //modify position of active Tag

		bool _modHeightInn; //modify height Inner circle and angleGrid
		bool _modWidthInn; //modify width Inner circle and angleGrid
		bool _modHeightMid; //modify height Middle circle and angleGrid
		bool _modWidthMid; //modify width Middle circle and angleGrid
		bool _modHeightOut; //modify height Outer circle and angleTag
		bool _modWidthOut; //modify width Outer circle and angleTag
		//------ NEW FLAGS
		bool _ready; //Ready for a new tag --ctrl + Right Click--
		bool _activePoints; //a new set of points is being configured
		bool _setP0; //Set P0 --Left Click--
		bool _setP1; //Set P1 --Left Click--
		bool _setP2; //Set P2 --Left Click--
		bool _setP3; //Set P3 --Left Click--
		bool _setP4; //Set P4 --Left Click--		

		cv::Point diff; //auxiliar variable
		cv::Point prevPosition; //auxiliar variable

		// FUNCTIONS
		void drawGrid(cv::Mat image); //function that draws a grid calling an instance of MyNewGrid
		void drawPoints(cv::Mat image); //function that draws the points while being edited
		double dist(cv::Point p1, cv::Point p2); //function that calculates the distance between two points
		
	public:
        BeesBookTagMatcher	( Settings &settings, std::string &serializationPathName, QWidget *parent );
		~BeesBookTagMatcher	( void );




		void track			( ulong frameNumber, cv::Mat & frame );
		void paint			( cv::Mat& image );
		void reset			( );


	public slots:
		//mouse click and move events
		void mouseMoveEvent		( QMouseEvent * e );
		void mousePressEvent	( QMouseEvent * e );
		void mouseReleaseEvent	( QMouseEvent * e );
		void mouseWheelEvent	( QWheelEvent * e );

};

#endif
