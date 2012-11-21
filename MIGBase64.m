/////////////////////////////////////////////////////////////////////////////////////////////////
//
// MIGBase64.m file
// Created by Darren Ford on 21/11/12.
//
// Basically a class wrapper for the categories specified in Base64+categories.h
// Having a Base64 class means that your code is much more understandable when handing
// objects throughout the code.
//
// Internally, all storage is done as a Base64 string.  When setters and getters are called
// (ie. data and string) then the stored data is automatically converted before being returned
// Note that this means that if you are calling the getters for 'data' or 'string' a lot with
// the same stored value, you'd be better off caching the original result, as these getters
// are relatively computationally intensive.
//
// Note also that 'useFormatting' property ONLY applies when _setting_ 'string' or 'data'
// properties.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

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


#import "MIGBase64.h"
#import "MIGBase64+categories.h"

#if !__has_feature(objc_arc)
#error MIGBase64+categories must be built with ARC.
#endif

@implementation MIGBase64

+ (id)create
{
    return [[MIGBase64 alloc] init];
}

+ (id)createWithData:(NSData *)data useFormatting:(BOOL)f
{
    return [[MIGBase64 alloc] initWithData:data useFormatting:f];
}

+ (id)createWithString:(NSString *)string useFormatting:(BOOL)f
{
    return [[MIGBase64 alloc] initWithString:string useFormatting:f];
}

+ (id)createWithBase64:(NSString *)base64String
{
    return [[MIGBase64 alloc] initWithBase64:base64String];
}

- (id)init
{
    id s = [super init];
    if (s)
    {
        _useFormatting = FALSE;
    }
    return s;
}

- (id)initWithData:(NSData *)data useFormatting:(BOOL)f
{
    id s = [super init];
    if (s)
    {
        _useFormatting = f;
        self.data = data;
    }
    return s;
}

- (id)initWithString:(NSString *)string useFormatting:(BOOL)f
{
    id s = [super init];
    if (s)
    {
        _useFormatting = f;
        self.string = string;
    }
    return s;
}

- (id)initWithBase64:(NSString *)base64String
{
    id s = [super init];
    if (s)
    {
        _useFormatting = FALSE;
        self.base64 = base64String;
    }
    return s;
}

- (NSString *)description
{
    NSString *result = [NSString stringWithFormat:@"formatting: %@\r\nBase64: %@", [NSNumber numberWithBool:_useFormatting], _base64];
    return result;
}

- (void)encodeWithCoder:(NSCoder *)coder
{
    [coder encodeBool:_useFormatting forKey:@"formatting"];
	[coder encodeObject:_base64 forKey:@"base64"];
}

- (id)initWithCoder:(NSCoder *)coder
{
    _useFormatting = [coder decodeBoolForKey:@"formatting"];
	_base64 = [coder decodeObjectForKey:@"base64"];
	return self;
}

- (void)setData:(NSData *)data
{
    NSError *err;
    _base64 = [data encodeAsBase64UsingLineEndings:_useFormatting error:&err];
    _lastError = err;
}

- (void)setString:(NSString *)str
{
    NSError *err;
    _base64 = [str encodeAsBase64UsingLineEndings:_useFormatting error:&err];
    _lastError = err;
}

- (NSData *)data
{
    NSError *err;
    NSData *data = [_base64 decodeBase64:&err];
    _lastError = err;
    return data;
}

- (NSString *)string
{
    NSError *err;
    NSString *str = [_base64 decodeBase64AsString:&err];
    _lastError = err;
    return str;
}

@end
