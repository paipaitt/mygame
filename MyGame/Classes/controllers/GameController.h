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

#ifndef _GAME_CONTROLLER_H_
#define _GAME_CONTROLLER_H_

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"

USING_NS_CC;

/**
 * @brief 游戏控制器类
 * 管理整个游戏流程，协调Model和View之间的交互
 * 
 * 职责：
 * - 管理游戏流程（初始化、开始、结束）
 * - 协调各个子控制器
 * - 处理用户操作和业务逻辑
 * 
 * 使用场景：
 * - 作为游戏场景的主要控制器
 */
class GameController
{
public:
    GameController();
    ~GameController();
    
    /**
     * @brief 开始游戏
     * @param levelId 关卡ID
     * @return 是否成功开始游戏
     */
    bool startGame(int levelId);
    
    /**
     * @brief 初始化视图
     * @param parentNode 父节点
     */
    void initView(Node* parentNode);
    
    /**
     * @brief 处理备用牌堆卡牌点击
     * @param cardId 卡牌ID
     */
    void handleReserveCardClick(int cardId);
    
    /**
     * @brief 处理底牌堆卡牌点击
     * @param cardId 卡牌ID
     */
    void handleBaseCardClick(int cardId);
    
    /**
     * @brief 处理主牌区卡牌点击
     * @param cardId 卡牌ID
     */
    void handlePlayfieldCardClick(int cardId);
    
    /**
     * @brief 处理回退按钮点击
     */
    void handleUndoButtonClick();
    
private:
    /**
     * @brief 初始化主牌区和手牌区的卡牌视图
     */
    void initCardViews();
    
    /**
     * @brief 记录撤销操作（备用牌堆到底牌堆替换）
     * @param cardId 操作的卡牌ID
     * @param targetCardId 被替换的卡牌ID
     */
    void recordReserveToBaseUndo(int cardId, int targetCardId);
    
    /**
     * @brief 记录撤销操作（桌面牌匹配）
     * @param cardId 操作的卡牌ID
     */
    void recordPlayfieldMatchUndo(int cardId);
    
    /**
     * @brief 执行备用牌堆到底牌堆替换
     * @param cardId 操作的卡牌ID
     */
    void executeReserveToBaseReplace(int cardId);
    
    /**
     * @brief 执行桌面牌匹配
     * @param cardId 操作的卡牌ID
     */
    void executePlayfieldMatch(int cardId);
    
    /**
     * @brief 更新视图（在执行撤销后）
     * @param record 被撤销的操作记录
     */
    void updateViewsAfterUndo(const UndoRecord& record);
    
    /**
     * @brief 更新备用牌堆卡牌位置
     */
    void updateReserveCardPositions();
    
    /**
     * @brief 处理桌面牌匹配的撤销视图更新
     * @param record 被撤销的操作记录
     */
    void _undoPlayfieldMatch(const UndoRecord& record);
    
    /**
     * @brief 处理备用牌堆到底牌堆替换的撤销视图更新
     * @param record 被撤销的操作记录
     */
    void _undoStackReplace(const UndoRecord& record);
    
    GameModel* _gameModel;       // 游戏模型
    GameView* _gameView;         // 游戏视图
    UndoManager _undoManager;    // 撤销管理器
};

#endif // _GAME_CONTROLLER_H_
