//
//  SpawnerGObject.hpp
//  BeatJump-mobile
//
//  Created by Allison Lindner on 20/08/21.
//

#ifndef SpawnerGObject_hpp
#define SpawnerGObject_hpp

#include <cocos2d.h>
#include "ParticleGObject.hpp"
#include "../Utils/ComponentPool.hpp"

namespace opalib
{

typedef std::function<cocos2d::Vec2(int index, float startX, float startY)> SEL_positioningSetupHandler;
typedef std::function<int(int index)>                                       SEL_indexHandler;
typedef std::function<OPCurve<float>(float from, float to)>					SEL_curveHandler;

enum class SpawnStyle
{
    RANDOM,
    TO_LEFT,
    TO_RIGHT,
    TO_TOP,
    TO_BOTTOM,
    TO_BOTTOM_RANDOM_X,
	CENTER_TO_BORDER,
	CENTER_TO_BORDER_RANDOM,
    STATIC
};

enum class IndexStyle
{
    LOOP,
    FORWARD,
    REVERSE,
    BEGIN,
    END
};

enum class CurveStyle
{
	EASE_IN,
	EASE_OUT,
	EASE_IN_OUT,
	LINEAR,
	MAX,
	MIN,
};

class SpawnerGObject
{
public:
	SpawnerGObject();
	~SpawnerGObject();
	
	void setup(int numberOfElements, std::vector<cocos2d::Rect> startRects, std::vector<cocos2d::Rect> endRects);
	void setup(int numberOfElements, cocos2d::Rect rect);
	
	void setStartRects(std::vector<cocos2d::Rect> startRects) { _startRects = startRects; }
	void setEndRects(std::vector<cocos2d::Rect> endRects) { _endRects = endRects; }
	
	void startSpawn(int numberOfElements,
					float spawnDuration,
					float spawnMoveDuration,
					float endMoveDuration);
	void startSpawn(int numberOfElements,
					float spawnDuration,
					float spawnMoveDuration,
					float endMoveDuration,
					std::vector<cocos2d::Rect> startRects,
					std::vector<cocos2d::Rect> endRects);
	void startSpawn(int numberOfElements,
					float spawnDuration,
					float spawnMoveDuration,
					float endMoveDuration,
					float lifetime);
	
	void update(float dt);
	void reset();

	void setOriginIndexStyle(IndexStyle style) {
		setOriginIndexHandler(getIndexHandlerForStyle(style, _currentOriginIndex, (int)_startRects.size()));
	}
	
    void setDestinationIndexStyle(IndexStyle style) {
		setDestinationIndexHandler(getIndexHandlerForStyle(style, _currentDestinationIndex, (int)_endRects.size()));
	}
	
    void setOriginIndexHandler(SEL_indexHandler handler) { _originIndexHandler = handler; }
	void setDestinationIndexHandler(SEL_indexHandler handler) { _destinationIndexHandler = handler; }

	void setSpawnHandler(SEL_positioningHandler handler) { _spawnHandler = handler; }
	void setUpdateHandler(SEL_positioningHandler handler) { _updateHandler = handler; }
	void setReachEndHandler(SEL_endObjHandler handler) { _reachEndObjHandler = handler; }
	void setLifetimeEndHandler(SEL_endObjHandler handler) { _lifetimeEndObjHandler = handler; }

	void setSpawnStyle(SpawnStyle style, float maxDistance);
	void setCustomSpawnStyle(SEL_positioningSetupHandler handler) { _spawnPositioningHandler = std::move(handler); }
	
	void setSpawnerCurveHandler(SEL_curveHandler handler) { _spawnerCurveHandler = std::move(handler); }
	void setSpawnerCurveStyle(CurveStyle style) { setSpawnerCurveHandler(getCurveForStyle(style)); }

	void setSpawnCurveHandler(SEL_particleCurveHandler handlerX, SEL_particleCurveHandler handlerY) {
		_spawnCurveXHandler = std::move(handlerX);
		_spawnCurveYHandler = std::move(handlerY);
	}
	void setSpawnCurveStyle(CurveStyle xStyle, CurveStyle yStyle) {
		setSpawnCurveHandler(getParticleCurveForStyle(xStyle), getParticleCurveForStyle(yStyle));
	}
	
	void setMoveCurveHandler(SEL_particleCurveHandler handlerX, SEL_particleCurveHandler handlerY) {
		_moveCurveXHandler = std::move(handlerX);
		_moveCurveYHandler = std::move(handlerY);
	}
	void setMoveCurveStyle(CurveStyle xStyle, CurveStyle yStyle) {
		setMoveCurveHandler(getParticleCurveForStyle(xStyle), getParticleCurveForStyle(yStyle));
	}
	
	void setAlphaCurveHandler(SEL_particleCurveHandler handler) { _alphaCurveHandler = handler; }
	void setAlphaCurveStyle(CurveStyle style) { setAlphaCurveHandler(getParticleCurveForStyle(style)); }
    
    void setShouldCenterOnEndRect(bool should) {
        _shouldCenterOnEndRect = should;
    }
	
	void setShouldCenterOnStartRect(bool should) {
		_shouldCenterOnStartRect = should;
	}
	
	void attachComponentPool(ComponentPool<fairygui::GComponent>* pool) { _attachedPool = pool; }
	void detachComponentPool() { _attachedPool = nullptr; }

private:
	void setupDefaultCurvers();
	
	int _totalNumberOfElements = 0;
	int _spawnNumberOfElements = 0;

	float _spawnDuration = 0.0f;
	float _spawnMoveDuration = 0.0f;
	float _endMoveDuration = 0.0f;
	float _lifetime = -1.0f;
	float _timelapse = 0.0f;

	int _numberOfSpawnedElements = 0;

	bool _startSpawn = false;
    bool _shouldCenterOnEndRect = false;
	bool _shouldCenterOnStartRect = false;

	std::map<int, ParticleGObject> _objects;

    std::vector<cocos2d::Rect> _startRects;
    std::vector<cocos2d::Rect> _endRects;

    SEL_indexHandler _originIndexHandler = nullptr;
    SEL_indexHandler _destinationIndexHandler = nullptr;
    SEL_positioningSetupHandler _spawnPositioningHandler = nullptr;
	SEL_positioningHandler _spawnHandler = nullptr;
	SEL_positioningHandler _updateHandler = nullptr;
	SEL_endObjHandler _reachEndObjHandler = nullptr;
	SEL_endObjHandler _lifetimeEndObjHandler = nullptr;

	int _currentDestinationIndex = 0;
    int _currentOriginIndex = 0;
	
	SEL_indexHandler getIndexHandlerForStyle(IndexStyle style,int& currentIndex, int maxIndex);
	
	OPCurve<float> _spawnCurve;

	SEL_curveHandler _spawnerCurveHandler = nullptr;
	SEL_particleCurveHandler _spawnCurveXHandler = nullptr;
	SEL_particleCurveHandler _spawnCurveYHandler = nullptr;
	SEL_particleCurveHandler _moveCurveXHandler = nullptr;
	SEL_particleCurveHandler _moveCurveYHandler = nullptr;
	SEL_particleCurveHandler _alphaCurveHandler = nullptr;

	SEL_particleCurveHandler getParticleCurveForStyle(CurveStyle style);
	SEL_curveHandler getCurveForStyle(CurveStyle style);
	
	ComponentPool<fairygui::GComponent>* _attachedPool = nullptr;
};

}

#endif /* SpawnerGObject_hpp */
