/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
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

#ifndef _CARD_VIEW_H_
#define _CARD_VIEW_H_

#include "cocos2d.h"
#include "models/CardModel.h"

USING_NS_CC;

/**
 * @brief 卡牌视图类
 * 负责卡牌的UI展示和用户交互
 * 
 * 职责：
 * - 显示卡牌的视觉效果（背景、数字、花色）
 * - 接收用户点击事件
 * - 播放卡牌动画（移动、翻转等）
 * 
 * 使用场景：
 * - 在主牌区和手牌区显示卡牌
 * - 响应用户的点击操作
 */
class CardView : public Sprite
{
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌模型指针（const类型）
     * @return 卡牌视图指针
     */
    static CardView* create(const CardModel* cardModel);
    
    /**
     * @brief 初始化卡牌视图
     * @param cardModel 卡牌模型指针（const类型）
     * @return 是否初始化成功
     */
    virtual bool init(const CardModel* cardModel);
    
    /**
     * @brief 获取卡牌ID
     * @return 卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 设置点击回调函数
     * @param callback 回调函数，参数为卡牌ID
     */
    void setClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 播放平移动画
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 动画完成后的回调
     */
    void playMoveAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 重新安装事件监听器（用于节点被removeFromParent后恢复触摸响应）
     */
    void reinstallEventListener();
    
protected:
    CardView();
    virtual ~CardView();
    
private:
    /**
     * @brief 获取卡牌资源路径
     * @return 资源路径字符串
     */
    std::string getCardBackgroundPath() const;
    std::string getCardNumberPath() const;
    std::string getCardSuitPath() const;
    std::string getCardNumberPathSmall() const;
    std::string getCardSuitPathSmall() const;
    
    int _cardId;                                    // 卡牌ID
    const CardModel* _cardModel;                   // 卡牌模型指针（const类型）
    Sprite* _numberSprite;                         // 左上角数字精灵
    Sprite* _suitSprite;                           // 左上角花色精灵
    std::function<void(int)> _clickCallback;       // 点击回调函数
};

#endif // _CARD_VIEW_H_
