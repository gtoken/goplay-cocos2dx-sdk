//
//  GTMacros.h
//  GoPlaySDK
//
//
//

#ifndef GTokenSDKDemo_GTMacros_h
#define GTokenSDKDemo_GTMacros_h

#define NS_GTOKEN_BEGIN		namespace GoPlaySDK {
#define NS_GTOKEN_END		}
#define USING_GTOKEN		using namespace GoPlaySDK

#define Description(d)
#define Concat(s1, s2) (s1 + s2)


#define SET_GET(varType, varName, funName, default)\
protected: varType varName = default;\
public: varType get##funName(void) const { return varName; }\
public: void set##funName(varType var){ varName = var; }

#define decimal float

#define SAFE_DELETE(p)         if(p) { delete (p); (p) = 0; }
#define SAFE_DELETE_ARRAY(p)   if(p) { delete[] (p); (p) = 0; }

#define btoa(b) (b?"true":"false")

#define DEBUG 1

#define DebugLog CCLOG

#endif
