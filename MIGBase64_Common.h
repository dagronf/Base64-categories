//
//  MIGCommon.h
//  Base64_Tests
//
//  Created by Darren Ford on 21/11/12.
//  Copyright (c) 2012 Darren Ford. All rights reserved.
//

#ifndef Base64_Tests_MIGCommon_h
#define Base64_Tests_MIGCommon_h

#import "MIGConverter.h"

#pragma mark -
#pragma mark Error domains for the NSError object

#define kB64IncorrectEncoding       @"IncorrectEncoding"     // Input base64 string isn't base 64
#define kB64NoData                  @"NoData"                // Input data for conversion is empty
#define kB64InsufficientMemory      @"NotEnoughMemory"       // Failed to allocate buffer space
#define kB64UnknownError            @"UnknownError"          // Unknown error


NSError *generateErrorStructure(MIG_Result res);

#endif
