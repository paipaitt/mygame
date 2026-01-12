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

#include "controllers/GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "views/CardView.h"
#include <algorithm>
#include "models/UndoModel.h"

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
{
}

GameController::~GameController()
{
    if (_gameModel)
    {
        delete _gameModel;
        _gameModel = nullptr;
    }
}

bool GameController::startGame(int levelId)
{
    // 加载关卡配置
    LevelConfig* config = LevelConfigLoader::loadLevelConfig(levelId);
    if (!config)
    {
        return false;
    }
    
    // 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(config);
    if (!_gameModel)
    {
        return false;
    }
    
    // 初始化撤销管理器
    _undoManager.init(_gameModel);
    
    return true;
}

void GameController::initView(Node* parentNode)
{
    if (!_gameModel)
    {
        return;
    }
    
    // 创建游戏视图
    _gameView = GameView::create();
    if (!_gameView)
    {
        return;
    }
    
    parentNode->addChild(_gameView);
    
    // 设置回退按钮回调
    _gameView->setUndoButtonCallback([this]() {
        this->handleUndoButtonClick();
    });
    
    // 初始化卡牌视图（会在initCardViews中设置回调）
    initCardViews();
}

void GameController::initCardViews()
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    // 定义回调函数
    auto playfieldCallback = [this](int cardId) {
        this->handlePlayfieldCardClick(cardId);
    };
    auto reserveCallback = [this](int cardId) {
        this->handleReserveCardClick(cardId);
    };
    auto baseCallback = [this](int cardId) {
        this->handleBaseCardClick(cardId);
    };
    
    // 创建主牌区的卡牌视图
    const std::vector<int>& playfieldCardIds = _gameModel->getPlayfieldCards();
    for (int cardId : playfieldCardIds)
    {
        CardModel* cardModel = _gameModel->getCardById(cardId);
        if (cardModel)
        {
            CardView* cardView = CardView::create(cardModel);
            if (cardView)
            {
                _gameView->addPlayfieldCard(cardView, playfieldCallback);
            }
        }
    }
    
    // 创建备用牌堆的卡牌视图（水平排列，显示在左侧）
    const std::vector<int>& reserveCardIds = _gameModel->getReserveCards();
    float reserveStartX = 100.0f;  // 备用牌堆起始X位置（相对于reserveArea）
    float reserveCardY = 290.0f;   // 卡牌Y位置
    float reserveCardSpacing = 120.0f;  // 卡牌间距
    
    for (size_t i = 0; i < reserveCardIds.size(); i++)
    {
        int cardId = reserveCardIds[i];
        CardModel* cardModel = _gameModel->getCardById(cardId);

        if (cardModel)
        {
            CardView* cardView = CardView::create(cardModel);
            if (cardView)
            {
                Vec2 reservePosition(reserveStartX + i * reserveCardSpacing, reserveCardY);
                cardView->setPosition(reservePosition);
                _gameView->addReserveCard(cardView, reserveCallback);
            }
        }
    }
    
    // 创建底牌堆的顶部卡牌视图（只显示一张，在右侧中心）
    int topBaseCardId = _gameModel->getTopBaseCardId();
    if (topBaseCardId > 0)
    {
        CardModel* baseCardModel = _gameModel->getCardById(topBaseCardId);
        if (baseCardModel)
        {
            CardView* baseCardView = CardView::create(baseCardModel);
            if (baseCardView)
            {
                _gameView->setBaseCard(baseCardView, baseCallback);
            }
        }
    }
}

void GameController::handleReserveCardClick(int cardId)
{
    // 备用牌堆卡牌点击：从备用牌堆抽取到底牌堆（需求中提到的功能，但当前需求1是点击底牌堆的卡牌）
    // 根据需求1，应该是点击底牌堆的卡牌（备用牌堆中的卡牌）来替换
    // 但需求说的是"点击手牌区♥A"，这里手牌区应该指的是备用牌堆
    // 所以点击备用牌堆的卡牌，应该平移到底牌堆顶部
    executeReserveToBaseReplace(cardId);
}

void GameController::handleBaseCardClick(int cardId)
{
    // 底牌堆卡牌点击：不做任何操作（底牌堆只用于接收匹配的牌）
}

