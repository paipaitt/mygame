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

#include "views/CardView.h"
#include "configs/models/CardTypes.h"

CardView::CardView()
    : _cardId(0)
    , _cardModel(nullptr)
    , _numberSprite(nullptr)
    , _suitSprite(nullptr)
{
}

CardView::~CardView()
{
}

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* view = new CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (!cardModel)
    {
        return false;
    }
    
    _cardModel = cardModel;
    _cardId = cardModel->getCardId();
    
    // 加载卡牌背景
    if (!Sprite::initWithFile(getCardBackgroundPath()))
    {
        return false;
    }
    
    // 设置卡牌位置
    setPosition(cardModel->getPosition());
    
    // 创建左上角数字精灵（使用small版本）
    std::string numberPathSmall = getCardNumberPathSmall();
    if (!numberPathSmall.empty())
    {
        _numberSprite = Sprite::create(numberPathSmall);
        if (_numberSprite)
        {
            // 左上角数字位置：更靠近左上角
            float cardWidth = getContentSize().width;
            float cardHeight = getContentSize().height;
            _numberSprite->setPosition(cardWidth * 0.12f, cardHeight * 0.88f);
            addChild(_numberSprite);
        }
    }
    
    // 创建左上角花色精灵
    std::string suitPath = getCardSuitPath();
    if (!suitPath.empty())
    {
        _suitSprite = Sprite::create(suitPath);
        if (_suitSprite)
        {
            // 左上角花色位置：在数字下方稍右
            float cardWidth = getContentSize().width;
            float cardHeight = getContentSize().height;
            _suitSprite->setPosition(cardWidth * 0.14f, cardHeight * 0.78f);
            addChild(_suitSprite);
        }
    }
    
    // 创建中心大数字精灵（使用big版本）
    std::string numberPathBig = getCardNumberPath();
    if (!numberPathBig.empty())
    {
        Sprite* centerNumberSprite = Sprite::create(numberPathBig);
        if (centerNumberSprite)
        {
            float cardWidth = getContentSize().width;
            float cardHeight = getContentSize().height;
            centerNumberSprite->setPosition(cardWidth * 0.5f, cardHeight * 0.5f);  // 居中
            addChild(centerNumberSprite);
        }
    }
    
    // 启用触摸
    reinstallEventListener();
    
    return true;
}

void CardView::reinstallEventListener()
{
    // 移除旧的事件监听器（如果存在）
    _eventDispatcher->removeEventListenersForTarget(this);
    
    // 确保节点已经添加到场景图中
    if (!getParent())
    {
        return;
    }
    
    // 创建新的事件监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        if (rect.containsPoint(locationInNode))
        {
            return true;
        }
        return false;
    };
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);
        
        if (rect.containsPoint(locationInNode))
        {
            if (_clickCallback)
            {
                _clickCallback(_cardId);
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::setClickCallback(const std::function<void(int)>& callback)
{
    _clickCallback = callback;
}

void CardView::playMoveAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback)
{
    auto move = MoveTo::create(duration, targetPos);
    if (callback)
    {
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(move, callFunc, nullptr);
        runAction(sequence);
    }
    else
    {
        runAction(move);
    }
}

std::string CardView::getCardBackgroundPath() const
{
    return "res/card_general.png";
}

std::string CardView::getCardNumberPath() const
{
    if (!_cardModel)
    {
        return "";
    }
    
    CardFaceType face = _cardModel->getFace();
    CardSuitType suit = _cardModel->getSuit();
    
    // 判断是红色还是黑色
    bool isRed = (suit == CST_DIAMONDS || suit == CST_HEARTS);
    std::string colorPrefix = isRed ? "big_red_" : "big_black_";
    
    // 获取数字字符串
    std::string faceStr;
    switch (face)
    {
        case CFT_ACE: faceStr = "A"; break;
        case CFT_TWO: faceStr = "2"; break;
        case CFT_THREE: faceStr = "3"; break;
        case CFT_FOUR: faceStr = "4"; break;
        case CFT_FIVE: faceStr = "5"; break;
        case CFT_SIX: faceStr = "6"; break;
        case CFT_SEVEN: faceStr = "7"; break;
        case CFT_EIGHT: faceStr = "8"; break;
        case CFT_NINE: faceStr = "9"; break;
        case CFT_TEN: faceStr = "10"; break;
        case CFT_JACK: faceStr = "J"; break;
        case CFT_QUEEN: faceStr = "Q"; break;
        case CFT_KING: faceStr = "K"; break;
        default: return "";
    }
    
    return "res/number/" + colorPrefix + faceStr + ".png";
}

std::string CardView::getCardSuitPath() const
{
    if (!_cardModel)
    {
        return "";
    }
    
    CardSuitType suit = _cardModel->getSuit();
    switch (suit)
    {
        case CST_CLUBS: return "res/suits/club.png";
        case CST_DIAMONDS: return "res/suits/diamond.png";
        case CST_HEARTS: return "res/suits/heart.png";
        case CST_SPADES: return "res/suits/spade.png";
        default: return "";
    }
}

std::string CardView::getCardNumberPathSmall() const
{
    if (!_cardModel)
    {
        return "";
    }
    
    CardFaceType face = _cardModel->getFace();
    CardSuitType suit = _cardModel->getSuit();
    
    // 判断是红色还是黑色
    bool isRed = (suit == CST_DIAMONDS || suit == CST_HEARTS);
    std::string colorPrefix = isRed ? "small_red_" : "small_black_";
    
    // 获取数字字符串
    std::string faceStr;
    switch (face)
    {
        case CFT_ACE: faceStr = "A"; break;
        case CFT_TWO: faceStr = "2"; break;
        case CFT_THREE: faceStr = "3"; break;
        case CFT_FOUR: faceStr = "4"; break;
        case CFT_FIVE: faceStr = "5"; break;
        case CFT_SIX: faceStr = "6"; break;
        case CFT_SEVEN: faceStr = "7"; break;
        case CFT_EIGHT: faceStr = "8"; break;
        case CFT_NINE: faceStr = "9"; break;
        case CFT_TEN: faceStr = "10"; break;
        case CFT_JACK: faceStr = "J"; break;
        case CFT_QUEEN: faceStr = "Q"; break;
        case CFT_KING: faceStr = "K"; break;
        default: return "";
    }
    
    return "res/number/" + colorPrefix + faceStr + ".png";
}

std::string CardView::getCardSuitPathSmall() const
{
    // 右下角使用相同的花色图片，但会旋转180度
    return getCardSuitPath();
}
