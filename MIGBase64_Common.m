//
//  MIGCommon.m
//  Base64_Tests
//
//  Created by Darren Ford on 21/11/12.
//  Copyright (c) 2012 Darren Ford. All rights reserved.
//

#import "MIGBase64_Common.h"

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