void GameController::handlePlayfieldCardClick(int cardId)
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    // 检查是否可以进行匹配（桌面牌和底牌堆顶部牌匹配）
    CardModel* playfieldCard = _gameModel->getCardById(cardId);
    if (!playfieldCard)
    {
        return;
    }
    
    int topBaseCardId = _gameModel->getTopBaseCardId();
    if (topBaseCardId <= 0)
    {
        return;
    }
    
    CardModel* topBaseCard = _gameModel->getCardById(topBaseCardId);
    if (!topBaseCard)
    {
        return;
    }
    
    bool canMatch = playfieldCard->canMatchWith(topBaseCard);
    if (canMatch)
    {
        executePlayfieldMatch(cardId);
    }
}

void GameController::handleUndoButtonClick()
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    UndoRecord record;
    if (_undoManager.undo(record))
    {
        updateViewsAfterUndo(record);
    }
}

void GameController::executeReserveToBaseReplace(int cardId)
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    int topBaseCardId = _gameModel->getTopBaseCardId();
    
    if (!card)
    {
        return;
    }
    
    // 记录撤销操作
    recordReserveToBaseUndo(cardId, topBaseCardId);
    
    // 获取目标位置（底牌堆中心，相对于baseArea的坐标）
    Vec2 targetPos(270, 290);  // 底牌堆区域中心位置（相对于baseArea，540宽度的中心）
    Vec2 fromPos = card->getPosition();
    
    // 更新模型：将卡牌从备用牌堆移到底牌堆顶部
    // 从备用牌堆移除
    std::vector<int>& reserveCardIds = const_cast<std::vector<int>&>(_gameModel->getReserveCards());
    reserveCardIds.erase(std::remove(reserveCardIds.begin(), reserveCardIds.end(), cardId), reserveCardIds.end());
    
    // 移到底牌堆顶部
    _gameModel->removeTopBaseCard();  // 移除原顶部
    _gameModel->pushTopBaseCard(cardId);  // 添加新顶部
    card->setPosition(targetPos);
    
    // 更新视图：播放平移动画
    CardView* cardView = _gameView->getCardView(cardId);
    
    if (cardView)
    {
        // 获取卡牌在reserveArea中的父节点
        Node* reserveArea = cardView->getParent();
        if (reserveArea)
        {
            // 获取baseArea的世界坐标位置
            // baseArea在(540, 0)相对于bottomLayer，bottomLayer在(origin.x, origin.y)
            // 目标位置(270, 290)相对于baseArea
            // 所以世界坐标 = bottomLayer位置 + baseArea相对位置 + 目标相对位置
            auto origin = Director::getInstance()->getVisibleOrigin();
            Vec2 baseAreaWorldPos(origin.x + 540, origin.y);
            Vec2 targetWorldPos(baseAreaWorldPos.x + 270, baseAreaWorldPos.y + 290);
            
            // 将世界坐标转换为reserveArea的本地坐标
            Vec2 targetReservePos = reserveArea->convertToNodeSpace(targetWorldPos);
            
            // 在reserveArea中播放动画到转换后的目标位置
            cardView->playMoveAnimation(targetReservePos, 0.3f, [this, cardId]() {
                // 动画完成后，将卡牌移到底牌堆
                CardView* cardView = _gameView->getCardView(cardId);
                if (cardView)
                {
                    auto baseCallback = [this](int id) {
                        this->handleBaseCardClick(id);
                    };
                    _gameView->setBaseCard(cardView, baseCallback);
                    
                    // 动画完成并移到底牌堆后，更新备用牌堆中剩余卡牌的位置
                    updateReserveCardPositions();
                }
            });
        }
        else
        {
            // 如果无法获取reserveArea，直接移到底牌堆（不应该发生）
            auto baseCallback = [this](int id) {
                this->handleBaseCardClick(id);
            };
            _gameView->setBaseCard(cardView, baseCallback);
            updateReserveCardPositions();
        }
    }
}

