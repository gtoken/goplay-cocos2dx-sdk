#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "cocos2d.h"
USING_NS_CC;

/** equals to Vec2(0.5, 0.5) */
static const CCPoint ANCHOR_MIDDLE = CCPoint(0.5f, 0.5f);
/** equals to Vec2(0, 0) */
static const CCPoint ANCHOR_BOTTOM_LEFT = CCPoint(0, 0);
/** equals to Vec2(0, 1) */
static const CCPoint ANCHOR_TOP_LEFT = CCPoint(0, 1);
/** equals to Vec2(1, 0) */
static const CCPoint ANCHOR_BOTTOM_RIGHT = CCPoint(1, 0);
/** equals to Vec2(1, 1) */
static const CCPoint ANCHOR_TOP_RIGHT = CCPoint(1, 1);
/** equals to Vec2(1, 0.5) */
static const CCPoint ANCHOR_MIDDLE_RIGHT = CCPoint(1, 0.5f);
/** equals to Vec2(0, 0.5) */
static const CCPoint ANCHOR_MIDDLE_LEFT = CCPoint(0, 0.5f);
/** equals to Vec2(0.5, 1) */
static const CCPoint ANCHOR_MIDDLE_TOP = CCPoint(0.5f, 1.0f);;
/** equals to Vec2(0.5, 0) */
static const CCPoint ANCHOR_MIDDLE_BOTTOM = CCPoint(0.5f, 0);

class Utilities : public cocos2d::CCString
{
public:
    static long long getCurrentMicroseconds();
    
    static long getCurrentSeconds();
    
    static int random();
    
    static std::map<std::string, std::string> explodeStringToMap(const std::string& s, const char& delim = '&', const char& compare = '=');

    static std::vector<std::string> explodeStringToArr(const std::string& s, const char& delim = '*');
    
    static std::vector<std::map<std::string, std::string> > explodeStringToArrMap(const std::string& s, const char& delimArr = '*', const char& delimMap = '&', const char& compare = '=');
    
    static int getHashCodeByString(const std::string& str);
    
    static int round(const float& f);
    
    static void pauseAllInNode(cocos2d::CCNode * n);
    static void resumeAllInNode(cocos2d::CCNode * n);
    
    /*
    static cocos2d::Sprite * createSpriteNotCache(const std::string& filename);
    */
    
    static Utilities * shareUtilities();
public:
    Utilities();
    virtual ~Utilities();
    
    int atoi(const char *instr);
    float atof(const char *instr);
    long atol(const char *instr);
    long long atoll(const char *instr);
    
    const char * itoa(const int& i);
    const char * ltoa(const long& l);
    const char * ftoa(const float& f);
    void pushLog(std::string content);
};
#endif