//
//  PinchableComponent.cpp
//  OpalibTemplate-mobile
//
//  Created by Arthur Motelevicz on 18/07/20.
//

#include "./OPPinchableComponent.hpp"

using namespace fairygui;
USING_NS_CC;

void OPPinchableComponent::onConstruct()
{
    GComponent::onConstruct();

    addListeners();
}

void OPPinchableComponent::handleDrag(Vec2 pos)
{
    _currentPosition = pos;
    float deltaX = _currentPosition.x -_lastPosition.x;
    
    if(deltaX == 0) return;
    
    _lastPosition = _currentPosition;
    
    if(_dragCallback)
    {
        _dragCallback(deltaX);
    }
}

void OPPinchableComponent::handlePinch(InputEvent* evt)
{
    _fingerTouches.at(evt->getTouchId())->x = UIRoot->globalToLocal(evt->getPosition()).x;
    _fingerTouches.at(evt->getTouchId())->y = UIRoot->globalToLocal(evt->getPosition()).y;
    
    auto pinch = getPinch();
    _pinches.pushBack(pinch);
    
    if(_pinches.size() >= 2)
    {
        float deltaSize = (_pinches.at(1)->size - _pinches.at(0)->size);
        handleDrag(pinch->middlePos);
        
        _pinches.erase(_pinches.begin());
         if(deltaSize == 0) return;
        
        if(_pinchCallback)
        {
            _pinchCallback(deltaSize*_zoomSensibility);
        }
    }
    else
    {
        _currentPosition = pinch->middlePos;
        _lastPosition = pinch->middlePos;
    }
}

OPPinchableComponent::HorizontalPinch* OPPinchableComponent::getPinch()
{
    std::vector<float> xPoints;
    for(auto pair : _fingerTouches)
    {
       xPoints.push_back(pair.second->x);
    }

    float rightX = xPoints.at(0) > xPoints.at(1) ? xPoints.at(1) : xPoints.at(0);

    HorizontalPinch* pinch = HorizontalPinch::create();
    pinch->size =  abs(xPoints.at(0) - xPoints.at(1));
    pinch->middlePos.x = rightX + pinch->size/2 - getPosition().x;
    
    return pinch;
}

void OPPinchableComponent::setDragCallback(SL_DragCallback callback)
{
    _dragCallback = callback;
    _shouldHandleDrag = (bool)callback;
}

void OPPinchableComponent::setPinchCallback(SL_PinchCallback callback)
{
    _pinchCallback = callback;
    _shouldHandlePinch = (bool)callback;
}

void OPPinchableComponent::addListeners()
{
    addEventListener(UIEventType::TouchBegin, [this](EventContext* context)
    {
        if(!_isGesturesEnabled) return;
        InputEvent* evt = (InputEvent*)context->getInput();
        
        auto point =  FingerPoint::create();
        
        point->x = UIRoot->globalToLocal(evt->getPosition()).x;
        point->y = UIRoot->globalToLocal(evt->getPosition()).y;
        point->id = evt->getTouchId();
        
        _currentPosition = UIRoot->globalToLocal(evt->getPosition());
        _lastPosition = UIRoot->globalToLocal(evt->getPosition());
        
        _fingerTouches.insert(point->id,point);
        
        context->captureTouch();
    });
     
    addEventListener(UIEventType::TouchMove, [this](EventContext* context)
     {
        if(!_isGesturesEnabled) return;
        
        InputEvent* evt = (InputEvent*)context->getInput();
        
        if(_fingerTouches.size() >= 2)
        {
            _wasPinched = true;
            handlePinch(evt);
        }
        else if(_fingerTouches.size() == 1)
        {
            if(!_wasPinched)
                handleDrag(UIRoot->globalToLocal(evt->getPosition()));
        }
     });
     
     addEventListener(UIEventType::TouchEnd, [this](EventContext* context)
     {
         if(!_isGesturesEnabled) return;
         
          InputEvent* evt = (InputEvent*)context->getInput();
         _fingerTouches.erase(evt->getTouchId());
         if(_fingerTouches.size() == 0)
         {
             _wasPinched = false;
             _pinches.clear();
         }
     });
}

void OPPinchableComponent::setGesturesEnabled(bool shoulGesturing)
{
    _isGesturesEnabled = shoulGesturing;
}

void OPPinchableComponent::setZoomSensibility(float value)
{
    if(value < 1.0f)
        value = 1.0f;
    
    _zoomSensibility = value;
}

float OPPinchableComponent::getZoomSensibility()
{
    return _zoomSensibility;
}

OPPinchableComponent::~OPPinchableComponent()
{
    removeEventListeners();
}

