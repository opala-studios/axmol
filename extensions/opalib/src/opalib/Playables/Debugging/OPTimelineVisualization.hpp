#ifndef OPALIB_CINEMATICS_OPTIMELINEVISUALIZATION_H
#define OPALIB_CINEMATICS_OPTIMELINEVISUALIZATION_H
#include <Playables/OPPlayable.hpp>

class OPTimelineVisualization {
public:
    static void saveAsSVG(const OPTimeline& timeline, const std::string& timelineName);
};
#endif