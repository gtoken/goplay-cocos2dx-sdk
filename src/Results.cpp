//
//  Results.cpp
//  GoPlaySDK
//
//
//

#include "Results.h"

NS_GTOKEN_BEGIN

bool Result::tryParse(WWW* www){
    _succeeded = false;
    
    if (www->getError().empty())
    {
        // No error, try to read the JSON data //
        try
        {
            
            JSONObject json = JSONObject(www->getBuffer());
          
            if (json.getField(Constants::RESPONSE_SUCCESS) == Constants::RESPONSE_JSON_TRUE)
            {
                onParse(json);
                _succeeded = true;
            }
            else
            {
                _message = json.getField(Constants::RESPONSE_MESSAGE);
                std::string errorString = json.getField(Constants::RESPONSE_ERROR_CODE);
                Error errCode = Converter::EnumFromDescription(ErrorToString, errorString);
#if DEBUG
                if (!errorString.empty())
                {
                    DebugLog("Unable to Parse this enum. Pls check this : %s", errorString.c_str());
                }
#endif
                _errorCode = errCode;
            }
        }
        catch (const std::string &msg)
        {
#if DEBUG
            DebugLog("Unable to parse Json data: %s", msg.c_str());
#endif
        }
        
    }
    else
    {
        _errorCode = Error::HttpRequestError;
    }
    return _succeeded;
}

void Result::copyFrom(IResult * result)
{
    this->_succeeded = result->getSucceeded();
    this->_message = result->getMessage();
    this->_errorCode = result->getErrorCode();
}

void ProfileResult::onParse( JSONObject& json)
{
    Result::onParse(json);
    _profile = UserProfile::Create(json);
}

void RegisterResult::onParse( JSONObject& json)
{
    ProfileResult::onParse(json);
    _session = json.getField(Constants::RESPONSE_SESSION);
}


void GetProgressResult::onParse( JSONObject& json)
{
Result::onParse(json); // YN TO CHECK THIS
    _data = json.getField(Constants::RESPONSE_DATA);
    string item = json.getField(Constants::RESPONSE_META);
    if (!item.empty()) {
        _meta = item;
    }
}


void GetUnFullFilledExchangesResult::onParse( JSONObject& json)
{
    _exchanges = Exchanges::Create(json);
}

void FullFillExchangeResult::onParse( JSONObject& json)
{
    _exchange = Exchange::Create(json);
}

void OAuthDataObject::onParse( JSONObject& json)
{
}


// Supporting Classes //
UserProfile UserProfile::Create(const JSONObject& json)
{
    UserProfile user;
    
    user.setUserName(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_ACCOUNT));
    
    user.setNickName(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_NICK_NAME));
    
    user.setEMail(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_EMAIL));
    
    Gender gender = Converter::EnumFromDescription(GenderToString, json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_GENDER));
    user.setGender(gender);
    
    Vip vip = Converter::EnumFromDescription(VipToString, json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_VIP));
    user.setVipStatus(vip);
    
    user.setCountryCode(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_COUNTRY_CODE));
    
    user.setGoPlayToken(atof(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_GOPLAY_TOKEN).c_str()));
    
    user.setFreeGoPlayToken(atof(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_FREE_GOPLAY_TOKEN).c_str()));
    
    user.setBalance(atof(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_GTOKEN).c_str()));
    
    user.setFreeBalance(atof(json.getNestedField(Constants::RESPONSE_PROFILE,Constants::RESPONSE_FREE_TOKEN).c_str()));
    
    
    return user;
}

Exchange Exchange::Create(const JSONObject& json)
{
    Exchange ex;
    
    Guid id(json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_TRANSACTION_ID).c_str());
    ex.setTransactionId(id);
    
    ExchangeOptionTypes option = Converter::EnumFromDescription(ExchangeOptionToString, json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_EXCHANGE_OPTION_TYPE));
    ex.setExchangeType(option);
    
    ex.setExchangeOptionId(json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_EXCHANGE_OPTION_IDENTIFIER).c_str());
   
    ex.setGoPlayTokenValue(atof(json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_GOPLAY_TOKEN_VALUE).c_str()));

    ex.setTokenValue(atof(json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_GTOKEN_VALUE).c_str()));
    
    ex.setQuantity(atoi(json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_QUANTITY).c_str()));
    
    string str = json.getNestedField(Constants::RESPONSE_EXCHANGE,Constants::RESPONSE_IS_FREE).c_str();
    bool isFree = str.compare(Constants::RESPONSE_JSON_TRUE)==0;
    ex.setIsFree(isFree);
    
    return ex;
}

Exchanges Exchanges::Create(const JSONObject& json)
{
    const rapidjson::Value &exchangeArray = json.getNestedArray(Constants::RESPONSE_EXCHANGES);
    Exchanges resultExchanges;
    
    for (rapidjson::SizeType i = 0; i < exchangeArray.Size(); i++) {
        Exchange ex;
        ex.setTransactionId(Guid(exchangeArray[i][Constants::RESPONSE_TRANSACTION_ID.c_str()].GetString()));
        ExchangeOptionTypes option = Converter::EnumFromDescription(ExchangeOptionToString,exchangeArray[i][Constants::RESPONSE_EXCHANGE_OPTION_TYPE.c_str()].GetString());
        ex.setExchangeType(option);
        
        ex.setExchangeOptionId(exchangeArray[i][Constants::RESPONSE_EXCHANGE_OPTION_IDENTIFIER.c_str()].GetString());
        
        ex.setGoPlayTokenValue(exchangeArray[i][Constants::RESPONSE_GOPLAY_TOKEN_VALUE.c_str()].GetDouble());
        
        ex.setTokenValue(exchangeArray[i][Constants::RESPONSE_GTOKEN_VALUE.c_str()].GetDouble());
        
        ex.setQuantity(exchangeArray[i][Constants::RESPONSE_QUANTITY.c_str()].GetDouble());
                
        bool isFree = exchangeArray[i][Constants::RESPONSE_IS_FREE.c_str()].GetBool();
        ex.setIsFree(isFree);
        
        resultExchanges.push_back(ex);
    }
    
    return resultExchanges;
}

string GameStats::ToJson() const
{
    
    string arrayJson = "";
    vector<GameStat>::const_iterator start; // declare an iterator to a vector of strings
    for(start = this->begin() ; start < this->end(); start++ ) {
        string publicString = ((*start).getPublic() == true) ? "true" : "false";
        string objectJson = "{\"title\":\"" +  (*start).getTitle() + "\",\"value\":\"" + (*start).getValue()
        + "\",\"public\":\"" + publicString +"\"}";
        arrayJson += objectJson + ",";
    }
        
    arrayJson = arrayJson.substr(0, arrayJson.size()-1);
    arrayJson = "[" + arrayJson + "]";
        
    return arrayJson;
}

NS_GTOKEN_END