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

#include "services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameModel* GameModelFromLevelGenerator::generateGameModel(LevelConfig* config)
{
    if (!config)
    {
        return nullptr;
    }
    
    GameModel* gameModel = new GameModel();
    
    // 生成主牌堆的卡牌
    const std::vector<CardPositionConfig>& playfieldCards = config->getPlayfieldCards();
    for (const auto& cardConfig : playfieldCards)
    {
        CardModel* card = createCardModel(cardConfig, cardConfig.position);
        if (card)
        {
            gameModel->addPlayfieldCard(card);
        }
    }
    
    // 生成备用牌堆和底牌堆的卡牌
    // Stack数组中的最后一张作为初始底牌，其余作为备用牌堆
    const std::vector<CardPositionConfig>& stackCards = config->getStackCards();
    if (!stackCards.empty())
    {
        // 最后一张作为初始底牌
        const CardPositionConfig& baseCardConfig = stackCards.back();
        CardModel* baseCard = createCardModel(baseCardConfig, baseCardConfig.position);
        if (baseCard)
        {
            gameModel->setInitialBaseCard(baseCard);
        }
        
        // 其余作为备用牌堆
        for (size_t i = 0; i < stackCards.size() - 1; i++)
        {
            CardModel* card = createCardModel(stackCards[i], stackCards[i].position);
            if (card)
            {
                gameModel->addReserveCard(card);
            }
        }
    }
    
    return gameModel;
}

CardModel* GameModelFromLevelGenerator::createCardModel(const CardPositionConfig& cardConfig, const Vec2& position)
{
    CardModel* card = new CardModel(cardConfig.cardFace, cardConfig.cardSuit);
    card->setPosition(position);
    card->setFlipped(true);  // 默认翻开状态
    return card;
}
