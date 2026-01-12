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

#ifndef _UNDO_MANAGER_H_
#define _UNDO_MANAGER_H_

#include "models/GameModel.h"
#include "models/UndoModel.h"

/**
 * @brief 撤销管理器类
 * 处理游戏的撤销功能
 * 
 * 职责：
 * - 管理撤销操作的数据
 * - 提供撤销操作的接口
 * 
 * 使用场景：
 * - 作为Controller的成员变量，处理撤销逻辑
 * 
 * 注意：禁止实现为单例模式，禁止反向依赖controller
 */
class UndoManager
{
public:
    UndoManager();
    ~UndoManager();
    
    /**
     * @brief 初始化撤销管理器
     * @param gameModel 游戏模型指针
     */
    void init(GameModel* gameModel);
    
    /**
     * @brief 执行撤销操作
     * @param record 输出参数，返回被撤销的操作记录
     * @return true表示撤销成功，false表示没有可撤销的记录
     */
    bool undo(UndoRecord& record);
    
    /**
     * @brief 判断是否有可撤销的记录
     * @return true表示有记录可撤销
     */
    bool canUndo() const;
    
private:
    GameModel* _gameModel;  // 游戏模型指针
};

#endif // _UNDO_MANAGER_H_
