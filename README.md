Base64+categories
=================

Overview
--------

Objective-C Wrapper for the migbase64 fast Base64 conversion libraries

The migbase64 conversion libraries (originally written in Java) provide very fast and
memory efficient conversion to and from Base64 encodings.  The Objective-C wrapper
is designed to minimise memory allocation/copying/duplication during the process
of conversion to/from Base64.  The raw conversion code is very fast and minimal,
attempting to reduce the time and effort to perform the conversions.

MIGConverter.h.c
----------------
The two files 'MIGConverter.c' and 'MIGConverter.h' are ports of the code originally
written Java to raw C.  Fundamentally, the only changes required were for handling
parameter passing and to unroll a 'static' function that prepared an intermediary 
array for fast processing.

MIGBase64+categories.h.m
---------------------
The two files 'MIGBase64+categories.m' and 'MIGBase64+categories.h' are an Objective-C (ARC)
wrapper on the top of the migbase64 port.

MIGBase64.h.m
----------
The two files 'MIGBase64.h' and 'MIGBase64.m' are a (basic) class wrapper for the 
provided categories.  I find it cleaner in the code (particularly when dealing with
base64-encoded NSStrings) to hand around an explicit Base64 object - makes it
obvious in functions what to expect the data to contain.

Simple examples
===============

Basic Category examples
-----------------------

Encode a string to a Base64 String
    NSString *b64Encoded = @"This is a test".Base64;

Decode a Base64 encoded string into a string
    NSError *err;
    NSString *encodedVector = @"Zm9vYmFy";
    NSString *result = [encodedVector decodeBase64AsString:&err];

Encode an NSData object to a Base64 String
    NSError *err;
    NSData *rawData = [NSData dataWithContentsOfFile:<some path>];
    NSString *result = [rawData encodeAsBase64UsingLineEndings:NO error:&err];

MIGBase64 class examples
------------------------

Create a instance with a basic string, and grab out the components
    NSString *testPhrase = @"Testing class encoding";
    MIGBase64 *b64 = [MIGBase64 createWithString:testPhrase useFormatting:YES];
    
    NSData *decodedData = b64.data;          // NSData representation of 'testPhrase'
    NSString *decodedString = b64.string;    // NSString representation of 'testPhrase'
    NSString *base64String = b64.base64;     // Base64 representation of 'testPhrase'

Base64 encode an NSData object
    NSError *err;
    NSData *rawData = [NSData dataWithContentsOfFile:<some path>];
    MIGBase64 *obj = [MIGBase64 createWithData:rawData useFormatting:YES];
    NSString *base64String = obj.base64;

Licenses
========

License for Base64+categories (MIT)
-----------------------------------
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
 
Licence for migbase64 (BSD)
---------------------------

/* Licence (BSD):
 * ==============
 *
 * Copyright (c) 2004, Mikael Grev, MiG InfoCom AB. (base64 @ miginfocom . com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 * Neither the name of the MiG InfoCom AB nor the names of its contributors may be
 * used to endorse or promote products derived from this software without specific
 * prior written permission.
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
 *
 * @version 2.2
 * @author Mikael Grev
 *         Date: 2004-aug-02
 *         Time: 11:31:11
 */