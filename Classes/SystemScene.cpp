//
//  SystemScene.cpp
//  ProjectN
//
//  Created by Peter Rodenkirch on 23.08.15.
//
//

#include "SystemScene.h"

using namespace cocos2d;
using namespace cocos2d::ui;

#pragma mark Init

Scene* SystemScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SystemScene::create();
    scene->addChild(layer);
    
    return scene;
}


bool SystemScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    

    _isRotatingClockwise = false;
    _isRotatingCounterClockwise = false;
    _isMoving = false;
    
    
    setupUI();
    setupMiniMap();
    setupPZLayer();
    setupSolarSystem();
    setupSpaceShip();
    setupGauge();
    centerSpaceShip();
    prepareCelestialBodies();
    setupTouchHandler();
    this->scheduleUpdate();
    
    return true;
}

#pragma mark -
#pragma mark Touch Events

void SystemScene::setupTouchHandler()
{
    auto winSize = Director::getInstance()->getWinSize();
    auto touchListener = EventListenerTouchAllAtOnce::create();
    
    touchListener->onTouchesBegan = [=](const std::vector<Touch*>&touches, Event *event)
    {
        if (touches.size() <= 1)
        {

            Vec2 location = touches[0]->getLocation();
            
            if (location.x < winSize.width * 0.2f) {
                _isRotatingCounterClockwise = true;
            } else if (location.x > winSize.width * 0.8f) {
                _isRotatingClockwise = true;
            }
            
            spaceShip->thrustWillChange();
            
        }
        
    };
    
    touchListener->onTouchesMoved = [=](const std::vector<Touch*>&touches, Event *event)
    {
        _isMoving = true;
        if (touches.size() <= 1) {
            if (touches[0]->getStartLocation().x >= winSize.width * 0.2f && touches[0]->getStartLocation().x <= winSize.width * 0.8f)
            {
                Vec2 start = touches[0]->getStartLocation();
                Vec2 dr = touches[0]->getLocation() - start;
                float dy = dr.y;
                dy = dy / winSize.height * spaceShip->maxThrust;
                
                if (spaceShip->thrustBeforeTouch + dy <= spaceShip->maxThrust && spaceShip->thrustBeforeTouch + dy >= 0.0f)
                {
                    spaceShip->thrust = spaceShip->thrustBeforeTouch + dy;
                } else if (spaceShip->thrustBeforeTouch + dy > spaceShip->maxThrust) {
                    spaceShip->thrust = spaceShip->maxThrust;
                } else if (spaceShip->thrustBeforeTouch + dy < 0.0f) {
                    spaceShip->thrust = 0.0f;
                }
                
                updateFlames();
                updateAccelerationLabel();
                updateThrustLabel();
            }
            
            //        log("%f, %f", dr.x, dr.y);
            
        }
    };
    
    touchListener->onTouchesEnded = [=](const std::vector<Touch*>&touches, Event *event)
    {
        _isRotatingCounterClockwise = false;
        _isRotatingClockwise = false;
        _isMoving = false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

#pragma mark -
#pragma mark Setup

void SystemScene::setupUI()
{
    accelerationLabel = Label::createWithSystemFont("Acceleration: 0.0", "Helvetica", 8.0f);
    accelerationLabel->setPosition(Vec2(10.0f, 280.0f));
    accelerationLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    velocityLabel = Label::createWithSystemFont("Velocity: 0.0", "Helvetica", 8.0f);
    velocityLabel->setPosition(Vec2(10.0f, 265.0f));
    velocityLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    fuelLabel = Label::createWithSystemFont("Fuel: 0.0 \%", "Helvetica", 8.0f);
    fuelLabel->setPosition(Vec2(10.0f, 250.0f));
    fuelLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    thrustLabel = Label::createWithSystemFont("Thrust: 0.0 \%", "Helvetica", 8.0f);
    thrustLabel->setPosition(Vec2(10.0f, 235.0f));
    thrustLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    this->addChild(accelerationLabel, 1);
    this->addChild(velocityLabel, 1);
    this->addChild(fuelLabel, 1);
    this->addChild(thrustLabel, 1);
}

void SystemScene::setupPZLayer()
{
    pzLayer = PanZoomLayer::create();
    this->addChild(pzLayer, -1);
    pzLayer->SetMinScale(0.001f);
    pzLayer->SetMaxScale(50.0f);
    pzLayer->setScale(0.02f);
}

void SystemScene::setupSolarSystem()
{
    CelestialBody *merkur = new CelestialBody(Sprite::create("Merkur.png"));
    CelestialBody *venus = new CelestialBody(Sprite::create("Venus.png"));
    CelestialBody *earth = new CelestialBody(Sprite::create("Erde.png"));
    CelestialBody *sun = new CelestialBody(Sprite::create("Sonne.png"));
    
    sun->sprite->setScale(10.0f);
    sun->sprite->setPosition(0, 0);
    sun->title = "sun";
    sun->mass = 100000000.0f;
    sun->size = 50.0f;
    
    earth->sprite->setScale(0.4f);
    earth->apoapsis = 150000.0f;
    earth->title = "earth";
    earth->size = 50.0f;
    earth->mass = 300000.0f;
    earth->position = Vec2(0, 150000.0f);
    earth->velocity = Vec2(25.81988897f, 0);
    
    venus->sprite->setScale(1.2f);
    venus->apoapsis = 108000.0f;
    venus->title = "venus";
    venus->size = 50.0f;
    venus->mass = 200000.0f;
    venus->position = Vec2(0, 108000.0f);
    venus->velocity = Vec2(30.42903097f, 0);
    
    merkur->sprite->cocos2d::Node::setScale(3.0f);
    merkur->apoapsis = 58000.0f;
    merkur->title = "merkur";
    merkur->size = 50.0f;
    merkur->mass = 15000.0f;
    merkur->position = Vec2(0, 58000.0f);
    merkur->velocity = Vec2(41.52273992f, 0.0f);

    
    celestialBodies = Vector<CelestialBody*>();
    celestialBodies.pushBack(sun);
    celestialBodies.pushBack(merkur);
    celestialBodies.pushBack(venus);
    celestialBodies.pushBack(earth);
}

void SystemScene::setupSpaceShip()
{
    spaceShip = new SpaceShip(Sprite::create("Spaceship.png"));
    spaceShip->orbit->orbitalEllipse = DrawNode::create();
    spaceShip->centralBody = celestialBodies.at(1);
    spaceShip->sprite->setScale(0.01f);
    spaceShip->sprite->setRotation(90.0f);
    spaceShip->position = Vec2(0.0f, 2000.0f);
    spaceShip->dOmega = 100.0f;
    spaceShip->maxThrust = 200.0f;
    spaceShip->velocity = Vec2(200.0f, 0.0f);
    pzLayer->addChild(spaceShip->sprite, 3);
    pzLayer->addChild(spaceShip->orbit->orbitalEllipse, 0);
    spaceShip->sprite->setPosition(spaceShip->position);
    centerSpaceShip();
    
    fire = ParticleFire::create();
    fire->setPosition(spaceShip->sprite->getContentSize().width / 2, 0.0f);
    fire->setRotation(180.0f);
    fire->setScale(0.4f);
    fire->setSpeed(500.0f);
    fire->setEmissionRate(0.0f);
    fire->setLife(0.15f);
    fire->setAngleVar(0.0f);
    spaceShip->sprite->addChild(fire);
    
    spaceShip->dot->drawDot(Vec2::ZERO, 3.0f, Color4F(1.0, 0.0, 0.0, 0.0));
    spaceShip->dotLine->drawLine(Vec2::ZERO, Vec2(0, 5.0f), Color4F(1.0, 0.0, 0.0, 0.0));
    pzLayer->addChild(spaceShip->dot, 3);
    pzLayer->addChild(spaceShip->dotLine, 3);
    
    //map->addChild(spaceShip->dot,0);
    //map->addChild(spaceShip->dotLine,0);
}

void SystemScene::setupGauge() {
    arrowNode = new cocos2d::Node();
    arrowNode->setPosition(5, 205);
    arrowNode->addChild(spaceShip->tank->arrowSprite);
    spaceShip->tank->arrowSprite->setScale(0.3f);
    spaceShip->tank->arrowSprite->setPosition(10, 0);
    arrowNode->setRotation(fullFuelAngle);
    spaceShip->tank->sprite->setScale(0.3f);
    spaceShip->tank->sprite->setPosition(15, 205);
    this->addChild(spaceShip->tank->sprite, 1);
    this->addChild(arrowNode, 1);
}

void SystemScene::centerSpaceShip()
{
    Size winSize = Director::getInstance()->getWinSize();
    Vec2 spaceShipPosition = spaceShip->sprite->getPosition();
    pzLayer->setPosition(Vec2(-spaceShipPosition.x * pzLayer->getScale() + winSize.width / 2, -spaceShipPosition.y * pzLayer->getScale() + winSize.height / 2));
    
//    spaceShip->dotLine->setScale(0.02f);
//    spaceShip->dot->setScale(0.02f);
}

void SystemScene::prepareCelestialBodies()
{
    for (CelestialBody *body : celestialBodies)
    {
        pzLayer->addChild(body->sprite, 2);
        
        if (!(strcmp(body->title, "earth")))
        {
            body->orbit->drawCircle(Vec2::ZERO, body->apoapsis, 360.0f, 1000, false, 1.0, 1.0, planetColor("earth", 0.6f), normalLineThickness * pzLayer->getScale());
            body->dot->drawDot(Vec2::ZERO, 10.0f, planetColor("earth", 0.0f));
            pzLayer->addChild(body->dot, 0);
            pzLayer->addChild(body->orbit, 0);
            
        } else if (!(strcmp(body->title, "venus")))
        {
            body->orbit->drawCircle(Vec2::ZERO, body->apoapsis, 360.0f, 1000, false, 1.0, 1.0, planetColor("venus", 0.6f), normalLineThickness * pzLayer->getScale());
            body->dot->drawDot(Vec2::ZERO, 10.0f, planetColor("venus", 0.0f));
            pzLayer->addChild(body->dot, 0);
            pzLayer->addChild(body->orbit, 0);
            
        } else if (!(strcmp(body->title, "merkur")))
        {
            body->orbit->drawCircle(Vec2::ZERO, body->apoapsis, 360.0f, 1000, false, 1.0, 1.0, planetColor("merkur", 0.6f), normalLineThickness * pzLayer->getScale());
            body->dot->drawDot(Vec2::ZERO, 10.0f, planetColor("merkur", 0.0f));
            pzLayer->addChild(body->dot, 0);
            pzLayer->addChild(body->orbit, 0);
        }
    }
}

void SystemScene::setupMiniMap() {
    map = new MiniMap();
    map->setScale(0.15f);
    map->setPosition(190, -100);
    //map->setContentSize(Size(80,80 * 9/16));
    this->addChild(map, 0);
}

#pragma mark -
#pragma mark Update

void SystemScene::update(float delta)
{
    updatePlanetPositions(delta);
    updateSpaceShip(delta);
    centerSpaceShip();
    updateVelocityLabel();
    updateFuelLabel();
    updateAccelerationLabel();
    updateGauge();
}


void SystemScene::updateSpaceShip(float delta)
{
    //Rotation Update
    
    float dOmega = spaceShip->dOmega;
    float rotation = spaceShip->sprite->getRotation();
    
    if (_isRotatingClockwise && !_isMoving) {
        spaceShip->sprite->setRotation(rotation + dOmega * delta);
    } else if (_isRotatingCounterClockwise && !_isMoving) {
        spaceShip->sprite->setRotation(rotation - dOmega * delta);
    }
    
    //Update SpaceShip Parameters
    
    spaceShip->gravAcceleration = calculateAccelerationAtPosition(spaceShip->position, true);
    spaceShip->updateVelocity(delta);
    spaceShip->updatePosition(delta);
    
    //Prepare Orbit
    
    spaceShip->orbit->radiusVec = spaceShip->position - spaceShip->centralBody->position;
    spaceShip->orbit->velocityVec = spaceShip->velocity - spaceShip->centralBody->velocity;
    spaceShip->orbit->centralMass = spaceShip->centralBody->mass;
    spaceShip->orbit->computeOrbitalParameters();
    
    //Draw Orbit
    
    spaceShip->orbit->orbitalEllipse->clear();
    spaceShip->orbit->orbitalEllipse->setRotation(CC_RADIANS_TO_DEGREES(spaceShip->orbit->inclinaton));
    spaceShip->orbit->orbitalEllipse->setPosition(spaceShip->orbit->center);
    spaceShip->orbit->orbitalEllipse->drawCircle(Vec2::ZERO, spaceShip->orbit->semiMajorAxis, 0.0f, 10000, false, spaceShip->orbit->semiMinorAxis / spaceShip->orbit->semiMajorAxis, 1.0, Color4F(1.0, 1.0, 1.0, 1.0), normalLineThickness * pzLayer->getScale());
    
    //Draw SpaceShip Position
    

    if (pzLayer->getScale() < 1.0f) {
        spaceShip->dot->clear();
        spaceShip->dotLine->clear();
        float opacity = 1 - pzLayer->getScale();
        spaceShip->dot->drawDot(spaceShip->position, 2.0f / pzLayer->getScale(), Color4F(255.0f / 255.0f, 50.0f / 255.0f, 150.0f / 255.0f, opacity));
        spaceShip->dotLine->drawLine(spaceShip->position, spaceShip->position + 8.0f / pzLayer->getScale() * spaceShip->getDirection(), Color4F(255.0f / 255.0f, 50.0f / 255.0f, 150.0f / 255.0f, opacity));
    } else if (pzLayer->getScale() >= 1.0f) {
        spaceShip->dot->clear();
        spaceShip->dotLine->clear();
    }
    
    spaceShip->sprite->setPosition(spaceShip->position);
    
}

void SystemScene::updatePlanetPositions(float delta)
{
    for (CelestialBody *body : celestialBodies)
    {
        body->sprite->setPosition(body->position);
        
        if ((strcmp(body->title, "sun")))
        {
            body->orbit->setPosition(Vec2::ZERO);
            
            if (pzLayer->getScale() < 0.5f) {
                float opacity = 1 - 2 * pzLayer->getScale();
                body->dot->clear();
                
                if (!(strcmp(body->title, "earth")))
                {
                    body->dot->drawDot(body->position, 4.0f / pzLayer->getScale(), planetColor("earth", opacity));
                    
                } else if (!(strcmp(body->title, "venus")))
                {
                    body->dot->drawDot(body->position, 4.0f / pzLayer->getScale(), planetColor("venus", opacity));
                    
                } else if (!(strcmp(body->title, "merkur")))
                {
                    body->dot->drawDot(body->position, 4.0f / pzLayer->getScale(), planetColor("merkur", opacity));
                }
                
            } else if (pzLayer->getScale() >= 0.5f) {
                body->dot->clear();
            }
            
            body->acceleration = calculateAccelerationAtPosition(body->position, false);
            body->updateVelocity(delta);
            body->updatePosition(delta);
            
        }
    }
}

void SystemScene::updateFlames()
{
    fire->setEmissionRate(400 * spaceShip->thrust);
    fire->setSpeed(200 * sqrt(spaceShip->thrust));
}

void SystemScene::updateAccelerationLabel()
{
    std::ostringstream ss;
    ss << "Acceleration: " << std::setprecision(2) << std::fixed << spaceShip->acceleration;
    accelerationLabel->setString(ss.str());
}

void SystemScene::updateVelocityLabel()
{
    std::ostringstream ss;
    ss << "Velocity: " << std::setprecision(2) << std::fixed << (spaceShip->velocity - spaceShip->centralBody->velocity).length();
    velocityLabel->setString(ss.str());
}

void SystemScene::updateFuelLabel() {
    std::ostringstream ss;
    ss << "Fuel: " << std::setprecision(2) << std::fixed << (spaceShip->tank->_capacity / spaceShip->tank->maximalCapacity * 100) << "\%";
    fuelLabel->setString(ss.str());

}

void SystemScene::updateThrustLabel() {
    std::ostringstream ss;
    ss << "Thrust: " << std::setprecision(2) << std::fixed << spaceShip->thrust;
    if (spaceShip->thrust == 200.f) {
        ss << " KNALLGAS!";
    }
    thrustLabel->setString(ss.str());
}

void SystemScene::updateGauge() {
    float angle = fullFuelAngle + fabsf(fullFuelAngle)*2 * (1-(spaceShip->tank->fuelMass/spaceShip->tank->maximalCapacity));
    arrowNode->setRotation(angle);
}


#pragma mark -
#pragma mark Calculation


Vec2 SystemScene::calculateAccelerationAtPosition(cocos2d::Vec2 currentPosition, bool ship)
{
    Vec2 acceleration = Vec2::ZERO;
    
    if (ship) {
        float dr = currentPosition.distance(spaceShip->centralBody->position);
        if (dr >= spaceShip->centralBody->size) {
            Vec2 direction = spaceShip->centralBody->position - currentPosition;
            direction.normalize();
            acceleration += direction * spaceShip->centralBody->mass / powf(dr, 2);
        }
    } else {
        float dr = currentPosition.distance(celestialBodies.at(0)->position);
        Vec2 direction = celestialBodies.at(0)->position - currentPosition;
        direction.normalize();
        acceleration += direction * celestialBodies.at(0)->mass / powf(dr, 2);
    }

    return acceleration;
}


inline Vec2 locationInGLFromTouch(Touch& touch)
{
    auto director = Director::getInstance();
    return director->convertToGL(touch.getLocationInView());
}

Color4F SystemScene::planetColor(const char *title, float opacity)
{
    if (!(strcmp(title, "earth")))
    {
        return Color4F(71.0/255.0, 179.0/255.0, 250.0/255.0, opacity);
        
    } else if (!(strcmp(title, "venus")))
    {
        return Color4F(255.0/255.0, 255.0/255.0, 110.0/255.0, opacity);
        
    } else if (!(strcmp(title, "merkur")))
    {
        return Color4F(255.0/255.0, 200.0/255.0, 100.0/255.0, opacity);
        
    } else {
        return Color4F::BLACK;
    }
}


    
