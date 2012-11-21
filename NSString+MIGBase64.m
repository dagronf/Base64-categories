//
//  NSString+MIGBase64.m
//  Base64_Tests
//
//  Created by Darren Ford on 21/11/12.
//  Copyright (c) 2012 Darren Ford. All rights reserved.
//
//  Basic Objective-C wrapper around the migbase64 fast base64 conversion routines
//

/**
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

#import "NSString+MIGBase64.h"

#import "MIGConverter.h"
#import "MIGBase64_Common.h"

#if !__has_feature(objc_arc)
#error MIGBase64+categories must be built with ARC.
#endif

@implementation NSString (MIGBase64)

#pragma mark Convenience property
@dynamic Base64;
-(NSString *)Base64
{
    NSError *error;
    NSString *result = [self encodeAsBase64StringUsingLineEndings:NO error:&error];
    if (!result)
        return @"";
    return result;
}

#pragma mark Encoders

- (NSString *)encodeAsBase64StringUsingLineEndings:(BOOL)useOptionalLineEndings
                                             error:(NSError **)error
{
    MIG_Result res;
    char *result;
    unsigned int result_len;
    *error = nil;
    
    const char *rawString = self.UTF8String;
    if (rawString)
    {
        MIG_Result res = MIG_encodeAsBase64(useOptionalLineEndings==YES?1:0,
                                            (const unsigned char *)(rawString), self.length,
                                            &result, &result_len);
        if (res == MIG_OK)
        {
            return [[NSString alloc] initWithBytesNoCopy:result
                                                  length:result_len
                                                encoding:NSASCIIStringEncoding
                                            freeWhenDone:YES];
        }
    }
    else
    {
        res = MIG_Base64EncodingInvalid;
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

- (NSData *)encodeAsBase64DataUsingLineEndings:(BOOL)useOptionalLineEndings
                                         error:(NSError **)error
{
    MIG_Result res;
    char *result;
    unsigned int result_len;
    *error = nil;
    
    const char *rawString = self.UTF8String;
    if (rawString)
    {
        MIG_Result res = MIG_encodeAsBase64(useOptionalLineEndings==YES?1:0,
                                            (const unsigned char *)(rawString), self.length,
                                            &result, &result_len);
        if (res == MIG_OK)
        {
            return [[NSData alloc] initWithBytesNoCopy:result
                                                length:result_len
                                          freeWhenDone:YES];
        }
    }
    else
    {
        res = MIG_Base64EncodingInvalid;
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

#pragma mark Decoding Base64 from NSString
- (NSData *)decodeBase64AsData:(NSError **)error
{
    MIG_Result res;
    unsigned char *result;
    unsigned int result_len;
    *error = nil;
    
    const char *rawString = self.UTF8String;
    if (rawString)
    {
        // Assumption that string is ASCII encoded.  In theory, if the string is a proper ASCII
        // formatted string, according to the internet self.UTF8String should not provide
        // any overhead.
        MIG_Result res = MIG_decodeAsBase64(self.UTF8String, self.length, &result, &result_len);
        if (res == MIG_OK)
        {
            return [[NSData alloc] initWithBytesNoCopy:result
                                                length:result_len
                                          freeWhenDone:YES];
        }
    }
    else
    {
        // UTF8String returns NULL if the input cant be encoded as UTF8
        res = MIG_Base64EncodingInvalid;
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

- (NSString *)decodeBase64AsString:(NSError **)error
{
    MIG_Result res;
    unsigned char *result;
    unsigned int result_len;
    *error = nil;
    
    const char *rawString = self.UTF8String;
    if (rawString)
    {
        // Assumption that string is ASCII encoded.  In theory, if the string is a proper ASCII
        // formatted string, according to the internet self.UTF8String should not provide
        // any overhead.
        MIG_Result res = MIG_decodeAsBase64(self.UTF8String, self.length, &result, &result_len);
        if (res == MIG_OK)
        {
            return [[NSString alloc] initWithBytesNoCopy:result
                                                  length:result_len
                                                encoding:NSUTF8StringEncoding
                                            freeWhenDone:YES];
        }
    }
    else
    {
        // UTF8String returns NULL if the input cant be encoded as UTF8
        res = MIG_Base64EncodingInvalid;
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}


@end

