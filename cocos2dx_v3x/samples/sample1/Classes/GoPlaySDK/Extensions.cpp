//
//  Extensions.cpp
//  GoPlaySDK
//
//
//

#include "Extensions.h"
#include "Constants.h"

using namespace rapidjson;

NS_GTOKEN_BEGIN

string UrlExtension::ToURL(string s, bool useLiveServer){
    return  Concat((useLiveServer ? URLs::BASE_LIVE_SERVER : URLs::BASE_DEV_SERVER) , s);
}

JSONObject::JSONObject(string data){
    if (_doc.Parse<0>(data.c_str()).HasParseError()){
        _isError = true;
    }
}

JSONObject::~JSONObject(){
}

string JSONObject::getField( const string& key) const
{
    if (_isError) {
        return "";
    }
    
    if(!_doc.HasMember(key.c_str())){
        return "";
    }
    
    if (_doc[key.c_str()].IsNull()) {
        return "";
    }
    
    if (_doc[key.c_str()].IsString()) {
        return _doc[key.c_str()].GetString();
    }
    
    if (_doc[key.c_str()].IsBool()) {
        return (_doc[key.c_str()].GetBool() ? "true" : "false");
    }
    
    return "";
}

string JSONObject::getNestedField(const string& jsonKey,const string& nestedKey) const{
    if (_isError) {
        return "";
    }
    
    if(!_doc.HasMember(jsonKey.c_str())){
        return "";
    }
    
    if(!_doc[jsonKey.c_str()].HasMember(nestedKey.c_str())){
        return "";
    }
    
    if(_doc[jsonKey.c_str()][nestedKey.c_str()].IsString()){
        return _doc[jsonKey.c_str()][nestedKey.c_str()].GetString();
    }
    
    if(_doc[jsonKey.c_str()][nestedKey.c_str()].IsBool()){
        return (_doc[jsonKey.c_str()][nestedKey.c_str()].GetBool() == true) ? "true" : "false";
    }
    
    if(_doc[jsonKey.c_str()][nestedKey.c_str()].IsDouble()){
        string reponse = StringUtils::format("%lf",_doc[jsonKey.c_str()][nestedKey.c_str()].GetDouble()).c_str();
        
        return reponse;
    }
    
    return "";
}

const rapidjson::Value & JSONObject::getNestedArray(const string& jsonKey) const{
    
    if(_doc[jsonKey.c_str()].IsArray()){
        const rapidjson::Value &array = _doc[jsonKey.c_str()];
        return array;
    }
    
    return nullptr;
}

WWW * WWW::CREATE(string url , const WWWForm& f, std::function<void(WWW*)> pOnHttpResponse){
    WWW * w = new WWW();
    w->autorelease();
    w->request(url, f, pOnHttpResponse);
    return w;
}

WWW::WWW(){
    
}

WWW::~WWW(){
    
}

void WWW::request(string url , WWWForm f, std::function<void(WWW*)> pOnHttpResponse){
    this->retain();
    
    HttpRequest* request = new HttpRequest();
    request->setUrl(url.c_str());
    request->setResponseCallback([&, pOnHttpResponse](HttpClient* client, HttpResponse* response){
        if (!response) {
            _error = "Response is null";
        }else if (!response->getHttpRequest()){
            _error = "Request is null";
        }else if (!response->isSucceed()){
            _error += response->getErrorBuffer();
            if (_error.empty()) {
                _error += "Connection failed";
            }
            _error.insert(0, "\n");
            
            _error += __String::createWithFormat("\nResponseCode: %ld", response->getResponseCode())->getCString();
            
            std::vector<char> *header_raw = response->getResponseHeader();
            string header = std::string(header_raw->begin(), header_raw->end());

            _error += __String::createWithFormat("\nResponseHeader: %s", header.c_str())->getCString();
            
        }else{
            std::vector<char> *buffer_raw = response->getResponseData();
            _buffer = std::string(buffer_raw->begin(), buffer_raw->end());
        }
        pOnHttpResponse(this);
        this->release();
    });
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setRequestData(f.getPost().c_str(), f.getPost().length());
    
    HttpClient::getInstance()->send(request);
    request->release();
}

NS_GTOKEN_END