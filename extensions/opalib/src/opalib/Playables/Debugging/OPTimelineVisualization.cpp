#include <Playables/Debugging/OPTimelineVisualization.hpp>
#include <Misc/OPDirectory.hpp>
#include <Misc/OPFile.hpp>
#include <Misc/OPColor.hpp>
#include <boost/algorithm/hex.hpp>
#include <fstream>
#include <cocos2d.h>
#include <sstream>
#include <Math/OPRect.hpp>

void svgRect(
    std::stringstream& stream,
    const std::string& color,
    const OPRect& rect
) {
    stream
        << "<rect fill=\""
        << color
        << "\" x=\""
        << rect.xMin
        << "\" y=\""
        << rect.yMin
        << "\" height=\""
        << rect.getHeight()
        << "\" width=\""
        << rect.getWidth()
        << "\" opacity=\"0.65\"/>"
        << std::endl;;
}

void svgCircle(
    std::stringstream& stream,
    const std::string& color,
    float x,
    float y,
    float radius
) {
//    <circle r="5" cx="0" cy="512" fill="white"/>
    stream
        << "<circle fill=\""
        << color
        << "\" r=\""
        << radius
        << "\" cx=\""
        << x
        << "\" cy=\""
        << y
        << "\" opacity=\"0.65\"/>"
        << std::endl;
}

void svgLine(
    std::stringstream& stream,
    const std::string& color,
    float width,
    float x1,
    float y1,
    float x2,
    float y2
) {
//    <circle r="5" cx="0" cy="512" fill="white"/>
    stream
        << "<line x1=\""
        << x1
        << "\" y1=\""
        << y1
        << "\" x2=\""
        << x2
        << "\" y2=\""
        << y2
        << "\" stroke=\""
        << color
        << "\" stroke-width=\""
        << width
        << "\" opacity=\"0.65\"/>"
        << std::endl;
}

void svgText(
    std::stringstream& stream,
    const std::string& color,
    float x,
    float y,
    const std::string& text,
    float fontSize = -1
) {

    stream << "<text x=\"" << x << "\" y=\"" << y << "\" color=\"" << color;
    if (fontSize > 0) {
        stream << "\" font-size=\"" << fontSize;
    }
    stream << "\" font-family=\"monospace\">"
           << text
           << "</text>"
           << std::endl;
}

union OPColor32 {
    struct {
        uint8_t r, g, b, a;
    };
    uint32_t rgba;
};

std::string getColor(OPColor32 color) {
    uint8_t r, g, b;
    r = color.r;
    g = color.g;
    b = color.b;
    /// RRGGBBAA
    std::stringstream col;
    col << OPColor::toHex(r, g, b);
    return col.str();
}

void OPTimelineVisualization::saveAsSVG(
    const OPTimeline& timeline,
    const std::string& timelineName
) {
    std::stringstream str;
    const float boxSize = 1024;
    str
        << "<svg viewBox=\"0 0 "
        << boxSize
        << " "
        << boxSize
        << "\" width=\""
        << boxSize
        << "\" height=\""
        << boxSize
        << "\" xmlns=\"http://www.w3.org/2000/svg\">"
        << std::endl;
    int currentSequenceIndex = 0;
    float timelineStart = timeline.getStartTime();
    float timelineEnd = timeline.getEndTime();
    const auto& sequences = timeline.getSequences();
    const float rectHeight = boxSize / sequences.size();
    svgRect(str, "black", OPRect(0, boxSize, 0, boxSize));
    for (const OPSequence& sequence : sequences) {
        float from = OPMath::inv_lerp(
            timelineStart,
            timelineEnd,
            sequence.getStartTime());
        float to =
            OPMath::inv_lerp(timelineStart, timelineEnd, sequence.getEndTime());
        float yBasePosition = currentSequenceIndex * rectHeight;
        float xStart = OPMath::lerp<float>(0, boxSize, from);
        float xEnd = OPMath::lerp<float>(0, boxSize, to);
        OPRect sequenceRect(
            xStart,
            xEnd,
            yBasePosition,
            yBasePosition + rectHeight
        );
        OPColor32 col;
        col.rgba = rand();
        col.a = 127;
        svgRect(str, getColor(col), sequenceRect);
        svgText(
            str,
            "white",
            xStart,
            yBasePosition + (rectHeight / 2),
            sequence.getName());
        float middle = (xStart + xEnd) / 2;
        size_t playableIndex = 0;
        for (const std::shared_ptr<OPPlayable>
                & playable : sequence.getElements()) {
            float playableTextY = yBasePosition + (playableIndex * 16);
            svgText(
                str,
                "white",
                middle,
                playableTextY,
                playable->getName(),
                8
            );
            playableIndex++;
        }
        currentSequenceIndex++;
    }
    std::vector<std::pair<float, int>> _numTextsPerTime;

    for (const OPMarker& marker : timeline.getMarkers()) {
        float pos =
            OPMath::inv_lerp(timelineStart, timelineEnd, marker.getTime());
        float tgtPos = OPMath::lerp<float>(0, boxSize, pos);
        auto found = std::find_if(
            _numTextsPerTime.begin(), _numTextsPerTime.end(),
            [tgtPos](std::pair<float, int> pair) {
                return OPMath::approximately(pair.first, tgtPos);
            }
        );
        int baseHeight = 0;
        if (found == _numTextsPerTime.end()) {
            _numTextsPerTime.emplace_back(tgtPos, 1);
        } else {
            baseHeight = found->second++;
        }
        svgCircle(str, "white", tgtPos, 64, 5);
        svgLine(str, "white", 1, tgtPos, 0, tgtPos, boxSize);
        svgText(
            str,
            "white",
            tgtPos,
            80.0F + (baseHeight * 32.0F),
            marker.getName(),
            8
        );
    }
    float zeroPoint = OPMath::lerp<float>(
        0,
        boxSize,
        OPMath::inv_lerp<float>(timelineStart, timelineEnd, 0));
    svgLine(str, "red", 1, zeroPoint, 0, zeroPoint, boxSize);

    str << "</svg>";
    std::string filename = timelineName + ".svg";
    auto parent = OPDirectory::getWritablePath(filename, true);
    OPFile::writeString(str.str(), parent);
}
