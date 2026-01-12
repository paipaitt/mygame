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

#include "configs/loaders/LevelConfigLoader.h"
#include "platform/CCFileUtils.h"
#include "json/rapidjson.h"
#include "json/document-wrapper.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 构建配置文件路径
    std::string filePath = StringUtils::format("configs/level_%d.json", levelId);
    
    // 读取文件内容
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(fullPath);
    
    if (jsonContent.empty())
    {
        return nullptr;
    }
    
    return parseJsonToLevelConfig(jsonContent);
}

LevelConfig* LevelConfigLoader::parseJsonToLevelConfig(const std::string& jsonContent)
{
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(jsonContent.c_str());
    
    if (doc.HasParseError() || !doc.IsObject())
    {
        return nullptr;
    }
    
    LevelConfig* config = new LevelConfig();
    
    // 解析主牌堆
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        std::vector<CardPositionConfig> playfieldCards;
        const rapidjson::Value& playfieldArray = doc["Playfield"];
        
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++)
        {
            const rapidjson::Value& cardObj = playfieldArray[i];
            if (cardObj.IsObject())
            {
                CardPositionConfig cardConfig;
                
                if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt())
                {
                    cardConfig.cardFace = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
                }
                
                if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt())
                {
                    cardConfig.cardSuit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());
                }
                
                if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
                {
                    const rapidjson::Value& posObj = cardObj["Position"];
                    if (posObj.HasMember("x") && posObj["x"].IsNumber() &&
                        posObj.HasMember("y") && posObj["y"].IsNumber())
                    {
                        cardConfig.position = Vec2(posObj["x"].GetFloat(), posObj["y"].GetFloat());
                    }
                }
                
                playfieldCards.push_back(cardConfig);
            }
        }
        
        config->setPlayfieldCards(playfieldCards);
    }
    
    // 解析备用牌堆
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        std::vector<CardPositionConfig> stackCards;
        const rapidjson::Value& stackArray = doc["Stack"];
        
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++)
        {
            const rapidjson::Value& cardObj = stackArray[i];
            if (cardObj.IsObject())
            {
                CardPositionConfig cardConfig;
                
                if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt())
                {
                    cardConfig.cardFace = static_cast<CardFaceType>(cardObj["CardFace"].GetInt());
                }
                
                if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt())
                {
                    cardConfig.cardSuit = static_cast<CardSuitType>(cardObj["CardSuit"].GetInt());
                }
                
                if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
                {
                    const rapidjson::Value& posObj = cardObj["Position"];
                    if (posObj.HasMember("x") && posObj["x"].IsNumber() &&
                        posObj.HasMember("y") && posObj["y"].IsNumber())
                    {
                        cardConfig.position = Vec2(posObj["x"].GetFloat(), posObj["y"].GetFloat());
                    }
                }
                
                stackCards.push_back(cardConfig);
            }
        }
        
        config->setStackCards(stackCards);
    }
    
    return config;
}
