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

#ifndef _GAME_MODEL_H_
#define _GAME_MODEL_H_

#include "cocos2d.h"
#include <map>
#include <vector>
#include "models/CardModel.h"
#include "models/UndoModel.h"

USING_NS_CC;

/**
 * @brief 游戏数据模型类
 * 存储游戏的运行时状态数据
 * 
 * 职责：
 * - 存储所有卡牌的数据
 * - 存储游戏状态（主牌堆、手牌堆等）
 * - 支持序列化和反序列化（用于存档恢复）
 * 
 * 使用场景：
 * - 作为游戏运行时的核心数据模型
 * - 在游戏存档和恢复时进行数据持久化
 */
class GameModel
{
public:
    GameModel();
    ~GameModel();
    
    /**
     * @brief 根据卡牌ID获取卡牌模型
     * @param cardId 卡牌ID
     * @return 卡牌模型指针，如果不存在返回nullptr
     */
    CardModel* getCardById(int cardId);
    
    /**
     * @brief 添加卡牌到主牌堆
     * @param card 卡牌模型指针
     */
    void addPlayfieldCard(CardModel* card);
    
    /**
     * @brief 添加卡牌到备用牌堆
     * @param card 卡牌模型指针
     */
    void addReserveCard(CardModel* card);
    
    /**
     * @brief 设置初始底牌
     * @param card 卡牌模型指针
     */
    void setInitialBaseCard(CardModel* card);
    
    /**
     * @brief 获取主牌堆的所有卡牌
     * @return 主牌堆卡牌ID列表
     */
    const std::vector<int>& getPlayfieldCards() const { return _playfieldCardIds; }
    
    /**
     * @brief 获取备用牌堆的所有卡牌
     * @return 备用牌堆卡牌ID列表
     */
    const std::vector<int>& getReserveCards() const { return _reserveCardIds; }
    
    /**
     * @brief 获取底牌堆顶部卡牌ID
     * @return 顶部卡牌ID，如果为空返回0
     */
    int getTopBaseCardId() const;
    
    /**
     * @brief 从底牌堆移除顶部卡牌
     * @return 被移除的卡牌ID
     */
    int removeTopBaseCard();
    
    /**
     * @brief 将卡牌添加到底牌堆顶部
     * @param cardId 卡牌ID
     */
    void pushTopBaseCard(int cardId);
    
    /**
     * @brief 获取撤销模型
     * @return 撤销模型引用
     */
    UndoModel& getUndoModel() { return _undoModel; }
    
    /**
     * @brief 获取所有卡牌的映射表
     * @return 卡牌ID到卡牌模型的映射表
     */
    const std::map<int, CardModel*>& getAllCards() const { return _allCards; }
    
private:
    std::map<int, CardModel*> _allCards;        // 所有卡牌的映射表（卡牌ID -> 卡牌模型）
    std::vector<int> _playfieldCardIds;        // 主牌堆卡牌ID列表
    std::vector<int> _reserveCardIds;          // 备用牌堆卡牌ID列表
    std::vector<int> _baseCardIds;             // 底牌堆卡牌ID列表
    UndoModel _undoModel;                      // 撤销模型
};

#endif // _GAME_MODEL_H_
