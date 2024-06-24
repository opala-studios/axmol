//
//  SpawnerGObject.cpp
//  BeatJump-mobile
//
//  Created by Allison Lindner on 20/08/21.
//

#include "SpawnerGObject.hpp"

opalib::SpawnerGObject::SpawnerGObject() {
	_objects.clear();
}

opalib::SpawnerGObject::~SpawnerGObject() {
    _objects.clear();
}

void opalib::SpawnerGObject::setup(int numberOfElements, std::vector<cocos2d::Rect> startRects, std::vector<cocos2d::Rect> endRects) {
    _totalNumberOfElements = numberOfElements;
	setStartRects(startRects);
	setEndRects(endRects);

	setSpawnCurveStyle(CurveStyle::EASE_OUT, CurveStyle::EASE_OUT);
	setSpawnerCurveStyle(CurveStyle::LINEAR);
	setupDefaultCurvers();
}

void opalib::SpawnerGObject::setup(int numberOfElements, cocos2d::Rect rect) {
	_totalNumberOfElements = numberOfElements;
	setStartRects({rect});
	setEndRects({rect});
	
	setShouldCenterOnStartRect(true);
	
	setSpawnCurveStyle(CurveStyle::EASE_IN_OUT, CurveStyle::EASE_IN_OUT);
	setSpawnStyle(SpawnStyle::CENTER_TO_BORDER_RANDOM, rect.size.width/2.0f);
	setSpawnerCurveStyle(CurveStyle::LINEAR);
	setupDefaultCurvers();
}

void opalib::SpawnerGObject::setupDefaultCurvers() {
	setOriginIndexStyle(IndexStyle::BEGIN);
	setDestinationIndexStyle(IndexStyle::BEGIN);
	
	setMoveCurveStyle(CurveStyle::EASE_IN, CurveStyle::EASE_IN);
	setAlphaCurveStyle(CurveStyle::EASE_OUT);
}

void opalib::SpawnerGObject::startSpawn(int numberOfElements,
										float spawnDuration,
										float spawnMoveDuration,
										float endMoveDuration)
{
	_spawnCurve = _spawnerCurveHandler(_numberOfSpawnedElements, numberOfElements);
	_timelapse = 0.0f;
	
	if(!_startSpawn)
	{
		_spawnNumberOfElements = numberOfElements;
		_spawnDuration = spawnDuration;
		_spawnMoveDuration = spawnMoveDuration;
		_endMoveDuration = endMoveDuration;
		
		_numberOfSpawnedElements = 0;
		
		_startSpawn = true;
	}
}

void opalib::SpawnerGObject::startSpawn(int numberOfElements,
										float spawnDuration,
										float spawnMoveDuration,
										float endMoveDuration,
										std::vector<cocos2d::Rect> startRects,
										std::vector<cocos2d::Rect> endRects)
{
	if(startRects.size() > 0)
		setStartRects(startRects);
	
	if(endRects.size() > 0)
		setEndRects(endRects);
	
	startSpawn(numberOfElements, spawnDuration, spawnMoveDuration, endMoveDuration);
}

void opalib::SpawnerGObject::startSpawn(int numberOfElements,
										float spawnDuration,
										float spawnMoveDuration,
										float endMoveDuration,
										float lifetime)
{
	_lifetime = lifetime;
	startSpawn(numberOfElements, spawnDuration, spawnMoveDuration, endMoveDuration);
}

