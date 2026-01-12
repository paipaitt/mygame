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

#include "models/GameModel.h"

GameModel::GameModel()
{
}

GameModel::~GameModel()
{
    // 清理所有卡牌
    for (auto& pair : _allCards)
    {
        delete pair.second;
    }
    _allCards.clear();
}

CardModel* GameModel::getCardById(int cardId)
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        return it->second;
    }
    return nullptr;
}

void GameModel::addPlayfieldCard(CardModel* card)
{
    if (card)
    {
        _allCards[card->getCardId()] = card;
        _playfieldCardIds.push_back(card->getCardId());
    }
}

void GameModel::addReserveCard(CardModel* card)
{
    if (card)
    {
        _allCards[card->getCardId()] = card;
        _reserveCardIds.push_back(card->getCardId());
    }
}

void GameModel::setInitialBaseCard(CardModel* card)
{
    if (card)
    {
        _allCards[card->getCardId()] = card;
        _baseCardIds.push_back(card->getCardId());
    }
}

int GameModel::getTopBaseCardId() const
{
    if (_baseCardIds.empty())
    {
        return 0;
    }
    return _baseCardIds.back();
}

int GameModel::removeTopBaseCard()
{
    if (_baseCardIds.empty())
    {
        return 0;
    }
    int cardId = _baseCardIds.back();
    _baseCardIds.pop_back();
    return cardId;
}

void GameModel::pushTopBaseCard(int cardId)
{
    _baseCardIds.push_back(cardId);
}
