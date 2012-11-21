//
//  SenTestCaseUI.m
//  Base64_Tests
//
//  Created by Darren Ford on 30/10/12.
//  Copyright (c) 2012 Darren Ford. All rights reserved.
//

#import "SenTestCaseUI.h"

@implementation SenTestCaseUI

static BOOL hasException = NO;

- (void) failWithException:(NSException *) anException
{
    hasException = YES;
    
    // Call the super
    [super failWithException:anException];
    
    NSProxy *proxy = [NSConnection rootProxyForConnectionWithRegisteredName:@"com.dford.unittest.messaging" host:nil];
    [proxy performSelector:@selector(handleException:) withObject:anException];
}

- (void) sendTestStart:(SEL)testMethod
{
    NSProxy *proxy = [NSConnection rootProxyForConnectionWithRegisteredName:@"com.dford.unittest.messaging" host:nil];
    
    NSString *str1 = [NSString stringWithFormat:@"%@", [self class]];
    NSString *str2 = NSStringFromSelector(testMethod);
    
    [proxy performSelector:@selector(handleTestStart:andCase:)
                withObject:str1
                withObject:str2];
}


- (void) setUpTestWithSelector:(SEL)testMethod
{
    [self sendTestStart:testMethod];

    hasException = NO;
    [super setUpTestWithSelector:testMethod];
}

- (void) tearDownTestWithSelector:(SEL)testMethod
{
    [super tearDownTestWithSelector:testMethod];

    NSProxy *proxy = [NSConnection rootProxyForConnectionWithRegisteredName:@"com.dford.unittest.messaging" host:nil];
    if (hasException == NO)
    {
        NSString *str = [NSString stringWithFormat:@"%@ %@", [self class], NSStringFromSelector(testMethod)];
        [proxy performSelector:@selector(handleSuccess:) withObject:str];
    }
    else
    {
        NSString *str = [NSString stringWithFormat:@"%@ %@", [self class], NSStringFromSelector(testMethod)];
        [proxy performSelector:@selector(handleSuccess:) withObject:str];
    }
    hasException = NO;
}


@end
