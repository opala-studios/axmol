//
//  OPWaveformWindowed.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 23/06/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPWaveformWindowed_hpp
#define OPWaveformWindowed_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <JuceHeader.h>
#include "OPMemoryPlayer.hpp"
#include "OPWaveformRange.hpp"

#include <FairyGUI.h>

class OPWaveform: public cocos2d::Ref
{
public:
    struct WindowZoomInfo
    {
        int startIndex;
        int startAudioPosition;
        int endIndex;
        float stepSize;
        int sizeInAudioSamples;
        int playerOffSetPosition;
    };

    typedef std::function<void(float audioPositionOnDraw)> SL_AudioDrawPostitionCallback;
    CC_SYNTHESIZE(SL_AudioDrawPostitionCallback, _audioDrawPostitionCallback, AudioDrawPostitionCallback);

    void update(float dt);
    bool init(fairygui::GObject* renderArea);
    static OPWaveform* create(fairygui::GObject* renderArea);

    ~OPWaveform();
    bool setupPlayerToRender(std::shared_ptr<OPMemoryPlayer> player);
    void setBaseWaveColor(cocos2d::Color4F);
    
    void setShouldDrawTopHalf(bool);
    void setShouldDrawBottomHalf(bool);

    OPWaveformRange* getRangeFromPosition(float start, float end);

    void setZoom(float);

    int getSampleFromPosition(float position);
    float getPositionFromSample(int sample);
    void fit(OPWaveformRange*, bool ifNeeded = true);

    void clearDraw();

    OPWaveformRange* addSubRegion(float start, float end, const cocos2d::Color4F& color);
    void removeSubRegion(OPWaveformRange*);
    void addSubRegion(OPWaveformRange*);
    void updateDraw();
    void updateDraw(int startSample);
    
    void drawBuffer(const float** buffer,int numberOfSamples);
    
    void moveZoomBy(float deltaPixels);
    void moveAudioPositionBy(float deltaPixels);
    void clearSubRanges();
    
    bool containsRange(OPWaveformRange* range);

    const WindowZoomInfo& getCurrentWindow() const;

protected:
    void updateBuffer();
    void updateBuffer(int startSample);
    void draw();
    cocos2d::Color4F getSubregionColor(int bufferIdx,cocos2d::Color4F defaultColor);
    
    int drawBufferIndexToAudioSampleIndex(int drawBufferIndex);
    
    WindowZoomInfo _currentWindow;
    
    std::vector<float> _drawBuffer;
    
    void drawBar(float x, float y, cocos2d::Color4F color);
    
    bool _shouldDrawTopHalf = true;
    bool _shouldDrawBottomHalf = true;
    
    float _currentZoom = 1.0f;
    
    int _drawSize = 10000;
    
    cocos2d::Color4F _baseWaveColor = cocos2d::Color4F::WHITE ;
   
    std::shared_ptr<OPMemoryPlayer> _player;
    cocos2d::Rect* _area;
    cocos2d::DrawNode* _drawNode;
    fairygui::GObject* _areaFairy;
        
    cocos2d::Vector <OPWaveformRange*>_subRegions;
        
    float getAudioPositionOnDraw();
    int getHowMuchTheAudioMovedSinceLastWindowUpdateNotLooping();
    
private:

};
#endif /* OPWaveformWindowed_hpp */
