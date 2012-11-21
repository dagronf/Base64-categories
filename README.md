# Base64+categories

## Overview

C/Objective-C wrapper for the MiGBase64 fast Base64 conversion libraries [located here](http://migbase64.sourceforge.net "Original Java source code"), originally written by [Mikael Grev](http://sourceforge.net/users/mgrev "Original author") and MiG InfoCom AB.

### MiGBase64 port

The MiGBase64 conversion libraries (originally written in Java) provide very fast and memory efficient conversion to and from Base64 encodings.  The core MiGBase64 code has been ported to pure C (MIGConverter.h.c), retaining as much of the original code style and allocation behaviours as possible.

***Note that this port is in no way associated with the original Java code or the migbase64 author***.

### Objective-C support

The Objective-C category and classes are designed to minimise memory allocation/copying/duplication during the process of conversion to/from Base64, maintaining the speed and performance of the original Java source whilst maintaining the use of core Objective-C primitives (NSData and NSString).

### Original source link

Originally ported from [revision 1.2](http://migbase64.cvs.sourceforge.net/viewvc/migbase64/migbase64/src/util/Base64.java?revision=1.2&content-type=text%2Fplain "Original MiGBase64 revision link") of the MiGBase64 library.

Please note the license for the MiGBase64 library at the end of this file (BSD license).

### Requirements

For the Objective-C components, Base64+categories currently requires ARC and will flag an error if built in a non-ARC environment.

## Classes and categories

### MIGConverter.h.c

The two files 'MIGConverter.c' and 'MIGConverter.h' are ports of the code originally written Java to raw C.  Fundamentally, the only changes required were for handling parameter passing and to unroll a 'static' function that prepared an intermediary 
array for fast processing.

The two function calls in MIGConverter.c return an internally allocated memory block for the result (when conversion is successful).  The caller MUST free() the result array or else a memory leak will occur.

The core C port (MIGConverter.c.h) is completely independent of the Objective-C code, which means it can be incorporated into other projects that can import or directly access C code.

### MIGCommon.m.h, NSData+MIGBase64.m.h, NSString+MIGBase64.m.h

These files are Objective-C (ARC) categories sitting on the top of the MIGConverter port.  These files provide Base64 categories for NSData and NSString - refer to the header file for descriptions of the supplied methods.

### MIGBase64.h.m

The two files 'MIGBase64.h' and 'MIGBase64.m' are a (basic) class wrapper for the provided categories.  I find it cleaner in the code (particularly when dealing with base64-encoded NSStrings) to hand around an explicit Base64 object - makes it obvious in functions what to expect when you're handed the data by another function.

## Important note regarding performance
Using NSStrings when converting to/from Base64 puts a huge penalty on conversion speed, as the NSString (in many cases) needs to be encoded to UTF8 encoding before a decode can take place

For raw speed, use the NSData (MIGBase64_FAST) category functions, located in NSData+MIGBase64.  The functions have very little overhead on the top of the base MiGBase64 conversion code.

Basically, any encode/decode routines (with one or two exceptions) that involve using NSString are slower functions.

## Simple examples

### Basic Category examples

Encode a string to a Base64 String

      NSString *b64Encoded = @"This is a test".Base64;

Decode a Base64 encoded string into a string

      NSError *err;
      NSString *encodedVector = @"Zm9vYmFy";
      NSString *result = [encodedVector decodeBase64AsString:&err];
      if (err) { <do something with error> }

Encode an NSData object to a Base64 String

      NSError *err;
      NSData *rawData = [NSData dataWithContentsOfFile:<some path>];
      NSString *result = [rawData encodeAsBase64UsingLineEndings:NO error:&err];
      if (err) { <do something with error> }

Decode a Base64 encoded string into an NSData object

      NSError *err;
      NSData *rawData = [base64EncodedNSString decodeBase64:&err];
      if (err) { <do something with error> }


### MIGBase64 class examples

Basic example of class usage

      MIGBase64 *b64 = [[MIGBase64 alloc] init];
      b64.data = [NSData dataWithContentsOfFile:<some path>];
      if (b64.lastError) { <do something with error> }
      
      NSString *encoded = b64.base64;

Create a instance with a basic string, and grab out the components

      NSString *testPhrase = @"Testing class encoding";
      MIGBase64 *b64 = [MIGBase64 createWithString:testPhrase useFormatting:YES];
      if (b64.lastError) { <do something with error> }
    
      NSData *decodedData = b64.data;          // NSData representation of 'testPhrase'
      NSString *decodedString = b64.string;    // NSString representation of 'testPhrase'
      NSString *base64String = b64.base64;     // Base64 representation of 'testPhrase'

Base64 encode an NSData object

      NSData *rawData = [NSData dataWithContentsOfFile:<some path>];
      MIGBase64 *b64 = [MIGBase64 createWithData:rawData useFormatting:YES];
      if (b64.lastError) { <do something with error> }
      NSString *base64String = obj.base64;

Decode a Base64 String

      MIGBase64 *obj = [MIGBase64 createWithBase64:base64EncodedString];
      if (obj.lastError) { <do something with error> }
      NSData *data = obj.data;

# Licenses
## License for Base64+categories (MIT-based)

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
    OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE. 
 
## Licence for MiGBase64 (BSD-based)

    Licence (BSD):
    ==============
    
    Copyright (c) 2004, Mikael Grev, MiG InfoCom AB. (base64 @ miginfocom . com)
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:
    Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.
    Neither the name of the MiG InfoCom AB nor the names of its contributors may be
    used to endorse or promote products derived from this software without specific
    prior written permission.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
    OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
    OF SUCH DAMAGE.
    
    @version 2.2
    @author Mikael Grev
            Date: 2004-aug-02
            Time: 11:31:11


