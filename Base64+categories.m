//
//  Base64+categories.m
//
//
// Basic Objective-C wrapper around the migbase64 fast base64 conversion routines
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


#import "Base64+categories.h"
#import "MIGConverter.h"

#if !__has_feature(objc_arc)
#error Base64+categories must be built with ARC.
#endif

#pragma mark -
#pragma mark Common functions

NSError *generateErrorStructure(MIG_Result res)
{
    NSMutableDictionary* details = [NSMutableDictionary dictionary];
    if (res == MIG_InputDataEmpty)
    {
        // Special case -- just empty
        [details setValue:NSLocalizedString(@"Base64 input string empty", nil) forKey:NSLocalizedDescriptionKey];
        return [NSError errorWithDomain:kB64IncorrectEncoding code:MIG_InputDataEmpty userInfo:details];
    }
    else if (res == MIG_Base64EncodingInvalid)
    {
        [details setValue:NSLocalizedString(@"Base64 encoding incorrect", nil) forKey:NSLocalizedDescriptionKey];
        return [NSError errorWithDomain:kB64IncorrectEncoding code:MIG_Base64EncodingInvalid userInfo:details];
    }
    else if (res == MIG_NoMemory)
    {
        [details setValue:NSLocalizedString(@"Unable to allocate buffer for result", nil) forKey:NSLocalizedDescriptionKey];
        return [NSError errorWithDomain:kB64InsufficientMemory code:MIG_NoMemory userInfo:details];
    }
    else if (res == MIG_Base64StringEmpty)
    {
        [details setValue:NSLocalizedString(@"Base64 input string empty", nil) forKey:NSLocalizedDescriptionKey];
        return [NSError errorWithDomain:kB64NoData code:MIG_Base64StringEmpty userInfo:details];
    }
    else
    {
        [details setValue:NSLocalizedString(@"Unknown error", nil) forKey:NSLocalizedDescriptionKey];
        return [NSError errorWithDomain:kB64UnknownError code:MIG_Base64UnknownError userInfo:details];
    }
}



#pragma mark -

@implementation NSData (Base64)

#pragma mark Convenience property
@dynamic Base64;
-(NSString *)Base64
{
    NSError *error;
    NSString *result = [self encodeAsBase64UsingLineEndings:NO error:&error];
    if (!result)
        return @"";
    return result;
}


#pragma mark Decoding Base64 to NSData
+ (NSData *)dataFromBase64EncodedString:(NSString *)b64
                                  error:(NSError **)error
{
    return [b64 decodeBase64:error];
}

#pragma mark Encoding Base64 from NSData
-(NSString *)encodeAsBase64UsingLineEndings:(BOOL)useOptionalLineEndings
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


#pragma mark -

@implementation NSString (Base64)

#pragma mark Convenience property
@dynamic Base64;
-(NSString *)Base64
{
    NSError *error;
    NSString *result = [self encodeAsBase64UsingLineEndings:NO error:&error];
    if (!result)
        return @"";
    return result;
}

#pragma mark Encoding Base64 to NSString
+ (NSString *)stringByEncodingDataAsBase64:(NSData *)data
                            withFormatting:(BOOL)useOptionalLineEndings
                                     error:(NSError **)error
{
    return [data encodeAsBase64UsingLineEndings:useOptionalLineEndings error:error];
}

- (NSString *)encodeAsBase64UsingLineEndings:(BOOL)useOptionalLineEndings
                                       error:(NSError **)error
{
    MIG_Result res;
    char *result;
    unsigned int result_len;
    
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

#pragma mark Decoding Base64 from NSString
- (NSData *)decodeBase64:(NSError **)error
{
    MIG_Result res;
    unsigned char *result;
    unsigned int result_len;
    
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






