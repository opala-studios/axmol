//
//  ParticleGObject.cpp
//  BeatJump-mobile
//
//  Created by Allison Lindner on 19/08/21.
//

#include "ParticleGObject.hpp"

opalib::ParticleGObject::ParticleGObject(cocos2d::Vec2 pos) {
    _pos = pos;
	_rotation = CCRANDOM_MINUS1_1() * 60.0f;
}

void opalib::ParticleGObject::spawn()
{
	_currentState = ParticleState::SPAWN;
	setupCurve();
}

void opalib::ParticleGObject::update(float dt)
{
	switch(_currentState)
	{
        case ParticleState::NONE:
			break;
        case ParticleState::SPAWN:
			spawnUpdate(dt);
			break;
        case ParticleState::MOVE:
			moveUpdate(dt);
			break;
		case ParticleState::FINISHED:
			break;
	}

	if(_currentState != ParticleState::NONE)
	{
        _timelapse += dt;
        _totalTimelapse += dt;
		_currentLifetime += dt;
		
		float lifetime = 0.0f;
		if(_lifetime > 0.0f)
			lifetime = _currentLifetime/_lifetime;

		_alpha = _curveAlpha.evaluate(lifetime);
		
		if(_updateHandler)
			_updateHandler(_index, _pos, _rotation, _alpha);
    }
}

void opalib::ParticleGObject::reset() {
    _currentState = ParticleState::NONE;
    _timelapse = 0.0f;
    _totalTimelapse = 0.0f;
	_currentLifetime = 0.0f;
}

void opalib::ParticleGObject::setupCurve()
{
	switch(_currentState)
	{
		case ParticleState::NONE:
			break;
        case ParticleState::SPAWN:
			_curveX = _spawnCurveXHandler(_index, _pos.x, _spawnPos.x);
			_curveY = _spawnCurveYHandler(_index, _pos.y, _spawnPos.y);
			_curveRotation = OPCurve<float>::easeInOut(_rotation, _rotation + (CCRANDOM_MINUS1_1() * 360));
			_curveAlpha = _alphaCurveHandler(_index, _startAlpha, _endAlpha);
			break;
        case ParticleState::MOVE:
			_curveX = _moveCurveXHandler(_index, _pos.x, _destPos.x);
			_curveY = _moveCurveYHandler(_index, _pos.y, _destPos.y);
			break;
		case ParticleState::FINISHED:
			break;
	}
}

void opalib::ParticleGObject::spawnUpdate(float dt)
{
    particleMove(_spawnTotalTime);
	
	if(_timelapse/_spawnTotalTime > 1.0f)
	{
		_currentState = ParticleState::MOVE;
		_timelapse = 0.0f;
		setupCurve();
	}
}

void opalib::ParticleGObject::moveUpdate(float dt)
{
	if(_lifetime > 0.0f) {
		if(_currentLifetime >= _lifetime) {
			if(_lifetimeEndObjHandler)
				_lifetimeEndObjHandler(_index);
			
			_currentState = ParticleState::FINISHED;
			return;
		}
	}
	
	if(_timelapse/_moveTotalTime > 1.0f)
	{
		if(_reachEndObjHandler)
			_reachEndObjHandler(_index);
		
		_currentState = ParticleState::FINISHED;
		return;
	}
	
	particleMove(_moveTotalTime);
}

void opalib::ParticleGObject::particleMove(float totalTime)
{
    _pos.x = _curveX.evaluate(_timelapse/totalTime);
    _pos.y = _curveY.evaluate(_timelapse/totalTime);
    _rotation = _curveRotation.evaluate(_totalTimelapse/(_spawnTotalTime + _moveTotalTime));
}
