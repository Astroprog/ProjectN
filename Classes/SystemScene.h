//
//  SystemScene.h
//  ProjectN
//
//  Created by Peter Rodenkirch on 23.08.15.
//
//

#ifndef __ProjectN__SystemScene__
#define __ProjectN__SystemScene__

#include <iomanip>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PanZoomLayer.h"
#include "CelestialBody.h"
#include "SpaceShip.h"
#include "Orbit.h"
#include "MiniMap.h"

class SystemScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    void update(float delta) override;
    void updatePlanetPositions(float delta);
    void updateSpaceShip(float delta);
    void updateFlames();
    void updateAccelerationLabel();
    void updateVelocityLabel();
    void updateFuelLabel();
    void updateThrustLabel();
    void updateGauge();
    void centerSpaceShip();
    void setupUI();
    void setupPZLayer();
    void setupSolarSystem();
    void setupSpaceShip();
    void setupGauge();
    void prepareCelestialBodies();
    void setupTouchHandler();
    void setupMiniMap();
    void capture();
    void afterCaptured(bool succeed, const std::string& outputFile);
    cocos2d::Vec2 calculateAccelerationAtPosition(cocos2d::Vec2 currentPosition, bool ship);
    cocos2d::Color4F planetColor(const char *title, float opacity);
    
    CREATE_FUNC(SystemScene);
    
private:
    
    bool _isRotatingCounterClockwise;
    bool _isRotatingClockwise;
    bool _isMoving;
    PanZoomLayer *pzLayer;
    SpaceShip *spaceShip;
    MiniMap *map;
    cocos2d::ParticleFire *fire;
    cocos2d::Label *accelerationLabel;
    cocos2d::Label *velocityLabel;
    cocos2d::Label *fuelLabel;
    cocos2d::Label *thrustLabel;
    Vector<CelestialBody *> celestialBodies;
    
    cocos2d::Node *arrowNode;
    
    const float normalLineThickness = 2.0;
    
    const float fullFuelAngle = -55.f;
};

#endif /* defined(__ProjectN__SystemScene__) */
