//
//  FacebookIOS.m
//  gt
//
//  Created by NND on 5/11/15.
//
//

#include "FacebookIOS.h"
#include "FBSDKLoginKit.framework/Headers/FBSDKLoginManager.h"
#include "FBSDKLoginKit.framework/Headers/FBSDKLoginManagerLoginResult.h"
#import "cocos2d.h"
#include "FacebookAgent.h"

USING_NS_CC;

static FacebookIOS* s_sharedFacebookIOS = NULL;

FacebookIOS* FacebookIOS::getInstance()
{
    if(NULL == s_sharedFacebookIOS)
    {
        s_sharedFacebookIOS = new FacebookIOS();
    }
    return s_sharedFacebookIOS;
}

void FacebookIOS::login(){
    
    FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
    [login logInWithReadPermissions:@[] handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
        if (error) {
            // Process error
            //NSLog([NSString stringWithUTF8String:"NND Error"]);
        } else if (result.isCancelled) {
            // Handle cancellations
            NSString *fbAccessToken = [[FBSDKAccessToken currentAccessToken] tokenString];
            NSLog(fbAccessToken);
            CCString* str = new CCString([fbAccessToken UTF8String]);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LOGIN_FB, str);

        } else {
            NSString *fbAccessToken = [[FBSDKAccessToken currentAccessToken] tokenString];
            NSLog(fbAccessToken);
            CCString* str = new CCString([fbAccessToken UTF8String]);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LOGIN_FB, str);
            //NSLog([NSString stringWithFormat:@"NND Success = %s",resu]);
            
            // If you ask for multiple permissions at once, you
            // should check if specific permissions missing
            // Success
            
        }
    }];
    
}


