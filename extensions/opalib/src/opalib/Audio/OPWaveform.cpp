//
//  OPWaveformWindowed.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 23/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "../Application/OPApplication.hpp"
#include "OPWaveform.hpp"
#include "../Debug/OPDebug.hpp"

using namespace juce;

bool OPWaveform::init(fairygui::GObject* renderArea)
{
    if (!renderArea) {
        return false;
    }

    _area = new cocos2d::Rect(0, 0, renderArea->getSize().width, renderArea->getSize().height);

    CC_SAFE_RELEASE_NULL(_areaFairy);
    _areaFairy = renderArea;
    _areaFairy->retain();

    _drawNode = cocos2d::DrawNode::create();
    _areaFairy->displayObject()->addChild(_drawNode);

    return true;
}

OPWaveform* OPWaveform::create(fairygui::GObject* renderArea)
{
    OPWaveform* pRet = new (std::nothrow) OPWaveform();
    if (pRet && pRet->init(renderArea)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool OPWaveform::setupPlayerToRender(std::shared_ptr<OPMemoryPlayer> player)
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);

    // safe checks
    if (!player || (player && !player->getAudioBuffer())) {
        CCASSERT(false, "Please check the inputs!");
        return false;
    }

    auto audioBuffer = player->getAudioBuffer();
    auto numSamples = audioBuffer->getSampleCount();

    if (player->getAudioBuffer() && numSamples < 1000) {
        CCASSERT(false, "Audio is too small!");
        return false;
    }

    _player = player;
    _drawSize = jmin(_drawSize, numSamples);
    _drawBuffer.resize(_drawSize);

    _drawNode->clear();

    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    return true;
}

void OPWaveform::draw()
{
    float resolution = _drawSize / _area->size.width;

    _drawNode->clear();
    for (int i = 0; i < _drawSize; i++) {
        float x = i / resolution;
        auto color = getSubregionColor(i, _baseWaveColor);
        float y = jmin(_drawBuffer[i]*_player->getVolume(),1.0f);
        y = jmax(y,-1.0f);
        drawBar(x, y, color);
    }
}

void OPWaveform::updateBuffer(int startSample) {
    auto audioBuffer = _player->getAudioBuffer();
    auto sampleBuffer = audioBuffer->getSampleBuffer();
    auto numSamples = audioBuffer->getSampleCount();
    float q = numSamples / _currentZoom;
    q = jmax(q, (float)_drawSize);

    if(q == _drawSize)
    {
        _currentZoom = numSamples/q;
    }

    float step = q/_drawSize;

    int audioOffset = q - (numSamples - startSample);
    audioOffset = jmax(audioOffset, 0);
    startSample -= audioOffset;

    startSample = jmin(startSample,numSamples-1);
    startSample = jmax(startSample,0);

    _currentWindow.stepSize = step;
    _currentWindow.startAudioPosition = startSample;

    for (int i = 0; i < _drawSize; i++) {
        int audioIdx = startSample + i * step;

        if (audioIdx < numSamples) {
            _drawBuffer[i] = sampleBuffer->getSample(0, audioIdx);
        } else {
            audioIdx = numSamples - 1;
            _drawBuffer[i] = 0;
        }

        if (i == 0) {
            _currentWindow.startIndex = audioIdx;
        }

        if (i == (_drawSize - 1)) {
            _currentWindow.endIndex = audioIdx;
        }
    }

    _currentWindow.sizeInAudioSamples = _currentWindow.endIndex - _currentWindow.startIndex;
}

void OPWaveform::updateBuffer()
{
    updateBuffer(_player->getCurrentPositionInSamples());
}

void OPWaveform::drawBar(float x, float y, cocos2d::Color4F color)
{
    if (!_shouldDrawTopHalf && y > 0) {
        y = 0;
    } else if (!_shouldDrawBottomHalf && y < 0) {
        y = 0;
    }

    float fixedX = _area->getMinX() + x;
    _drawNode->drawLine(cocos2d::Vec2(fixedX, _area->getMidY()),
                        cocos2d::Vec2(fixedX, _area->getMidY() + (_area->size.height / 2.0f * y)),
                        color);
}

void OPWaveform::setZoom(float val)
{
    if(val == _currentZoom) return;
    _currentZoom = val;
    _currentZoom = jmax(_currentZoom,1.0f);
    updateDraw();
}

void OPWaveform::setBaseWaveColor(cocos2d::Color4F color)
{
    _baseWaveColor = color;
    updateDraw();
}

void OPWaveform::setShouldDrawTopHalf(bool val)
{
    _shouldDrawTopHalf = val;
    updateDraw();
}

void OPWaveform::setShouldDrawBottomHalf(bool val)
{
    _shouldDrawBottomHalf = val;
    updateDraw();
}

void OPWaveform::update(float dt)
{
    getAudioPositionOnDraw();
}

OPWaveformRange* OPWaveform::addSubRegion(float start, float end, const cocos2d::Color4F& color)
{
    OPWaveformRange* range = getRangeFromPosition(start, end);

    range->setColor(color);

    std::cout << "startInSamples idx: " << std::to_string(range->getStartInSamples()) << std::endl;
    std::cout << "endInSamples: " << std::to_string(range->getEndInSamples()) << std::endl;

    CCASSERT(range->getStartInSamples() >= 0 && range->getEndInSamples() >= 0 && (range->getStartInSamples() != range->getEndInSamples()),
             "Problem with the selected indexes!");

    _subRegions.pushBack(range);

    draw();

    return range;
}

