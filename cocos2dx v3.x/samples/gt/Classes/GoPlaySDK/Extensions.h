//
//  Extensions.h
//  GoPlaySDK
//
//
//

#ifndef __GoPlaySDK__Extensions__
#define __GoPlaySDK__Extensions__

#include "GoPlaySDkMacros.h"
#include "cocos2d.h"
#include "network/HttpClient.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "cocos-ext.h"

#include <map>
#include <string>

USING_NS_CC_EXT;
USING_NS_CC;

using namespace std;

NS_GTOKEN_BEGIN

class UrlExtension {
public:
    static string ToURL(string s, bool useLiveServer = false);
};


class JSONObject {
private:
    rapidjson::Document _doc;
    bool _isError = false;
public:
    JSONObject(string data);
    
    ~JSONObject();
    
    string getField( const string& key) const;
    string getNestedField(const string& jsonKey,const string& nestedKey) const;
    const rapidjson::Value &  getNestedArray(const string& jsonKey) const;
    
};


class Guid {
private:
    string _s;
public:
    Guid( const char * s = ""){
        _s = string(s);
    }
    
    string ToString() const{
        return _s;
    }
};

class WWWForm {
private:
    string _post = "";
    int _count_item = 0;
public:
    void addFieldLowerCase( std::string key, std::string value){
        std::string value_2 = value;
        for(unsigned short loop=0;loop < value_2.size();loop++)
        {
            value_2[loop]=tolower(value_2[loop]);
        }
        this->addField(key, value_2);
    }
    
    void addField( std::string key, std::string value){
        if (_count_item > 0) {
            _post += "&";
        }
        _post += CCString::createWithFormat("%s=%s", key.c_str(), value.c_str())->getCString();
        _count_item ++ ;
    }
    
    void addFieldIfValid( std::string key, std::string value){
        if (!value.empty()) {
            addField(key, value);
        }
    }
    
    std::string getPost() const {
        return _post;
    }
};

class WWW : public CCNode {
public:
    static WWW * CREATE(string url , const WWWForm& f, CCNode* target, SEL_CallFuncN pOnHttpResponse);
public:
    WWW();
    ~WWW();
public:
    SET_GET(string, _error, Error, "")
    SET_GET(string, _buffer, Buffer, "")
    
    void request(string url , WWWForm f, CCNode* target, SEL_CallFuncN pOnHttpResponse);
    
private:
    void onHttpResponse(CCHttpClient* client, CCHttpResponse* response);
    
    SEL_CallFuncN _callback;
    CCNode* _target;
};

NS_GTOKEN_END

#endif /* defined(__GoPlaySDK__Extensions__) */
