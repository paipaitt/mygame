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

#include "views/GameView.h"
#include <set>
#include "cocos2d.h"


bool GameView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    initUI();
    
    return true;
}

void GameView::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建主牌区背景（棕色，1080x1500，位置在顶部）
    auto playfieldBg = LayerColor::create(Color4B(139, 69, 19, 255), 1080, 1500);  // 棕色背景
    playfieldBg->setPosition(origin.x, origin.y + 580);
    addChild(playfieldBg, 0);
    
    // 创建主牌区（1080x1500，位置在顶部）
    _playfieldLayer = Layer::create();
    _playfieldLayer->setContentSize(Size(1080, 1500));
    _playfieldLayer->setPosition(origin.x, origin.y + 580);
    addChild(_playfieldLayer, 1);
    
    // 创建底部区域背景（紫色，1080x580，位置在底部）
    auto bottomBg = LayerColor::create(Color4B(128, 0, 128, 255), 1080, 580);  // 紫色背景
    bottomBg->setPosition(origin.x, origin.y);
    addChild(bottomBg, 0);
    
    // 创建底部区域层（1080x580，位置在底部）
    _bottomLayer = Layer::create();
    _bottomLayer->setContentSize(Size(1080, 580));
    _bottomLayer->setPosition(origin.x, origin.y);
    addChild(_bottomLayer, 1);
    
    // 创建备用牌堆区域（左侧，约540x580）
    _reserveArea = Layer::create();
    _reserveArea->setContentSize(Size(540, 580));
    _reserveArea->setPosition(origin.x, origin.y);
    _bottomLayer->addChild(_reserveArea, 1);
    
    // 创建底牌堆区域（右侧，约540x580）
    _baseArea = Layer::create();
    _baseArea->setContentSize(Size(540, 580));
    _baseArea->setPosition(origin.x + 540, origin.y);
    _bottomLayer->addChild(_baseArea, 1);
    
    // 创建回退按钮（白色背景，黑色文字）
    auto label = Label::createWithSystemFont("回退", "Arial", 36);
    if (label)
    {
        label->setColor(Color3B::BLACK);  // 黑色文字
        
        // 创建白色背景
        auto bg = LayerColor::create(Color4B(255, 255, 255, 255), 100, 60);
        bg->setPosition(Vec2(400, 260));  // 相对于baseArea的位置
        _baseArea->addChild(bg, 99);
        
        // 创建菜单项
        _undoButton = MenuItemLabel::create(label, nullptr);
        _undoButton->setPosition(Vec2(450, 290));  // 底牌堆区域右侧位置（相对于baseArea，540宽度的右侧）
        auto menu = Menu::create(_undoButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        _baseArea->addChild(menu, 100);
    }
}

void GameView::addPlayfieldCard(CardView* cardView, const std::function<void(int)>& callback)
{
    if (!cardView || !_playfieldLayer)
    {
        return;
    }
    
    // 确保cardView有效（防止在调用过程中被释放）
    Node* parent = cardView->getParent();
    if (parent)
    {
        cardView->removeFromParent();
    }
    _playfieldLayer->addChild(cardView);
    _cardViews[cardView->getCardId()] = cardView;
    // 重新安装事件监听器（因为removeFromParent会移除事件监听器）
    cardView->reinstallEventListener();
    if (callback)
    {
        cardView->setClickCallback(callback);
    }
}

void GameView::addReserveCard(CardView* cardView, const std::function<void(int)>& callback)
{
    if (cardView && _reserveArea)
    {
        // 如果卡牌已经有父节点，先移除
        if (cardView->getParent())
        {
            cardView->removeFromParent();
        }
        _reserveArea->addChild(cardView);
        _cardViews[cardView->getCardId()] = cardView;
        // 重新安装事件监听器（因为removeFromParent会移除事件监听器）
        cardView->reinstallEventListener();
        if (callback)
        {
            cardView->setClickCallback(callback);
        }
    }
}

