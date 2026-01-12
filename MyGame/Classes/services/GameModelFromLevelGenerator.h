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

#ifndef _GAME_MODEL_FROM_LEVEL_GENERATOR_H_
#define _GAME_MODEL_FROM_LEVEL_GENERATOR_H_

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

/**
 * @brief 游戏模型生成服务类
 * 将静态配置（LevelConfig）转换为动态运行时数据（GameModel）
 * 
 * 职责：
 * - 根据关卡配置生成游戏运行时数据模型
 * - 处理卡牌随机生成策略（如果需要）
 * 
 * 使用场景：
 * - 在游戏初始化时，根据关卡配置生成GameModel
 * 
 * 注意：这是一个无状态的服务类，不持有数据，只提供转换功能
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 根据关卡配置生成游戏模型
     * @param config 关卡配置对象指针
     * @return 生成的游戏模型指针，如果生成失败返回nullptr。调用者需要负责释放内存
     */
    static GameModel* generateGameModel(LevelConfig* config);
    
private:
    /**
     * @brief 根据配置创建卡牌模型
     * @param cardConfig 卡牌配置
     * @param position 卡牌位置
     * @return 卡牌模型指针，调用者需要负责释放内存
     */
    static CardModel* createCardModel(const CardPositionConfig& cardConfig, const Vec2& position);
};

#endif // _GAME_MODEL_FROM_LEVEL_GENERATOR_H_
