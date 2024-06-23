//
//  PinchableComponent.hpp
//  OpalibTemplate-mobile
//
//  Created by Arthur Motelevicz on 18/07/20.
//

#ifndef PinchableComponent_hpp
#define PinchableComponent_hpp

#include <FairyGUI.h>
#include <cocos2d.h>

class OPPinchableComponent : public fairygui::GComponent
{
public:
    class FingerPoint: public cocos2d::Ref
    {
    public:
       bool init(){return true;};
       CREATE_FUNC(FingerPoint);
       ~FingerPoint(){};

       float x;
       float y;
       int id;
    };

    class HorizontalPinch: public cocos2d::Ref
    {
    public:
       bool init(){return true;};
       CREATE_FUNC(HorizontalPinch);
       ~HorizontalPinch(){};

      cocos2d::Vec2 middlePos;
       float size;
    };

    typedef std::function<void(float delta)> SL_DragCallback;
    typedef std::function<void(float delta)> SL_PinchCallback;
    
    CREATE_FUNC(OPPinchableComponent);
    virtual ~OPPinchableComponent();
    virtual void onConstruct() override;
    void setDragCallback(SL_DragCallback callback);
    void setPinchCallback(SL_PinchCallback callback);
    
    void setGesturesEnabled(bool);
    bool isGesturesEnabled();
    void setZoomSensibility(float value);
    float getZoomSensibility();

protected:
    //to handle pinch/drag_pinch
    cocos2d::Map<int,FingerPoint*> _fingerTouches;
    cocos2d::Vector<HorizontalPinch*> _pinches;
    void movePlayerPosition();
    HorizontalPinch* getPinch();

    //to handle simple drag
    cocos2d::Vec2 _currentPosition, _lastPosition;

    void handlePinch(fairygui::InputEvent*);
    void handleDrag(cocos2d::Vec2 pos);
    
private:
    float _zoomSensibility = 10.0f;

    void addListeners();
    bool _wasPinched = false;
    bool _shouldHandleDrag = false;
    bool _shouldHandlePinch = false;
    
    bool _isGesturesEnabled = true;
    
    SL_DragCallback _dragCallback;
    SL_PinchCallback _pinchCallback;
};
#endif /* PinchableComponent_hpp */
