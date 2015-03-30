
#include "Utilities.h"

#include <string>

using namespace std;
//
class Random
{
public:
    Random(void);
    Random( long seed);
    static void setSeed(long _seed);
    static int next();
public:
    static long seed;
//    static long multiplier;
//    static long addend;
//    static long mask;
};

//long Random::multiplier = 0x5DEECE6C;
//long Random::addend = 0xB;
//long Random::mask = (1L << 48) - 1;
long Random::seed= 0x2342dda;

Random::Random(void)
{
    setSeed(time(NULL));
}

Random::Random( long seed) {
    setSeed(seed);
}

void Random::setSeed(long _seed) {
    seed = (unsigned)(_seed & 0x7fffffffU);
}

int Random::next() {
    seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return (int)(seed);
}

//#define RANDOM(X) Random::next()%(int)X
//#define RANDOM_SEED(X) Random::setSeed(X)

Random _random;
//
//
/*
 *keep_empty for "a&a" vs "a&&a"
 *const vector<string> words = split("So close no matter how far", " ");
 *
 */
vector<string> meSplit(const string& s, const char& delim, const bool keep_empty = true) {
    vector<string> result;
    std::string token, mystring(s);
    //    int delim_length = delim.length();
    int delim_length = 1;
    while(token != mystring){
        int indext = (int)mystring.find_first_of(delim);
        token = mystring.substr( 0, indext);
        mystring = mystring.substr(indext + delim_length);
        if(keep_empty || token.length() > 0){
            result.push_back(token);
        }
    }
    return result;
}


//Static Utilities

long long Utilities::getCurrentMicroseconds(){
    long long lLastTime = 0;
    struct timeval stCurrentTime;
    
    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec;
    lLastTime *= 1000;
    lLastTime += (stCurrentTime.tv_usec / 1000);
    return lLastTime;
}

long Utilities::getCurrentSeconds(){
    struct timeval stCurrentTime;
    gettimeofday(&stCurrentTime,NULL);
    return (long)stCurrentTime.tv_sec;
}

int Utilities::random(){
    /*
    struct timeval stCurrentTime;
    gettimeofday(&stCurrentTime,NULL);
    return (int)stCurrentTime.tv_usec;
     */
    return _random.next();
}

std::map<std::string, std::string> Utilities::explodeStringToMap(const std::string& s, const char& delim, const char& compare){
    std::map<std::string, std::string> result;
    const vector<string> arr = meSplit(s,delim,false);
    for (int i = 0; i < arr.size(); i++) {
        const vector<string> keyAndValue = meSplit(arr[i],compare,true);
        if (keyAndValue.size() == 2) {
            result.insert(std::pair<std::string, std::string>( keyAndValue[0], keyAndValue[1]));
        }
    }
    //var_dump
    /*
    log("input = %s", s.c_str());
    for (std::map<std::string, std::string>::iterator it = result.begin(); it!=result.end(); ++it){
        log("%s=%s", it->first.c_str(), it->second.c_str());
    }
     */
    return result;
}

std::vector<std::string> Utilities::explodeStringToArr(const std::string& s, const char& delim){
    return meSplit(s, delim, false);
}

std::vector<std::map<std::string, std::string>> Utilities::explodeStringToArrMap(const std::string& s, const char& delimArr, const char& delimMap, const char& compare){
    std::vector<std::map<std::string, std::string>> result;
    const std::vector<std::string> arr = Utilities::explodeStringToArr(s, delimArr);
    for (int i = 0; i < arr.size(); i++) {
        const std::map<std::string, std::string> dic = Utilities::explodeStringToMap(arr[i], delimMap);
        if (dic.size() > 0) {
            result.push_back(dic);
        }
    }
    return result;
}

int Utilities::getHashCodeByString(const std::string& str){
    const char *key = str.c_str();
    const char *end = key + str.length();
	unsigned int hash;
    
	for (hash = 0; key < end; key++)
	{
		hash *= 16777619;
		hash ^= (unsigned int) (unsigned char) toupper(*key);
	}
	return (hash);
}

int Utilities::round(const float& f){
    if (f < 0) {
        int result = (int) f;
        if (result != f) {
            result--;
        }
        return result;
    }
    return (int)f;
}

void Utilities::pauseAllInNode(cocos2d::Node * n){
    Vector<Node*> a = n->getChildren();
    for (int i = 0; i < a.size(); i++) {
        Utilities::pauseAllInNode(a.at(i));
    }
    n->pause();
}

void Utilities::resumeAllInNode(cocos2d::Node * n){
    Vector<Node*> a = n->getChildren();
    for (int i = 0; i < a.size(); i++) {
        Utilities::resumeAllInNode(a.at(i));
    }
    n->resume();
}