void GameController::executePlayfieldMatch(int cardId)
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    int topBaseCardId = _gameModel->getTopBaseCardId();
    
    if (!card)
    {
        return;
    }
    
    // 记录撤销操作
    recordPlayfieldMatchUndo(cardId);
    
    // 获取目标位置（底牌堆中心，相对于baseArea的坐标）
    Vec2 targetPos(270, 290);  // 底牌堆区域中心位置（相对于baseArea，540宽度的中心）
    
    // 更新模型：将卡牌从主牌区移到底牌堆顶部
    // 从主牌区移除
    std::vector<int>& playfieldCardIds = const_cast<std::vector<int>&>(_gameModel->getPlayfieldCards());
    playfieldCardIds.erase(std::remove(playfieldCardIds.begin(), playfieldCardIds.end(), cardId), playfieldCardIds.end());
    
    // 移到底牌堆顶部
    _gameModel->removeTopBaseCard();  // 移除原顶部
    _gameModel->pushTopBaseCard(cardId);  // 添加新顶部
    card->setPosition(targetPos);
    
    // 更新视图：播放平移动画
    CardView* cardView = _gameView->getCardView(cardId);
    
    if (cardView)
    {
        // 获取卡牌在playfieldLayer中的父节点
        Node* playfieldLayer = cardView->getParent();
        if (playfieldLayer)
        {
            // 获取baseArea的世界坐标位置
            // baseArea在(540, 0)相对于bottomLayer，bottomLayer在(origin.x, origin.y)
            // 目标位置(270, 290)相对于baseArea
            // 所以世界坐标 = bottomLayer位置 + baseArea相对位置 + 目标相对位置
            auto origin = Director::getInstance()->getVisibleOrigin();
            Vec2 baseAreaWorldPos(origin.x + 540, origin.y);
            Vec2 targetWorldPos(baseAreaWorldPos.x + 270, baseAreaWorldPos.y + 290);
            
            // 将世界坐标转换为playfieldLayer的本地坐标
            Vec2 targetPlayfieldPos = playfieldLayer->convertToNodeSpace(targetWorldPos);
            
            // 在playfieldLayer中播放动画到转换后的目标位置
            cardView->playMoveAnimation(targetPlayfieldPos, 0.3f, [this, cardId]() {
                // 动画完成后，将卡牌移到底牌堆
                CardView* cardView = _gameView->getCardView(cardId);
                if (cardView)
                {
                    auto baseCallback = [this](int id) {
                        this->handleBaseCardClick(id);
                    };
                    _gameView->setBaseCard(cardView, baseCallback);
                }
            });
        }
        else
        {
            // 如果无法获取playfieldLayer，直接移到底牌堆（不应该发生）
            auto baseCallback = [this](int id) {
                this->handleBaseCardClick(id);
            };
            _gameView->setBaseCard(cardView, baseCallback);
        }
    }
}

void GameController::recordReserveToBaseUndo(int cardId, int targetCardId)
{
    if (!_gameModel)
    {
        return;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    if (!card)
    {
        return;
    }
    
    UndoRecord record;
    record.actionType = UAT_STACK_REPLACE;  // 重用这个类型，表示备用牌堆到底牌堆的替换
    record.cardId = cardId;
    record.fromPosition = card->getPosition();
    record.toPosition = Vec2(270, 290);  // 底牌堆中心（相对于baseArea）
    record.targetCardId = targetCardId;
    
    _gameModel->getUndoModel().addRecord(record);
}

void GameController::recordPlayfieldMatchUndo(int cardId)
{
    if (!_gameModel)
    {
        return;
    }
    
    CardModel* card = _gameModel->getCardById(cardId);
    if (!card)
    {
        return;
    }
    
    // 记录被替换的顶部牌ID
    int topBaseCardId = _gameModel->getTopBaseCardId();
    
    UndoRecord record;
    record.actionType = UAT_PLAYFIELD_MATCH;
    record.cardId = cardId;
    record.fromPosition = card->getPosition();
    record.toPosition = Vec2(270, 290);  // 底牌堆中心（相对于baseArea）
    record.targetCardId = topBaseCardId;  // 记录被替换的顶部牌ID
    
    _gameModel->getUndoModel().addRecord(record);
}

void GameController::updateViewsAfterUndo(const UndoRecord& record)
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    if (record.actionType == UAT_PLAYFIELD_MATCH)
    {
        _undoPlayfieldMatch(record);
    }
    else if (record.actionType == UAT_STACK_REPLACE)
    {
        _undoStackReplace(record);
    }
}