void opalib::SpawnerGObject::update(float dt) {
	for(auto &obj: _objects) {
		obj.second.update(dt);
		
		if(_attachedPool) {
			auto attachedObj = _attachedPool->getObject(obj.second.getIndex());
			
			attachedObj->setPosition(obj.second.getPosition().x, obj.second.getPosition().y);
			attachedObj->setRotation(obj.second.getRotation());
			attachedObj->setAlpha(obj.second.getAlpha());
		}
	}
	
	if(_numberOfSpawnedElements < _spawnNumberOfElements && _numberOfSpawnedElements < _totalNumberOfElements) {
		int newSpawnedElements = (int)_spawnCurve.evaluate(_timelapse/_spawnDuration);
		int spawnLeft = newSpawnedElements - _numberOfSpawnedElements;
		
		if(newSpawnedElements > _totalNumberOfElements)
			newSpawnedElements = _totalNumberOfElements;
		
		if(spawnLeft > 0) {
			for(int i = _numberOfSpawnedElements; i < newSpawnedElements; i++) {
				int originIndex = 0;
				if(_originIndexHandler)
					originIndex = _originIndexHandler(i);

				int destinationIndex = 0;
				if(_destinationIndexHandler)
					destinationIndex = _destinationIndexHandler(i);

				cocos2d::Vec2 startPos;
				
				if(_shouldCenterOnStartRect) {
					startPos.x = _startRects[originIndex].origin.x + _startRects[originIndex].size.width/2.0f;
					startPos.y = _startRects[originIndex].origin.y + _startRects[originIndex].size.height/2.0f;
				} else {
					startPos.x = _startRects[originIndex].origin.x + (AXRANDOM_0_1() * _startRects[originIndex].size.width);
					startPos.y = _startRects[originIndex].origin.y + (AXRANDOM_0_1() * _startRects[originIndex].size.height);
				}

				cocos2d::Vec2 endPos;
				if(_shouldCenterOnEndRect) {
					endPos.x = _endRects[destinationIndex].origin.x + _endRects[destinationIndex].size.width/2.0f;
					endPos.y = _endRects[destinationIndex].origin.y + _endRects[destinationIndex].size.height/2.0f;
				} else{
					endPos.x = _endRects[destinationIndex].origin.x + (AXRANDOM_0_1() * _endRects[destinationIndex].size.width);
					endPos.y = _endRects[destinationIndex].origin.y + (AXRANDOM_0_1() * _endRects[destinationIndex].size.height);
				}

				_objects[i] = ParticleGObject(startPos);
				_objects[i].setIndex(i);

				cocos2d::Vec2 spawnPosition = startPos;
				if(_spawnPositioningHandler)
					spawnPosition = _spawnPositioningHandler(i, startPos.x, startPos.y);

				_objects[i].setSpawnPosition(spawnPosition);
				_objects[i].setDestPosition(endPos);
				_objects[i].setSpawnTotalTime(_spawnMoveDuration);
				_objects[i].setMoveTotalTime(_endMoveDuration);
				
				_objects[i].setUpdateHandler(_updateHandler);
				_objects[i].setReachEndHandler(_reachEndObjHandler);

				_objects[i].setSpawnCurveHandler(_spawnCurveXHandler, _spawnCurveYHandler);
				_objects[i].setMoveCurveHandler(_moveCurveXHandler, _moveCurveYHandler);
				_objects[i].setAlphaCurveHandler(_alphaCurveHandler);
				
				_objects[i].setLifetime(_lifetime);
				_objects[i].setStartAlpha(1.0f);
				_objects[i].setEndAlpha(0.0f);

				if(_spawnHandler)
					_spawnHandler(_objects[i].getIndex(),
								  _objects[i].getPosition(),
								  _objects[i].getRotation(),
								  _objects[i].getAlpha());
				
				if(_attachedPool) {
					_attachedPool->getObject(_objects[i].getIndex())->setVisible(true);
				}
				
				_objects[i].spawn();
				
				_numberOfSpawnedElements++;
			}
		}
	}
	
	_timelapse += dt;
}

void opalib::SpawnerGObject::reset() {
    _numberOfSpawnedElements = 0;
    _timelapse = 0.0f;
    _startSpawn = false;
	_currentDestinationIndex = 0;
    _currentOriginIndex = 0;

    for(auto &obj: _objects)
    {
        obj.second.reset();
    }
	
	_objects.clear();
}

