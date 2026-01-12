/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "models/CardModel.h"
#include "configs/models/CardTypes.h"
#include <cmath>

int CardModel::_nextCardId = 1;

CardModel::CardModel()
    : _cardId(0)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _position(Vec2::ZERO)
    , _isFlipped(false)
{
}

CardModel::CardModel(CardFaceType face, CardSuitType suit)
    : _cardId(_nextCardId++)
    , _face(face)
    , _suit(suit)
    , _position(Vec2::ZERO)
    , _isFlipped(true)
{
}

CardModel::~CardModel()
{
}

bool CardModel::canMatchWith(const CardModel* other) const
{
    if (!other)
    {
        return false;
    }
    
    int thisValue = getCardValue();
    int otherValue = other->getCardValue();
    
    // 判断数字是否相差1（支持循环，A和K也可以匹配）
    int diff = std::abs(thisValue - otherValue);
    if (diff == 1 || diff == 12)  // 相邻或者A和K（1和13）
    {
        return true;
    }
    
    return false;
}

int CardModel::getCardValue() const
{
    switch (_face)
    {
        case CFT_ACE: return 1;
        case CFT_TWO: return 2;
        case CFT_THREE: return 3;
        case CFT_FOUR: return 4;
        case CFT_FIVE: return 5;
        case CFT_SIX: return 6;
        case CFT_SEVEN: return 7;
        case CFT_EIGHT: return 8;
        case CFT_NINE: return 9;
        case CFT_TEN: return 10;
        case CFT_JACK: return 11;
        case CFT_QUEEN: return 12;
        case CFT_KING: return 13;
        default: return 0;
    }
}
