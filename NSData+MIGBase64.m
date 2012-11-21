//
//  NSData+MIGBase64.m
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

#import "NSData+MIGBase64.h"

#import "MIGConverter.h"
#import "MIGBase64_Common.h"
#import "NSString+MIGBase64.h"

#if !__has_feature(objc_arc)
#error MIGBase64+categories must be built with ARC.
#endif

#pragma mark -
#pragma mark NSData Base 64 additions (fastest routines)

@implementation NSData (MIGBase64_FAST)

#pragma mark Decoders

+ (NSData *)dataFromBase64EncodedChars:(const char *)data
                                length:(int)length
                                 error:(NSError **)error
{
    unsigned char *result;
    unsigned int result_len;
    MIG_Result res = MIG_decodeAsBase64(data, length, &result, &result_len);
    if (res == MIG_OK)
    {
        return [[NSData alloc] initWithBytesNoCopy:result
                                            length:result_len
                                      freeWhenDone:YES];
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

+ (NSData *)dataFromBase64EncodedData:(NSData *)data
                                error:(NSError **)error
{
    return [NSData dataFromBase64EncodedChars:data.bytes length:data.length error:error];
}

- (NSData *)decodeFromBase64Data:(NSError **)error
{
    return [NSData dataFromBase64EncodedChars:self.bytes length:self.length error:error];
}

#pragma mark Encoders

+ (NSString *)stringByEncodingDataAsBase64:(NSData *)data
                            withFormatting:(BOOL)useOptionalLineEndings
                                     error:(NSError **)error
{
    return [data encodeAsBase64StringUsingLineEndings:useOptionalLineEndings error:error];
}

-(NSData *)encodeAsBase64DataUsingLineEndings:(BOOL)useOptionalLineEndings
                                        error:(NSError **)error
{
    char *result;
    unsigned int result_len;
    
    MIG_Result res = MIG_encodeAsBase64(useOptionalLineEndings==YES?1:0,
                                        (const unsigned char *)(self.bytes), self.length,
                                        &result, &result_len);
    if (res == MIG_OK)
    {
        return [[NSData alloc] initWithBytesNoCopy:result
                                            length:result_len
                                      freeWhenDone:YES];
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

-(NSString *)encodeAsBase64StringUsingLineEndings:(BOOL)useOptionalLineEndings
                                            error:(NSError **)error
{
    char *result;
    unsigned int result_len;
    MIG_Result res = MIG_encodeAsBase64(useOptionalLineEndings==YES?1:0,
                                        (const unsigned char *)(self.bytes), self.length,
                                        &result, &result_len);
    if (res == MIG_OK)
    {
        // Assumption here is that the result is an ASCII formatted string containing the Base64 encoding.
        return [[NSString alloc] initWithBytesNoCopy:result
                                              length:result_len
                                            encoding:NSASCIIStringEncoding
                                        freeWhenDone:YES];
    }
    
    // Got an error -- generate a descriptive error
    *error = generateErrorStructure(res);
    return nil;
}

@end



////////////////////////////////////////////////////////////////////////
// Slower routines
////////////////////////////////////////////////////////////////////////

#pragma mark -
#pragma mark NSData Base 64 additions (slower, NSString-based)
@implementation NSData (MIGBase64)

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

#pragma mark -
#pragma mark Decoders
+ (NSData *)dataFromBase64EncodedString:(NSString *)b64
                                  error:(NSError **)error
{
    return [b64 decodeBase64AsData:error];
}

- (NSString *)decodeBase64DataAsString:(NSError **)error
{
    unsigned char *result;
    unsigned int result_len;
    *error = nil;
    
    // Assumption that string is ASCII encoded.  In theory, if the string is a proper ASCII
    // formatted string, according to the internet self.UTF8String should not provide
    // any overhead.
    MIG_Result res = MIG_decodeAsBase64(self.bytes, self.length, &result, &result_len);
    if (res == MIG_OK)
    {
        return [[NSString alloc] initWithBytesNoCopy:result
                                              length:result_len
                                            encoding:NSUTF8StringEncoding
                                        freeWhenDone:YES];
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
