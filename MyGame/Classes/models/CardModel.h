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

#ifndef _CARD_MODEL_H_
#define _CARD_MODEL_H_

#include "cocos2d.h"
#include "configs/models/CardTypes.h"

USING_NS_CC;

/**
 * @brief 卡牌数据模型类
 * 存储卡牌的运行时数据，包括卡牌类型、位置、状态等信息
 * 
 * 职责：
 * - 存储单张卡牌的所有运行时数据
 * - 支持序列化和反序列化（用于存档恢复）
 * 
 * 使用场景：
 * - 作为游戏运行时卡牌数据的载体
 * - 在游戏存档和恢复时进行数据持久化
 */
class CardModel
{
public:
    CardModel();
    CardModel(CardFaceType face, CardSuitType suit);
    ~CardModel();
    
    /**
     * @brief 获取卡牌唯一ID
     * @return 卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 设置卡牌唯一ID
     * @param id 卡牌ID
     */
    void setCardId(int id) { _cardId = id; }
    
    /**
     * @brief 获取卡牌正面类型
     * @return 卡牌正面类型
     */
    CardFaceType getFace() const { return _face; }
    
    /**
     * @brief 设置卡牌正面类型
     * @param face 卡牌正面类型
     */
    void setFace(CardFaceType face) { _face = face; }
    
    /**
     * @brief 获取卡牌花色类型
     * @return 卡牌花色类型
     */
    CardSuitType getSuit() const { return _suit; }
    
    /**
     * @brief 设置卡牌花色类型
     * @param suit 卡牌花色类型
     */
    void setSuit(CardSuitType suit) { _suit = suit; }
    
    /**
     * @brief 获取卡牌位置
     * @return 卡牌位置坐标
     */
    Vec2 getPosition() const { return _position; }
    
    /**
     * @brief 设置卡牌位置
     * @param pos 卡牌位置坐标
     */
    void setPosition(const Vec2& pos) { _position = pos; }
    
    /**
     * @brief 判断卡牌是否翻开
     * @return true表示已翻开，false表示覆盖
     */
    bool isFlipped() const { return _isFlipped; }
    
    /**
     * @brief 设置卡牌翻开状态
     * @param flipped true表示翻开，false表示覆盖
     */
    void setFlipped(bool flipped) { _isFlipped = flipped; }
    
    /**
     * @brief 判断两张卡牌是否可以匹配（数字相差1）
     * @param other 另一张卡牌
     * @return true表示可以匹配
     */
    bool canMatchWith(const CardModel* other) const;
    
    /**
     * @brief 获取卡牌的数字值（用于匹配判断）
     * @return 卡牌数字值，A=1, 2-10=2-10, J=11, Q=12, K=13
     */
    int getCardValue() const;
    
private:
    static int _nextCardId;      // 用于生成唯一ID的静态计数器
    
    int _cardId;                 // 卡牌唯一ID
    CardFaceType _face;          // 卡牌正面类型
    CardSuitType _suit;          // 卡牌花色类型
    Vec2 _position;              // 卡牌位置
    bool _isFlipped;             // 是否翻开
};

#endif // _CARD_MODEL_H_
