//
//  MiniMap.h
//  ProjectN
//
//  Created by Maximilian Scheurer on 06.09.15.
//
//

#ifndef __ProjectN__MiniMap__
#define __ProjectN__MiniMap__

#include "cocos2d.h"

class MiniMap : public cocos2d::LayerColor {
public:
    MiniMap();
    virtual bool init();
    cocos2d::Sprite *sprite;
    void setImage(cocos2d::Sprite *sprite);
};

#endif /* defined(__ProjectN__MiniMap__) */