/*
cocos2d::Sprite * Utilities::createSpriteNotCache(const std::string& filename){
    Texture2D * texture = nullptr;
    Image* image = nullptr;
    std::string fullpath = FileUtils::getInstance()->fullPathForFilename(filename);
    if (fullpath.size() == 0)
    {
        return nullptr;
    }
    // all images are handled by UIImage except PVR extension that is handled by our own handler
    do
    {
        image = new (std::nothrow) Image();
        CC_BREAK_IF(nullptr == image);
        
        bool bRet = image->initWithImageFile(fullpath);
        CC_BREAK_IF(!bRet);
        
        texture = new (std::nothrow) Texture2D();
        
        if( texture && texture->initWithImage(image) ){
            //Success
        }
        else
        {
            CCLOG("cocos2d: Couldn't create texture for file:%s in TextureCache", filename.c_str());
        }
    } while (0);
    
    CC_SAFE_RELEASE(image);
    if (!texture) {
        return nullptr;
    }
    Sprite * result = Sprite::createWithTexture(texture);
    texture->release();
    return result;
}
 */

//----------------------------------------------------//

Utilities * mUtilities = nullptr;

Utilities * Utilities::shareUtilities(){
    if(!mUtilities){
        mUtilities = new Utilities();
    }
    return mUtilities;
}

Utilities::Utilities(){
    //Test
    /*
    std::string s = "-1202.827";
    log("atoi(%s)= %d", s.c_str(), atoi(s.c_str()));
    s = "12-02.827";
    log("atoi(%s)= %d", s.c_str(), atoi(s.c_str()));
    s = "0.827";
    log("atof(%s)= %f", s.c_str(), atof(s.c_str()));
    s = "1202.827";
    log("atof(%s)= %f", s.c_str(), atof(s.c_str()));
     */
}

Utilities::~Utilities(){
}

int Utilities::atoi(const char *instr){
    bool isRevert = false;
    if (instr[0] == '-') {
        instr++;
        isRevert = true;
    }
	int retval;
    
	retval = 0;
	for (; *instr; instr++) {
        if (*instr < '0'
            || *instr > '9') {
            break;
        }
		retval = 10*retval + (*instr - '0');
	}
    if (isRevert) {
        return -retval;
    }
	return retval;
}

float Utilities::atof(const char *instr){
    bool isRevert = false;
    bool isHasDot = false;
    if (instr[0] == '-') {
        instr++;
        isRevert = true;
    }
	double retval;
    
	retval = 0;
	for (; *instr; instr++) {
        if (*instr == '.') {
            isHasDot = true;
            instr++;
            break;
        }
        if (*instr < '0'
            || *instr > '9') {
            break;
        }
		retval = 10*retval + (*instr - '0');
	}
    if(isHasDot){
        double retval2 = 0;
        double p = 1;
        for (; *instr; instr++) {
            if (*instr < '0'
                || *instr > '9') {
                break;
            }
            retval2 = 10*retval2 + (*instr - '0');
            p = p * 0.1;
        }
        retval += (retval2 * p);
    }
    if (isRevert) {
        return (float)(-retval);
    }
    return (float)(retval);
}

long Utilities::atol(const char *instr){
    bool isRevert = false;
    if (instr[0] == '-') {
        instr++;
        isRevert = true;
    }
	long retval;
    
	retval = 0;
	for (; *instr; instr++) {
        if (*instr < '0'
            || *instr > '9') {
            break;
        }
		retval = 10*retval + (*instr - '0');
	}
    if (isRevert) {
        return -retval;
    }
	return retval;
}

long long Utilities::atoll(const char *instr)
{
    bool isRevert = false;
    if (instr[0] == '-') {
        instr++;
        isRevert = true;
    }
	long long retval;
    
	retval = 0;
	for (; *instr; instr++) {
		retval = 10*retval + (*instr - '0');
	}
    if (isRevert) {
        return -retval;
    }
	return retval;
}

const char * Utilities::itoa(const int& i){
    this->initWithFormat("%d", i);
    return this->getCString();
}

const char * Utilities::ltoa(const long& l){
    this->initWithFormat("%ld", l);
    return this->getCString();
}

const char * Utilities::ftoa(const float& f){
    this->initWithFormat("%f", f);
    std::string ver_client = this->getCString();
    int i1 = (int)ver_client.find_first_of('.');
    if (i1 > 0) {
        int e = (int)ver_client.find_last_not_of('0');
        if (e > 0 && e > i1 && e+1 < ver_client.size()) {
            ver_client.erase(e + 1, ver_client.size());
        }
    }
    return ver_client.c_str();
}

void Utilities::pushLog(std::string content){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Label * label = Label::createWithTTF(content, "fonts/myriadprobold.ttf", 26, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    label->setAnchorPoint( Vec2::ANCHOR_MIDDLE_BOTTOM);
    label->setPosition( visibleSize.width * 0.5f, 20);
    label->enableOutline(Color4B::BLACK, 2);
    label->setAdditionalKerning(-2);
    label->setColor(Color3B(Color3B(255, 77, 30)));
    label->setOpacity(0);
    label->setScale(1.6f);
    
    FiniteTimeAction * action = Sequence::create(
                                                 Spawn::create(FadeIn::create(0.1f),
                                                               DelayTime::create(1.0f),
                                                               ScaleTo::create(0.1f, 1),
                                                               NULL
                                                               ),
                                                 DelayTime::create(1),
                                                 FadeOut::create(0.1f),
                                                 CallFunc::create(CC_CALLBACK_0( Label::removeFromParent, label)),
                                                 NULL
                                                 );
    
    Director::getInstance()->getRunningScene()->addChild(label);
    
    label->runAction(action);
    
}