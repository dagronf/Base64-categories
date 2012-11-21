//
//  NSData+MIGBase64.h
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

#import <Foundation/Foundation.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
// NSData additions for speed
//   Dealing with NSString conversions can be very time consuming due to the encoding
//   requirements.
//   The following routines deal only with NSData, and as such are MUCH faster than their
//   NSString related counterparts
/////////////////////////////////////////////////////////////////////////////////////////////////

#pragma mark -
#pragma mark NSData Base 64 additions (fastest routines)
@interface NSData (MIGBase64_FAST)

#pragma mark Decoders


/** Decodes from the passed in NSData object, and returns a new NSData object on success
 If an error occurs, returns nil.  Use the error object to determine the failure. */
+ (NSData *)dataFromBase64EncodedChars:(const char *)data
                                length:(int)length
                                 error:(NSError **)error;

/** Decodes from the passed in NSData object, and returns a new NSData object on success
 If an error occurs, returns nil.  Use the error object to determine the failure. */
+ (NSData *)dataFromBase64EncodedData:(NSData *)data
                                error:(NSError **)error;

/** Decodes from the passed in NSData object, and returns a new NSData object on success
 If an error occurs, returns nil.  Use the error object to determine the failure. */
- (NSData *)decodeFromBase64Data:(NSError **)error;

#pragma mark Encoders

/** Creates an NSString object containing the base64 encoding of 'data' using line formatting
 if specified by 'useOptionalLineEndings'.
 If an error occurs, returns nil.  Use the error object to determine the failure */
+ (NSString *)stringByEncodingDataAsBase64:(NSData *)data
                            withFormatting:(BOOL)useOptionalLineEndings
                                     error:(NSError **)error;

/** Returns a base-64 encoded NSData based on the current data, using line formatting
 if specified by 'useOptionalLineEndings */
- (NSData *)encodeAsBase64DataUsingLineEndings:(BOOL)useOptionalLineEndings
                                         error:(NSError **)error;

/** Returns a base-64 encoded NSString based on the current data, using line formatting
 if specified by 'useOptionalLineEndings */
- (NSString *)encodeAsBase64StringUsingLineEndings:(BOOL)useOptionalLineEndings
                                             error:(NSError **)error;

@end


/////////////////////////////////////////////////////////////////////////////////////////////////
// NSData additions
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark NSData Base 64 additions (slower, NSString-based)
@interface NSData (MIGBase64)


/** Property to provide a dirty property-style access to Base64 data
 For example,
 NSString *result = <NSData_object>.Base64;
 This is a convenience only, doesn't include formattings and doesn't deal
 with errors other than to return an empty string.
 For robust code you should use the other methods provided.
 */
@property (readonly) NSString *Base64;

/** Creates an NSData object from the decoding of the input string
 If an error occurs, returns nil.  Use the error object to determine the failure
 Note that the results are undefined if the input string is not Base64
 (ie. ASCII chars only)
 
 Unfortunately, this can be slow as the code needs to convert the base64 NSString
 to a UTF8 encoding in order to perform the conversion.  If you need speed, then
 use the dataFromBase64EncodedChars or decodeFromBase64Data below.
 */
+ (NSData *)dataFromBase64EncodedString:(NSString *)b64
                                  error:(NSError **)error;

/** Decodes the Base64-encoded data from the object into an NSString
 If an error occurs, returns nil.  Use the error object to determine the failure
 Note that the results are undefined if the input data is not Base64
 (ie. ASCII chars only)
 
 Unfortunately, this can be slow as the code needs to convert the base64 NSString
 to a UTF8 encoding in order to perform the conversion.  If you need speed, then
 use the dataFromBase64EncodedChars or decodeFromBase64Data below.
 */
- (NSString *)decodeBase64DataAsString:(NSError **)error;

@end