void opalib::SpawnerGObject::setSpawnStyle(opalib::SpawnStyle style, float maxDistance) {
	switch (style) {
        case SpawnStyle::RANDOM:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX + (AXRANDOM_MINUS1_1() * maxDistance),
                        startY + (AXRANDOM_MINUS1_1() * maxDistance));
            });
			break;
        case SpawnStyle::TO_LEFT:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX - (AXRANDOM_0_1() * maxDistance), startY);
            });
			break;
        case SpawnStyle::TO_RIGHT:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX + (AXRANDOM_0_1() * maxDistance), startY);
            });
			break;
        case SpawnStyle::TO_TOP:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX, startY - (AXRANDOM_0_1() * maxDistance));
            });
			break;
        case SpawnStyle::TO_BOTTOM:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX, startY + (AXRANDOM_0_1() * maxDistance));
            });
			break;
        case SpawnStyle::TO_BOTTOM_RANDOM_X:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX + ((AXRANDOM_0_1() - 0.5f) * maxDistance), startY + (AXRANDOM_0_1() * maxDistance));
            });
            break;
		case SpawnStyle::CENTER_TO_BORDER:
			setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
				if(index % 4 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							(cocos2d::Vec2(-AXRANDOM_0_1(), -AXRANDOM_0_1()) * maxDistance);
				} else if(index % 3 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							(cocos2d::Vec2(AXRANDOM_0_1(), -AXRANDOM_0_1()) * maxDistance);
				} else if(index % 2 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							(cocos2d::Vec2(AXRANDOM_0_1(), AXRANDOM_0_1()) * maxDistance);
				}
				
				return cocos2d::Vec2(startX, startY) +
						(cocos2d::Vec2(-AXRANDOM_0_1(), AXRANDOM_0_1()) * maxDistance);
			});
			break;
		case SpawnStyle::CENTER_TO_BORDER_RANDOM:
			setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
				if(index % 4 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							cocos2d::Vec2(-AXRANDOM_0_1(), -AXRANDOM_0_1()).getNormalized() *
							(0.1f + AXRANDOM_0_1() * 0.9f) * maxDistance;
				} else if(index % 3 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							cocos2d::Vec2(AXRANDOM_0_1(), -AXRANDOM_0_1()).getNormalized() *
							(0.1f + AXRANDOM_0_1() * 0.9f) * maxDistance;
				} else if(index % 2 == 0.0f) {
					return cocos2d::Vec2(startX, startY) +
							cocos2d::Vec2(AXRANDOM_0_1(), AXRANDOM_0_1()).getNormalized() *
							(0.1f + AXRANDOM_0_1() * 0.9f) * maxDistance;
				}
				
				return cocos2d::Vec2(startX, startY) +
						cocos2d::Vec2(-AXRANDOM_0_1(), AXRANDOM_0_1()).getNormalized() *
						(0.1f + AXRANDOM_0_1() * 0.9f) * maxDistance;
			});
			break;
        case SpawnStyle::STATIC:
            setCustomSpawnStyle([maxDistance](int index, float startX, float startY) {
                return cocos2d::Vec2(startX, startY);
            });
            break;
    }
}

opalib::SEL_indexHandler opalib::SpawnerGObject::getIndexHandlerForStyle(opalib::IndexStyle style,int& currentIndex, int maxIndex) {
    switch (style) {
        case IndexStyle::LOOP:
            currentIndex = 0;
			return ([this, &currentIndex, maxIndex](int index) {
				int result = currentIndex;
				if(currentIndex < maxIndex - 1)
                    currentIndex++;
				else
                    currentIndex = 0;

				return result;
			});
        case IndexStyle::FORWARD:
            currentIndex = 0;
			return ([this, &currentIndex, maxIndex](int index) {
                int result = currentIndex;
                if(currentIndex < maxIndex - 1)
                    currentIndex++;

                return result;
			});
        case IndexStyle::REVERSE:
            currentIndex = maxIndex - 1;
			return ([this, &currentIndex, maxIndex](int index) {
				int result = currentIndex;
				if(currentIndex > 0)
                    currentIndex--;

				return result;
			});
        case IndexStyle::BEGIN:
			return ([](int index) {
				return 0;
			});
        case IndexStyle::END:
			return ([maxIndex](int index) {
				return maxIndex - 1;
			});
		default:
			return ([](int index) {
				return 0;
			});
	}
}

opalib::SEL_particleCurveHandler opalib::SpawnerGObject::getParticleCurveForStyle(opalib::CurveStyle style) {
	return ([this, style](int index, float fromValue, float toValue) {
		auto curveHandler = getCurveForStyle(style);
		return curveHandler(fromValue, toValue);
	});
}

opalib::SEL_curveHandler opalib::SpawnerGObject::getCurveForStyle(opalib::CurveStyle style) {
	switch(style) {
		case CurveStyle::EASE_IN:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::easeIn(fromValue, toValue);
			});
		case CurveStyle::EASE_OUT:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::easeOut(fromValue, toValue);
			});
		case CurveStyle::EASE_IN_OUT:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::easeInOut(fromValue, toValue);
			});
		case CurveStyle::LINEAR:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::linear(fromValue, toValue);
			});
		case CurveStyle::MAX:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::linear(toValue, toValue);
			});
		case CurveStyle::MIN:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::linear(fromValue, fromValue);
			});
		default:
			return ([](float fromValue, float toValue) {
				return OPCurve<float>::linear(fromValue, toValue);
			});
	}
}