void OPWaveform::addSubRegion(OPWaveformRange* range)
{
     _subRegions.pushBack(range);
    draw();
}

void OPWaveform::removeSubRegion(OPWaveformRange* subRegion)
{
    _subRegions.eraseObject(subRegion);
    draw();
}

cocos2d::Color4F OPWaveform::getSubregionColor(int bufferIndex, cocos2d::Color4F defaultColor)
{
    cocos2d::Color4F color = defaultColor;

    for (auto s : _subRegions) {
        int audioIndex = drawBufferIndexToAudioSampleIndex(bufferIndex);

        if (s->containsIndex(audioIndex)) {
            color = s->getColor();
        }
    }

    return color;
}

int OPWaveform::drawBufferIndexToAudioSampleIndex(int drawBufferIndex)
{
    int audioSampleIndex;
    audioSampleIndex = drawBufferIndex * _currentWindow.stepSize + _currentWindow.startIndex;

    audioSampleIndex = jmin(audioSampleIndex, _currentWindow.endIndex);

    CCASSERT(audioSampleIndex < _player->getAudioBuffer()->getSampleCount(), "FUCKING Shit!");

    return audioSampleIndex;
}

void OPWaveform::updateDraw()
{
    updateBuffer();
    draw();
}

void OPWaveform::updateDraw(int startSample) {
    updateBuffer(startSample);
    draw();
}

float OPWaveform::getAudioPositionOnDraw()
{
    int audioDelta = getHowMuchTheAudioMovedSinceLastWindowUpdateNotLooping();
    float resp = ((float)audioDelta) / _currentWindow.sizeInAudioSamples;

    if (_audioDrawPostitionCallback) {
        _audioDrawPostitionCallback(resp);
    }

    return resp;
}

int OPWaveform::getHowMuchTheAudioMovedSinceLastWindowUpdateNotLooping()
{
    int R, Q, P;

    Q = _player->getCurrentPositionInSamples();
    P = _currentWindow.startAudioPosition;
    R = Q - P;

    return R;
}

void OPWaveform::drawBuffer(const float** buffer, int numberOfSamples)
{
    float resolutionDraw = _drawSize / _area->size.width;
    float resolutionAudio = _drawSize / (float)numberOfSamples;

    _drawNode->clear();
    for (int i = 0; i < _drawSize; i++) {
        float x = i / resolutionDraw;
        int bufferIdx = floor(i / resolutionAudio);

        auto color = getSubregionColor(i, _baseWaveColor);

        CCASSERT(bufferIdx < numberOfSamples, "Error! BufferIdx out of buffer.");

        drawBar(x, buffer[0][bufferIdx], color);
    }
}

OPWaveform::~OPWaveform()
{
    _drawNode->clear();
    _drawNode->removeFromParent();
    CC_SAFE_RELEASE_NULL(_areaFairy);
    cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

int OPWaveform::getSampleFromPosition(float position)
{
    int drawBuffer = _drawSize*position;
    int sampeIndex = drawBufferIndexToAudioSampleIndex(drawBuffer);
    return sampeIndex;
}

float OPWaveform::getPositionFromSample(int sample)
{
    float delta = sample - _currentWindow.startIndex;
    float size = _currentWindow.endIndex  - _currentWindow.startIndex;
    float position = delta/size;
    
    position = jmin(position,1.0f);
    position = jmax(position,0.0f);
    
    return position;
}

 void OPWaveform::fit(OPWaveformRange* range, bool ifNeeded)
{
    float size = range->getEndInSamples() - range->getStartInSamples();
    float fixedZoom = _currentWindow.sizeInAudioSamples/size;
    
    if (ifNeeded && ((fixedZoom > _currentZoom) || size < _currentWindow.sizeInAudioSamples)){
        return;
    }
    
    _currentZoom *= fixedZoom;
    if(_currentZoom < 1.0f)
        _currentZoom = 1.0f;
}

OPWaveformRange* OPWaveform::getRangeFromPosition(float start, float end)
{
    OPWaveformRange* selection = OPWaveformRange::create();

    int startInSamples = getSampleFromPosition(start);
    int endInSamples = getSampleFromPosition(end);

    selection->setup(startInSamples, endInSamples);

    return selection;
}

void OPWaveform::moveAudioPositionBy(float deltaPixels)
{
    if(_player->isPlaying()) return;
    
    float resolution = _drawSize/_area->size.width;
    int samplesToMove = floor(deltaPixels*resolution*_currentWindow.stepSize);
    
    int newPos = _currentWindow.startIndex + samplesToMove;
    
    int endLimit = _player->getAudioBuffer()->getSampleCount() - _drawSize*_currentWindow.stepSize;
    int startLimit = 0;
    
    if(newPos > endLimit)
        newPos = endLimit;
    
    if(newPos < startLimit)
        newPos = startLimit;
    
    _player->setCurrentPositionInSamples(newPos);
    updateDraw();
}

void OPWaveform::moveZoomBy(float deltaPixels)
{
    float resolution = _drawSize/_area->size.width;
    float newZoom = _currentZoom + (deltaPixels*resolution*_currentWindow.stepSize)/_currentWindow.sizeInAudioSamples;
    setZoom(newZoom);
}

void OPWaveform::clearSubRanges()
{
    _subRegions.clear();
    draw();
}

bool OPWaveform::containsRange(OPWaveformRange* range)
{
    return _subRegions.contains(range);
}

void OPWaveform::clearDraw()
{
    _drawNode->clear();
}

const OPWaveform::WindowZoomInfo& OPWaveform::getCurrentWindow() const
{
    return _currentWindow;
}

