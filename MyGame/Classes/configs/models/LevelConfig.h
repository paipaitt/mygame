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

#ifndef _LEVEL_CONFIG_H_
#define _LEVEL_CONFIG_H_

#include "cocos2d.h"
#include "configs/models/CardTypes.h"

USING_NS_CC;

/**
 * @brief 卡牌位置配置数据
 * 描述单个卡牌在关卡中的位置信息
 */
struct CardPositionConfig
{
    CardFaceType cardFace;      // 卡牌正面类型
    CardSuitType cardSuit;      // 卡牌花色类型
    Vec2 position;              // 卡牌位置坐标
    
    CardPositionConfig() : cardFace(CFT_NONE), cardSuit(CST_NONE), position(Vec2::ZERO) {}
};

/**
 * @brief 关卡配置类
 * 存储关卡中的所有静态配置数据，包括主牌堆和备用牌堆的卡牌信息
 * 
 * 职责：
 * - 存储关卡中所有卡牌的静态配置数据
 * - 提供关卡配置数据的访问接口
 * 
 * 使用场景：
 * - 在关卡加载时从JSON文件读取并创建LevelConfig实例
 * - 作为生成游戏运行时数据的依据
 */
class LevelConfig
{
public:
    LevelConfig();
    ~LevelConfig();
    
    /**
     * @brief 获取主牌堆的卡牌配置列表
     * @return 主牌堆卡牌配置列表的引用
     */
    const std::vector<CardPositionConfig>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * @brief 获取备用牌堆的卡牌配置列表
     * @return 备用牌堆卡牌配置列表的引用
     */
    const std::vector<CardPositionConfig>& getStackCards() const { return _stackCards; }
    
    /**
     * @brief 设置主牌堆的卡牌配置列表
     * @param cards 主牌堆卡牌配置列表
     */
    void setPlayfieldCards(const std::vector<CardPositionConfig>& cards) { _playfieldCards = cards; }
    
    /**
     * @brief 设置备用牌堆的卡牌配置列表
     * @param cards 备用牌堆卡牌配置列表
     */
    void setStackCards(const std::vector<CardPositionConfig>& cards) { _stackCards = cards; }
    
private:
    std::vector<CardPositionConfig> _playfieldCards;  // 主牌堆卡牌配置列表
    std::vector<CardPositionConfig> _stackCards;      // 备用牌堆卡牌配置列表
};

#endif // _LEVEL_CONFIG_H_
