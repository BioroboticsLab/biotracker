#include "AreaInfo.h"

#include "Rectification.h"

#include "settings/Settings.h"
#include "util/misc.h"

AreaInfo::AreaInfo(QObject *parent) :
	IModelAreaDescriptor(parent)
{

	_rect = std::make_shared<AreaInfoElement>();
	_apperture = std::make_shared<AreaInfoElement>();

	std::vector<cv::Point> p = Rectification::instance().getDefaultArena();
	_rect->setVertices(p);
    _rect->setShowNumbers(true);
    _rect->setAreaType(BiotrackerTypes::AreaType::RECT);


	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string arena = _settings->getValueOrDefault<std::string>(AREADESCRIPTOR::CN_APPERTURE, "15,15;15,105;105,105;105,15");
	p = stringToCVPointVec(arena);
	_apperture->setVertices(p);
    _apperture->setAreaType(BiotrackerTypes::AreaType::APPERTURE);
}

void AreaInfo::updateRectification() {

	Rectification::instance().setArea(_rect->getQVertices());
	Rectification::instance().setupRecitification(100, 100, 2040, 2040);
}

void AreaInfo::setRectificationDimensions(double w, double h) {
	Rectification::instance().setDimension(w,h);
}

void AreaInfo::updateApperture() {
	std::vector<cv::Point> p = _apperture->getVertices();

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	_settings->setParam(AREADESCRIPTOR::CN_APPERTURE, cvPointsToString(p).c_str());
}

bool AreaInfo::inTrackingArea(cv::Point2f point_cm) {
	return _apperture->insideElement(point_cm);
}

cv::Point2f AreaInfo::pxToCm(cv::Point point_px) {
	return Rectification::instance().pxToCm(point_px);
}

cv::Point2f AreaInfo::cmToPx(cv::Point2f point_cm) {
	return Rectification::instance().cmToPx(point_cm);
}