void GameView::setBaseCard(CardView* cardView, const std::function<void(int)>& callback)
{
    if (!cardView || !_baseArea)
    {
        return;
    }
    
    // 先隐藏旧的底牌（如果有），但不删除，以便撤销时恢复
    auto& children = _baseArea->getChildren();
    for (auto child : children)
    {
        CardView* oldCard = dynamic_cast<CardView*>(child);
        if (oldCard && oldCard != cardView)  // 排除当前要设置的卡牌
        {
            oldCard->setVisible(false);  // 隐藏旧顶部牌，但保留在场景树中
        }
    }
    
    // 如果卡牌已经在baseArea中，就不需要removeFromParent了
    Node* currentParent = cardView->getParent();
    if (currentParent != _baseArea)
    {
        // 先移除卡牌从任何父节点（可能在其他区域）
        if (currentParent)
        {
            cardView->removeFromParent();
        }
        // 添加新的底牌到中心位置（相对于baseArea）
        Vec2 basePosition(270, 290);  // 底牌堆区域中心位置（相对于baseArea，540宽度的中心）
        cardView->setPosition(basePosition);
        _baseArea->addChild(cardView, 10);  // z-order设为10，确保在最上层
    }
    else
    {
        // 如果已经在baseArea中，只需要更新位置
        Vec2 basePosition(270, 290);
        cardView->setPosition(basePosition);
    }
    
    cardView->setVisible(true);  // 确保新顶部牌可见
    _cardViews[cardView->getCardId()] = cardView;
    // 重新安装事件监听器（因为removeFromParent会移除事件监听器）
    cardView->reinstallEventListener();
    if (callback)
    {
        cardView->setClickCallback(callback);
    }
}

CardView* GameView::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}

void GameView::removePlayfieldCard(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end() && it->second)
    {
        it->second->removeFromParent();
        // 不移除_cardViews中的引用，因为撤销操作需要恢复卡牌视图
        // _cardViews.erase(it);
    }
}


void GameView::setPlayfieldCardClickCallback(const std::function<void(int)>& callback)
{
    if (!_playfieldLayer || !callback)
    {
        return;
    }
    
    // 直接遍历主牌区的所有子节点
    auto& children = _playfieldLayer->getChildren();
    for (auto child : children)
    {
        CardView* cardView = dynamic_cast<CardView*>(child);
        if (cardView)
        {
            cardView->setClickCallback(callback);
        }
    }
}

void GameView::refreshPlayfieldCards(const std::vector<int>& playfieldCardIds, const std::function<void(int)>& callback)
{
    if (!_playfieldLayer || !callback)
    {
        return;
    }
    
    // 创建playfield卡牌ID的集合，用于快速查找
    std::set<int> playfieldCardIdSet(playfieldCardIds.begin(), playfieldCardIds.end());
    
    // 遍历主牌区的所有子节点，但只处理属于playfield的卡牌
    auto& children = _playfieldLayer->getChildren();
    
    for (auto child : children)
    {
        CardView* cardView = dynamic_cast<CardView*>(child);
        if (cardView)
        {
            int cardId = cardView->getCardId();
            // 只处理属于playfield的卡牌
            if (playfieldCardIdSet.find(cardId) != playfieldCardIdSet.end())
            {
                // 确保卡牌在playfieldLayer中
                if (cardView->getParent() != _playfieldLayer)
                {
                    if (cardView->getParent())
                    {
                        cardView->removeFromParent();
                    }
                    _playfieldLayer->addChild(cardView);
                }
                
                // 重新安装事件监听器
                cardView->reinstallEventListener();
                // 重新设置回调函数
                cardView->setClickCallback(callback);
            }
        }
    }
}

void GameView::setReserveCardClickCallback(const std::function<void(int)>& callback)
{
    if (!_reserveArea || !callback)
    {
        return;
    }
    
    // 直接遍历备用牌堆区域的所有子节点
    auto& children = _reserveArea->getChildren();
    for (auto child : children)
    {
        CardView* cardView = dynamic_cast<CardView*>(child);
        if (cardView)
        {
            cardView->setClickCallback(callback);
        }
    }
}

void GameView::setBaseCardClickCallback(const std::function<void(int)>& callback)
{
    if (!_baseArea || !callback)
    {
        return;
    }
    
    // 直接遍历底牌堆区域的所有子节点
    auto& children = _baseArea->getChildren();
    for (auto child : children)
    {
        CardView* cardView = dynamic_cast<CardView*>(child);
        if (cardView)
        {
            cardView->setClickCallback(callback);
        }
    }
}

void GameView::setUndoButtonCallback(const std::function<void()>& callback)
{
    if (_undoButton)
    {
        _undoButton->setCallback([callback](Ref* sender) {
            if (callback)
            {
                callback();
            }
        });
    }
}
