//
// Created by bruno on 31/05/2021.
//
#include <Playables/Transformations/OPInterpolatePlayable.hpp>
#include <utility>
#include <opalib/Utils/OPTransform.hpp>

void OPInterpolatePlayable::evaluate(OPFrameTimes times) {
    if (keyframes.size() == 1) {

        const OPInterpolationKeyframe& uniqueKeyframe = keyframes[0];
        cocos2d::Vec2 pos = uniqueKeyframe.position.get();
        if (!uniqueKeyframe.blockOffset) {
            pos += offset.evaluate(1);
        }
        object->setPosition(pos.x, pos.y);
        return;
    }
    for (int i = 1; i < keyframes.size(); ++i) {
        OPInterpolationKeyframe& toKeyframe = keyframes[i];
        float nextTime = toKeyframe.time;
        if (nextTime < times.time) {
            continue;
        }
        OPInterpolationKeyframe& fromKeyframe = keyframes[i - 1];
        float keyframeTime = fromKeyframe.time;
        float offsetProgress =
            getInterpolationValue(nextTime, keyframeTime, times, offsetType);
        const cocos2d::Vec2& from = fromKeyframe.position.get();
        const cocos2d::Vec2& to = toKeyframe.position.get();
        float interpolationProgress = OPMath::clamp01<float>(
            getInterpolationValue(
                nextTime,
                keyframeTime,
                times,
                interpolationType
            )
        );
        cocos2d::Vec2 pos = OPMath::lerp(from, to, interpolationProgress);
        if (!fromKeyframe.blockOffset) {
            pos += offset.evaluate(offsetProgress);
        }
        object->setPosition(pos.x, pos.y);
        break;
    }
}

float OPInterpolatePlayable::getInterpolationValue(
    float nextTime,
    float keyframeTime,
    const OPFrameTimes& times,
    const OPFrameTimeType& type
) const {
    float progress;
    float relative = OPMath::inv_lerp(keyframeTime, nextTime, times.time);
    float absolute = times.time;
    switch (type) {
        case RELATIVE:
            progress = relative;
            break;
        case ABSOLUTE:
            progress = absolute;
            break;
    }
    return progress;
}

OPInterpolatePlayable::OPInterpolatePlayable(
    const cocos2d::Vec2& from,
    const cocos2d::Vec2& to,
    fairygui::GObject* object,
    OPFrameTimeType offsetType,
    OPFrameTimeType interpolationType
) :
    OPInterpolatePlayable(
        OPPositionAnchor(from),
        OPPositionAnchor(to),
        object,
        OPCurve<float>::linear(0, 1),
        OPCurve<cocos2d::Vec2>::zero(),
        offsetType,
        interpolationType
    ) {}

OPInterpolatePlayable::OPInterpolatePlayable(
    fairygui::GObject* from,
    fairygui::GObject* to,
    fairygui::GObject* object,
    OPFrameTimeType offsetType,
    OPFrameTimeType interpolationType
) :
    OPInterpolatePlayable(
        OPPositionAnchor(from),
        OPPositionAnchor(to),
        object,
        OPCurve<float>::linear(0, 1),
        OPCurve<cocos2d::Vec2>::zero(),
        offsetType,
        interpolationType
    ) {}

OPInterpolatePlayable::OPInterpolatePlayable(
    std::vector<OPInterpolationKeyframe> keyframes,
    fairygui::GObject* object,
    const OPCurve<cocos2d::Vec2>& offset,
    const OPCurve<float>& interpolation,
    OPFrameTimeType offsetType,
    OPFrameTimeType interpolationType
) :
    keyframes(std::move(keyframes)),
    object(object),
    offset(offset),
    interpolation(interpolation),
    offsetType(offsetType),
    interpolationType(interpolationType) {
    std::sort(
        OPInterpolatePlayable::keyframes.begin(),
        OPInterpolatePlayable::keyframes.end(),
        [](
            const OPInterpolatePlayable::OPInterpolationKeyframe& first,
            const OPInterpolatePlayable::OPInterpolationKeyframe& second
        ) {
            return first.time < second.time;
        }
    );
}

OPInterpolatePlayable::OPInterpolatePlayable(
    OPPositionAnchor from,
    OPPositionAnchor to,
    fairygui::GObject* object,
    const OPCurve<float>& interpolation,
    const OPCurve<cocos2d::Vec2>& offset,
    OPFrameTimeType offsetType,
    OPFrameTimeType interpolationType
) :
    OPInterpolatePlayable(
        std::vector<OPInterpolationKeyframe>(
            {
                {
                    .time = 0,
                    .position = std::move(from)
                },
                {
                    .time = 1,
                    .position = std::move(to)
                }
            }
        ),
        object,
        offset,
        interpolation,
        offsetType,
        interpolationType
    ) {}

OPPositionAnchor::OPPositionAnchor(
    const cocos2d::Vec2& pos,
    cocos2d::Vec2 offset
) :
    OPAnchor(pos),
    offset(offset) {}

OPPositionAnchor::OPPositionAnchor(
    fairygui::GObject* anchor,
    cocos2d::Vec2 offset,
    fairygui::GObject* relativeTo
) :
    OPAnchor(anchor, relativeTo),
    offset(offset) {}

cocos2d::Vec2 OPPositionAnchor::extract(fairygui::GObject* object) const {
    cocos2d::Vec2 basePos;
    if (relativeTo != nullptr) {
        basePos = OPTransform::getPositionRelativeTo(object, relativeTo);
    } else {
        basePos = object->getPosition();
    }
    return basePos + offset;
}

OPPositionAnchor::OPPositionAnchor() :
    OPAnchor<cocos2d::Vec2>() {
}
