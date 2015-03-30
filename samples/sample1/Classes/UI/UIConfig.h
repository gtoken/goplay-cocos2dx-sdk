//
//  UIConfig.h
//  GTokenSDKDemo
//
//  Created by DucHQ on 3/10/15.
//
//

#ifndef __GTokenSDKDemo__UIConfig__
#define __GTokenSDKDemo__UIConfig__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define IS_PLATFORM_MOBILE 1
#else
#define IS_PLATFORM_MOBILE 0
#endif

#endif /* defined(__GTokenSDKDemo__UIConfig__) */
