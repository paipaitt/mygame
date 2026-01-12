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

#ifndef _UNDO_MODEL_H_
#define _UNDO_MODEL_H_

#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief 撤销操作类型枚举
 */
enum UndoActionType
{
    UAT_NONE = 0,
    UAT_STACK_REPLACE,      // 手牌区翻牌替换
    UAT_PLAYFIELD_MATCH     // 桌面牌匹配
};

/**
 * @brief 撤销操作数据模型
 * 存储单次操作的撤销信息，用于实现回退功能
 * 
 * 职责：
 * - 存储操作类型和相关信息（卡牌ID、位置等）
 * - 支持序列化和反序列化（用于存档恢复）
 * 
 * 使用场景：
 * - 记录每次操作，用于实现撤销功能
 */
struct UndoRecord
{
    UndoActionType actionType;   // 操作类型
    int cardId;                  // 操作的卡牌ID
    Vec2 fromPosition;           // 原位置
    Vec2 toPosition;             // 目标位置
    int targetCardId;            // 目标卡牌ID（用于替换操作）
    
    UndoRecord()
        : actionType(UAT_NONE)
        , cardId(0)
        , fromPosition(Vec2::ZERO)
        , toPosition(Vec2::ZERO)
        , targetCardId(0)
    {
    }
};

/**
 * @brief 撤销数据模型类
 * 管理所有的撤销操作记录
 * 
 * 职责：
 * - 存储撤销操作的历史记录
 * - 提供撤销记录的添加和获取接口
 * 
 * 使用场景：
 * - 在每次操作后添加撤销记录
 * - 在撤销操作时获取并移除记录
 */
class UndoModel
{
public:
    UndoModel();
    ~UndoModel();
    
    /**
     * @brief 添加撤销记录
     * @param record 撤销记录
     */
    void addRecord(const UndoRecord& record);
    
    /**
     * @brief 获取并移除最后一个撤销记录
     * @return 撤销记录，如果没有记录则返回空的UndoRecord
     */
    UndoRecord popRecord();
    
    /**
     * @brief 判断是否有可撤销的记录
     * @return true表示有记录可撤销
     */
    bool hasRecord() const { return !_records.empty(); }
    
    /**
     * @brief 清空所有撤销记录
     */
    void clear();
    
private:
    std::vector<UndoRecord> _records;  // 撤销操作记录列表
};

#endif // _UNDO_MODEL_H_
