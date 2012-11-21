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


#import <Foundation/Foundation.h>

#pragma mark -
#pragma mark Error domains for the NSError object

#define kB64IncorrectEncoding       @"IncorrectEncoding"     // Input base64 string isn't base 64
#define kB64NoData                  @"NoData"                // Input data for conversion is empty
#define kB64InsufficientMemory      @"NotEnoughMemory"       // Failed to allocate buffer space
#define kB64UnknownError            @"UnknownError"          // Unknown error

/////////////////////////////////////////////////////////////////////////////////////////////////
// NSData additions
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark NSData Base 64 additions
@interface NSData (Base64)

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
    (ie. ASCII chars only) */
+ (NSData *)dataFromBase64EncodedString:(NSString *)b64
                                  error:(NSError **)error;

/** Returns a base-64 encoded string based on the current data, using line formatting
    if specified by 'useOptionalLineEndings */
- (NSString *)encodeAsBase64UsingLineEndings:(BOOL)useOptionalLineEndings
                                       error:(NSError **)error;

@end



/////////////////////////////////////////////////////////////////////////////////////////////////
// NSString additions
/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark NSString Base 64 additions
@interface NSString (Base64)

/** Property to provide a quick property-style access to the Base64 data
    For example,
        NSString *result = @"My string".Base64;
    This is a convenience only, doesn't include formattings and doesn't deal
    with errors other than to return an empty string.
    For robust code conditions (for example when the NSString is coming in
    from an external source) you should really use the other methods provided.
*/
@property (readonly) NSString *Base64;

/** Creates an NSString object containing the base64 encoding of 'data' using line formatting
    if specified by 'useOptionalLineEndings'.
    If an error occurs, returns nil.  Use the error object to determine the failure */
+ (NSString *)stringByEncodingDataAsBase64:(NSData *)data
                            withFormatting:(BOOL)useOptionalLineEndings
                                     error:(NSError **)error;

/** This is a convenience function.
    Encodes the string object as Base64, and returns a new NSString object on success
    If an error occurs, returns nil.  Use the error object to determine the failure.
    NOTE: not all input strings can be defined as UTF8, thus this function
    should only be used for very basic cases */
- (NSString *)encodeAsBase64UsingLineEndings:(BOOL)useOptionalLineEndings
                                       error:(NSError **)error;

/** Decodes from the passed in string object, and returns a new NSData object on success
    If an error occurs, returns nil.  Use the error object to determine the failure.
    Note that the results are undefined if the input string is not Base64
    (ie. ASCII chars only) */
- (NSData *)decodeBase64:(NSError **)error;

/** Decodes from the passed in string object, and returns a new NSString object on success
    If an error occurs, returns nil.  Use the error object to determine the failure.
    NOTE: the results are undefined if the input Base64 does not define an encoded UTF8 
          string.  If the encoded string has come from an unknown source (eg. a web page)
          then you should use 'decodeBase64' and parse the result yourself */
- (NSString *)decodeBase64AsString:(NSError **)error;

@end






