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

#include "managers/UndoManager.h"

UndoManager::UndoManager()
    : _gameModel(nullptr)
{
}

UndoManager::~UndoManager()
{
}

void UndoManager::init(GameModel* gameModel)
{
    _gameModel = gameModel;
}

bool UndoManager::undo(UndoRecord& record)
{
    if (!_gameModel || !canUndo())
    {
        return false;
    }
    
    UndoModel& undoModel = _gameModel->getUndoModel();
    record = undoModel.popRecord();
    
    if (record.actionType == UAT_NONE)
    {
        return false;
    }
    
    CardModel* card = _gameModel->getCardById(record.cardId);
    if (!card)
    {
        return false;
    }
    
    // 恢复卡牌位置
    card->setPosition(record.fromPosition);
    
    // 根据操作类型执行不同的撤销逻辑
    if (record.actionType == UAT_STACK_REPLACE)
    {
        // 备用牌堆到底牌堆替换的撤销：恢复原来的顶部卡牌
        if (record.targetCardId > 0)
        {
            _gameModel->removeTopBaseCard();  // 移除当前顶部
            _gameModel->pushTopBaseCard(record.targetCardId);  // 恢复原顶部
        }
        
        // 将卡牌从底牌堆移回备用牌堆
        CardModel* cardToRestore = _gameModel->getCardById(record.cardId);
        if (cardToRestore)
        {
            _gameModel->addReserveCard(cardToRestore);
        }
    }
    else if (record.actionType == UAT_PLAYFIELD_MATCH)
    {
        // 桌面牌匹配的撤销：将卡牌从底牌堆移回主牌区
        _gameModel->removeTopBaseCard();  // 从底牌堆移除
        
        // 恢复原顶部卡牌
        if (record.targetCardId > 0)
        {
            _gameModel->pushTopBaseCard(record.targetCardId);  // 恢复原顶部
        }
        
        // 将卡牌添加回主牌区
        CardModel* cardToRestore = _gameModel->getCardById(record.cardId);
        if (cardToRestore)
        {
            _gameModel->addPlayfieldCard(cardToRestore);
        }
    }
    
    return true;
}

bool UndoManager::canUndo() const
{
    if (!_gameModel)
    {
        return false;
    }
    
    return _gameModel->getUndoModel().hasRecord();
}
