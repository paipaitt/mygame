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

#ifndef _GAME_VIEW_H_
#define _GAME_VIEW_H_

#include "cocos2d.h"
#include <map>
#include "views/CardView.h"

USING_NS_CC;

/**
 * @brief 游戏视图类
 * 负责整个游戏界面的UI展示
 * 
 * 职责：
 * - 显示主牌区和手牌区
 * - 管理所有卡牌视图
 * - 提供回退按钮
 * 
 * 使用场景：
 * - 作为游戏主场景的视图层
 */
class GameView : public Layer
{
public:
    CREATE_FUNC(GameView);
    
    virtual bool init() override;
    
    /**
     * @brief 添加卡牌视图到主牌区
     * @param cardView 卡牌视图指针
     * @param callback 点击回调函数（可选）
     */
    void addPlayfieldCard(CardView* cardView, const std::function<void(int)>& callback = nullptr);
    
    /**
     * @brief 添加卡牌视图到备用牌堆
     * @param cardView 卡牌视图指针
     * @param callback 点击回调函数（可选）
     */
    void addReserveCard(CardView* cardView, const std::function<void(int)>& callback = nullptr);
    
    /**
     * @brief 设置底牌堆顶部卡牌
     * @param cardView 卡牌视图指针
     * @param callback 点击回调函数（可选）
     */
    void setBaseCard(CardView* cardView, const std::function<void(int)>& callback = nullptr);
    
    /**
     * @brief 根据卡牌ID获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图指针，如果不存在返回nullptr
     */
    CardView* getCardView(int cardId);
    
    /**
     * @brief 移除主牌区的卡牌视图
     * @param cardId 卡牌ID
     */
    void removePlayfieldCard(int cardId);
    
    /**
     * @brief 设置主牌区卡牌点击回调
     * @param callback 回调函数，参数为卡牌ID
     */
    void setPlayfieldCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 重新初始化主牌区的所有卡牌（重新安装事件监听器和回调）
     * @param playfieldCardIds 主牌区的卡牌ID列表（用于验证卡牌是否属于主牌区）
     * @param callback 回调函数，参数为卡牌ID
     */
    void refreshPlayfieldCards(const std::vector<int>& playfieldCardIds, const std::function<void(int)>& callback);
    
    /**
     * @brief 设置备用牌堆卡牌点击回调
     * @param callback 回调函数，参数为卡牌ID
     */
    void setReserveCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 设置底牌堆卡牌点击回调
     * @param callback 回调函数，参数为卡牌ID
     */
    void setBaseCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 设置回退按钮点击回调
     * @param callback 回调函数
     */
    void setUndoButtonCallback(const std::function<void()>& callback);
    
private:
    void initUI();
    
    Layer* _playfieldLayer;          // 主牌区层
    Layer* _bottomLayer;             // 底部区域层（紫色区域）
    Layer* _reserveArea;             // 备用牌堆区域（左侧）
    Layer* _baseArea;                // 底牌堆区域（右侧）
    std::map<int, CardView*> _cardViews;  // 卡牌视图映射表
    MenuItemLabel* _undoButton;      // 回退按钮
};

#endif // _GAME_VIEW_H_