void GameController::_undoPlayfieldMatch(const UndoRecord& record)
{
    auto playfieldCallback = [this](int cardId) {
        this->handlePlayfieldCardClick(cardId);
    };
    
    CardView* cardView = _gameView->getCardView(record.cardId);
    if (!cardView)
    {
        return;
    }
    
    if (record.targetCardId > 0)
    {
        CardView* targetCardView = _gameView->getCardView(record.targetCardId);
        if (targetCardView)
        {
            auto baseCallbackForTarget = [this](int id) {
                this->handleBaseCardClick(id);
            };
            _gameView->setBaseCard(targetCardView, baseCallbackForTarget);
        }
    }
    
    Node* currentParent = cardView->getParent();
    if (!currentParent)
    {
        _gameView->addPlayfieldCard(cardView, playfieldCallback);
        cardView->setPosition(record.fromPosition);
        cardView->setVisible(true);
        const std::vector<int>& playfieldCardIds = _gameModel->getPlayfieldCards();
        _gameView->refreshPlayfieldCards(playfieldCardIds, playfieldCallback);
        return;
    }
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vec2 fromWorldPos(origin.x + record.fromPosition.x, origin.y + 580 + record.fromPosition.y);
    Vec2 targetBaseAreaPos = currentParent->convertToNodeSpace(fromWorldPos);
    
    const std::vector<int>& playfieldCardIds = _gameModel->getPlayfieldCards();
    cardView->playMoveAnimation(targetBaseAreaPos, 0.3f, [this, record, playfieldCallback, playfieldCardIds]() {
        CardView* cardView = _gameView->getCardView(record.cardId);
        if (cardView)
        {
            _gameView->addPlayfieldCard(cardView, playfieldCallback);
            cardView->setPosition(record.fromPosition);
            cardView->setVisible(true);
            _gameView->refreshPlayfieldCards(playfieldCardIds, playfieldCallback);
        }
    });
}

void GameController::_undoStackReplace(const UndoRecord& record)
{
    auto reserveCallback = [this](int cardId) {
        this->handleReserveCardClick(cardId);
    };
    
    CardView* cardView = _gameView->getCardView(record.cardId);
    if (!cardView)
    {
        return;
    }
    
    Node* currentParent = cardView->getParent();
    if (!currentParent)
    {
        _gameView->addReserveCard(cardView, reserveCallback);
        cardView->setPosition(record.fromPosition);
        cardView->setVisible(true);
        updateReserveCardPositions();
        if (record.targetCardId > 0)
        {
            CardView* targetCardView = _gameView->getCardView(record.targetCardId);
            if (targetCardView)
            {
                auto baseCallbackForTarget = [this](int id) {
                    this->handleBaseCardClick(id);
                };
                _gameView->setBaseCard(targetCardView, baseCallbackForTarget);
            }
        }
        return;
    }
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    Vec2 reserveAreaWorldPos(origin.x + record.fromPosition.x, origin.y + record.fromPosition.y);
    Vec2 targetBaseAreaPos = currentParent->convertToNodeSpace(reserveAreaWorldPos);
    
    cardView->playMoveAnimation(targetBaseAreaPos, 0.3f, [this, record, reserveCallback]() {
        CardView* cardView = _gameView->getCardView(record.cardId);
        if (cardView)
        {
            _gameView->addReserveCard(cardView, reserveCallback);
            cardView->setPosition(record.fromPosition);
            cardView->setVisible(true);
            updateReserveCardPositions();
        }
        
        if (record.targetCardId > 0)
        {
            CardView* targetCardView = _gameView->getCardView(record.targetCardId);
            if (targetCardView)
            {
                auto baseCallbackForTarget = [this](int id) {
                    this->handleBaseCardClick(id);
                };
                _gameView->setBaseCard(targetCardView, baseCallbackForTarget);
            }
        }
    });
}

void GameController::updateReserveCardPositions()
{
    if (!_gameModel || !_gameView) return;

    const std::vector<int>& reserveCardIds = _gameModel->getReserveCards();
    float reserveStartX = 100.0f;
    float reserveCardY = 290.0f;
    float reserveCardSpacing = 120.0f;

    for (size_t i = 0; i < reserveCardIds.size(); i++)
    {
        int cardId = reserveCardIds[i];
        CardModel* cardModel = _gameModel->getCardById(cardId);
        if (cardModel)
        {
            Vec2 reservePosition(reserveStartX + i * reserveCardSpacing, reserveCardY);
            cardModel->setPosition(reservePosition); // Update model position
            CardView* cardView = _gameView->getCardView(cardId);
            if (cardView && cardView->getParent())
            {
                cardView->playMoveAnimation(reservePosition, 0.3f); // Animate to new position
            }
        }
    }
}