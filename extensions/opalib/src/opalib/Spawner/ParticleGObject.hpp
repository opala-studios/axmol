//
//  ParticleGObject.hpp
//  BeatJump-mobile
//
//  Created by Allison Lindner on 19/08/21.
//

#ifndef SpawnGObject_hpp
#define SpawnGObject_hpp

#include <cocos2d.h>
#include "../Math/OPCurve.hpp"

namespace opalib
{

typedef std::function<void(int index, cocos2d::Vec2, float rotation, float alpha)> 	SEL_positioningHandler;
typedef std::function<void(int index)>												SEL_endObjHandler;
typedef std::function<OPCurve<float>(int index, float from, float to)>				SEL_particleCurveHandler;

enum class ParticleState
{
	NONE,
	SPAWN,
	MOVE,
	FINISHED
};

class ParticleGObject
{
public:
	ParticleGObject() = default;
	ParticleGObject(cocos2d::Vec2 pos);
	
	void spawn();
	void update(float dt);
	void reset();

	void setIndex(int index) { _index = index; }
	int getIndex() { return _index; }

	cocos2d::Vec2 getPosition() { return _pos; }
	float getRotation() { return _rotation; }
	float getAlpha() { return _alpha; }
	void setAlphaCurveValues(float startAlpha, float endAlpha) { _startAlpha = startAlpha; _endAlpha = endAlpha; }
	void setPosition(cocos2d::Vec2 pos) { _pos = pos; }
	void setStartAlpha(float alpha) { _startAlpha = alpha; }
	void setEndAlpha(float alpha) { _endAlpha = alpha; }
	void setDestPosition(cocos2d::Vec2 pos) { _destPos = pos; }
	void setSpawnPosition(cocos2d::Vec2 pos) { _spawnPos = pos; }
	void setSpawnTotalTime(float time) { _spawnTotalTime = time; }
	void setMoveTotalTime(float time) { _moveTotalTime = time; }
	void setLifetime(float lifetime) { _lifetime = lifetime; }
	
	void setUpdateHandler(SEL_positioningHandler handler) { _updateHandler = handler; }
	void setReachEndHandler(SEL_endObjHandler handler) { _reachEndObjHandler = handler; }
	void setLifetimeEndHandler(SEL_endObjHandler handler) { _lifetimeEndObjHandler = handler; }

	void setSpawnCurveHandler(SEL_particleCurveHandler handlerX, SEL_particleCurveHandler handlerY) {
		_spawnCurveXHandler = handlerX;
		_spawnCurveYHandler = handlerY;
	}
	void setMoveCurveHandler(SEL_particleCurveHandler handlerX, SEL_particleCurveHandler handlerY) {
		_moveCurveXHandler = handlerX;
		_moveCurveYHandler = handlerY;
	}
	
	void setAlphaCurveHandler(SEL_particleCurveHandler handler) { _alphaCurveHandler = handler; }

private:
	int _index;
	
	cocos2d::Vec2 _pos;
	cocos2d::Vec2 _destPos;
	cocos2d::Vec2 _spawnPos;
	float _rotation = 0.0f;
	float _alpha = 1.0f;
	
	float _startAlpha = 1.0f;
	float _endAlpha = 1.0f;

	float _timelapse = 0.0f;
	float _currentLifetime = 0.0f;
	float _totalTimelapse = 0.0f;
	float _spawnTotalTime = 0.0f;
	float _moveTotalTime = 0.0f;
	float _lifetime = -1.0f;
	
	ParticleState _currentState = ParticleState::NONE;
	
	void spawnUpdate(float dt);
	void moveUpdate(float dt);
	void particleMove(float totalTime);
	
	void setupCurve();

	OPCurve<float> _curveX;
	OPCurve<float> _curveY;
	OPCurve<float> _curveRotation;
	OPCurve<float> _curveAlpha;
	
	SEL_positioningHandler _updateHandler = nullptr;
	SEL_endObjHandler _reachEndObjHandler = nullptr;
	SEL_endObjHandler _lifetimeEndObjHandler = nullptr;

	SEL_particleCurveHandler _spawnCurveXHandler = nullptr;
	SEL_particleCurveHandler _spawnCurveYHandler = nullptr;
	SEL_particleCurveHandler _moveCurveXHandler = nullptr;
	SEL_particleCurveHandler _moveCurveYHandler = nullptr;
	SEL_particleCurveHandler _alphaCurveHandler = nullptr;
	
};

}

#endif /* SpawnGObject_hpp */
