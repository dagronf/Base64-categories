/////////////////////////////////////////////////////////////////////////////////////////////////
//
// MIGBase64.h file
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

#import <Foundation/Foundation.h>


@interface MIGBase64 : NSObject

#pragma mark Static creators
+ (id)create;
+ (id)createWithData:(NSData *)data useFormatting:(BOOL)f;
+ (id)createWithString:(NSString *)string useFormatting:(BOOL)f;
+ (id)createWithBase64:(NSString *)base64String;

#pragma mark Initializers
- (id)init;
- (id)initWithData:(NSData *)data useFormatting:(BOOL)f;
- (id)initWithString:(NSString *)string useFormatting:(BOOL)f;
- (id)initWithBase64:(NSString *)base64String;

#pragma mark Properties

/** Use formatting when encoding data */
@property BOOL useFormatting;

/** The encapsulated base64 string */
@property (retain) NSData *base64;

/** Setter and getter for raw data */
@property NSData *data;

/** Setter and getter for raw strings */
@property NSString *string;

#pragma mark Read-only properties

/** When a call errors, this is set to the error */
@property (readonly) NSError *lastError;

@end

