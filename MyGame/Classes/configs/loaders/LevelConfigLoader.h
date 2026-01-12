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

#ifndef _LEVEL_CONFIG_LOADER_H_
#define _LEVEL_CONFIG_LOADER_H_

#include "configs/models/LevelConfig.h"

/**
 * @brief 关卡配置加载器
 * 负责从JSON配置文件中加载关卡配置数据
 * 
 * 职责：
 * - 从JSON文件读取关卡配置数据
 * - 解析JSON数据并转换为LevelConfig对象
 * 
 * 使用场景：
 * - 在游戏初始化时加载指定关卡的配置数据
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 加载指定关卡的配置
     * @param levelId 关卡ID
     * @return 关卡配置对象的指针，如果加载失败返回nullptr
     */
    static LevelConfig* loadLevelConfig(int levelId);
    
private:
    /**
     * @brief 解析JSON字符串并创建LevelConfig对象
     * @param jsonContent JSON字符串内容
     * @return 关卡配置对象的指针，如果解析失败返回nullptr
     */
    static LevelConfig* parseJsonToLevelConfig(const std::string& jsonContent);
};

#endif // _LEVEL_CONFIG_LOADER_H_
