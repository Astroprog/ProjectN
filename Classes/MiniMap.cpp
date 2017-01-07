//
//  MiniMap.cpp
//  ProjectN
//
//  Created by Maximilian Scheurer on 06.09.15.
//
//

#include "MiniMap.h"
using namespace cocos2d;

MiniMap::MiniMap() {
    init();
}

bool MiniMap::init() {
    if( !LayerColor::initWithColor(Color4B(255,255,255,255)) )
    {
        return false;
    }
    return true;
}

void MiniMap::setImage(cocos2d::Sprite *sprite) {
    this->removeAllChildren();
    this->addChild(sprite);
}