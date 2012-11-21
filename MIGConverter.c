/*
    MIGConverter.h
    A C port of the fast Base64 encoding/decoding routines in the migbase64 project
 
    Port created by Darren Ford on 22/10/12.
 
    Ported from
    http://migbase64.cvs.sourceforge.net/viewvc/migbase64/migbase64/src/util/Base64.java?revision=1.2&content-type=text%2Fplain
 
    As much of the original code style and allocation is preserved as possible
    Note that this port is in no way associated with the original Java code or the migbase64 author
 */


/** A very fast and memory efficient class to encode and decode to and from BASE64 in full accordance
 * with RFC 2045.<br><br>
 * On Windows XP sp1 with 1.4.2_04 and later ;), this encoder and decoder is about 10 times faster
 * on small arrays (10 - 1000 bytes) and 2-3 times as fast on larger arrays (10000 - 1000000 bytes)
 * compared to <code>sun.misc.Encoder()/Decoder()</code>.<br><br>
 *
 * On byte arrays the encoder is about 20% faster than Jakarta Commons Base64 Codec for encode and
 * about 50% faster for decoding large arrays. This implementation is about twice as fast on very small
 * arrays (&lt 30 bytes). If source/destination is a <code>String</code> this
 * version is about three times as fast due to the fact that the Commons Codec result has to be recoded
 * to a <code>String</code> from <code>byte[]</code>, which is very expensive.<br><br>
 *
 * This encode/decode algorithm doesn't create any temporary arrays as many other codecs do, it only
 * allocates the resulting array. This produces less garbage and it is possible to handle arrays twice
 * as large as algorithms that create a temporary array. (E.g. Jakarta Commons Codec). It is unknown
 * whether Sun's <code>sun.misc.Encoder()/Decoder()</code> produce temporary arrays but since performance
 * is quite low it probably does.<br><br>
 *
 * The encoder produces the same output as the Sun one except that the Sun's encoder appends
 * a trailing line separator if the last character isn't a pad. Unclear why but it only adds to the
 * length and is probably a side effect. Both are in conformance with RFC 2045 though.<br>
 * Commons codec seem to always att a trailing line separator.<br><br>
 *
 * <b>Note!</b>
 * The encode/decode method pairs (types) come in three versions with the <b>exact</b> same algorithm and
 * thus a lot of code redundancy. This is to not create any temporary arrays for transcoding to/from different
 * format types. The methods not used can simply be commented out.<br><br>
 *
 * There is also a "fast" version of all decode methods that works the same way as the normal ones, but
 * har a few demands on the decoded input. Normally though, these fast verions should be used if the source if
 * the input is known and it hasn't bee tampered with.<br><br>
 *
 * If you find the code useful or you find a bug, please send me a note at base64 @ miginfocom . com.
 *
 * Licence (BSD):
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



#include <stdio.h>
#include "stdlib.h"

#include "MIGConverter.h"

static const char *CA = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int IA[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1,  0, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


/** Encodes a raw byte array into a BASE64 <code>char[]</code> representation i accordance with RFC 2045.
 * No line separator will be in breach of RFC 2045 which specifies max 76 per line but will be a
 * little faster.
 * @return A BASE64 encoded array. Never <code>null</code>.
 */


MIG_Result MIG_encodeAsBase64(int useOptionalLineEndings,
                              const unsigned char *sArr,
                              unsigned int sLen,
                              char **result,
                              unsigned int *resultLen)
{
    /* Check special case */
    if (sArr == NULL)
    {
        /* Special case -- shouldn't deal with it */
        return MIG_InputDataEmpty;
    }
    else if (sLen == 0)
    {
        /* Empty string -- return empty string according to RFC */
        *result = (char *)calloc(1, sizeof(char));
        *resultLen = 0;
        return MIG_OK;
    }
    
    int eLen = (sLen / 3) * 3;              /* Length of even 24-bits. */
    int cCnt = ((sLen - 1) / 3 + 1) << 2;   /* Returned character count */
    int dLen = cCnt + ((useOptionalLineEndings==1) ? (cCnt - 1) / 76 << 1 : 0); /* Length of returned array */

    /* Create the storage array.  When complete, the array will become contained
       within the returned NSString object, so it will be freed when the result
       object is released. */
    char *dArr = (char *)calloc(dLen, sizeof(char));
    if (dArr == NULL)
    {
        return MIG_NoMemory;
    }
    
    /* Encode even 24-bits */
    for (int s = 0, d = 0, cc = 0; s < eLen;)
    {
        /* Copy next three bytes into lower 24 bits of int, paying attension to sign. */
        int i = (sArr[s++] & 0xff) << 16 | (sArr[s++] & 0xff) << 8 | (sArr[s++] & 0xff);
        
        /* Encode the int into four chars */
        dArr[d++] = CA[(i >> 18) & 0x3f];
        dArr[d++] = CA[(i >> 12) & 0x3f];
        dArr[d++] = CA[(i >> 6) & 0x3f];
        dArr[d++] = CA[i & 0x3f];
        
        /* Add optional line separator */
        if ((useOptionalLineEndings==1) && ++cc == 19 && d < dLen - 2)
        {
            dArr[d++] = '\r';
            dArr[d++] = '\n';
            cc = 0;
        }
    }
    
    /* Pad and encode last bits if source isn't even 24 bits. */
    int left = sLen - eLen; /* 0 - 2. */
    if (left > 0)
    {
        /* Prepare the int */
        int i = ((sArr[eLen] & 0xff) << 10) | (left == 2 ? ((sArr[sLen - 1] & 0xff) << 2) : 0);
        
        /* Set last four chars */
        dArr[dLen - 4] = CA[i >> 12];
        dArr[dLen - 3] = CA[(i >> 6) & 0x3f];
        dArr[dLen - 2] = left == 2 ? CA[i & 0x3f] : '=';
        dArr[dLen - 1] = '=';
    }

    *result = dArr;
    *resultLen = dLen;
    
    return MIG_OK;
}

/** Decodes a BASE64 encoded char array. All illegal characters will be ignored and can handle both arrays with
 * and without line separators.
 * @param sArr The source array. <code>null</code> or length 0 will return an empty array.
 * @return The decoded array of bytes. May be of length 0. Will be <code>null</code> if the legal characters
 * (including '=') isn't divideable by 4.  (I.e. definitely corrupted).
 */

MIG_Result MIG_decodeAsBase64(const char *sArr,
                              unsigned int sLen,
                              unsigned char **result,
                              unsigned int *resultLen)
{
    if (sArr == NULL)
    {
        return MIG_Base64StringEmpty;
    }
    else if (sLen == 0)
    {
        /* Empty string -- return empty string according to RFC */
        *result = (unsigned char *)calloc(1, sizeof(unsigned char));
        *resultLen = 0;
        return MIG_OK;
    }
    
    /* Count illegal characters (including '\r', '\n') to know what size the returned array will be,
       so we don't have to reallocate & copy it later. */
    int sepCnt = 0; /* Number of separator characters. (Actually illegal characters, but that's a bonus...) */
    for (int i = 0; i < sLen; i++)  /* If input is "pure" (I.e. no line separators or illegal chars) base64 this loop can be commented out. */
    {
        char c = sArr[i];
        if (IA[c] < 0)
        {
            sepCnt++;
        }
    }
    
    /* Check so that legal chars (including '=') are evenly divideable by 4 as specified in RFC 2045. */
    if ((sLen - sepCnt) % 4 != 0)
    {
        return MIG_Base64EncodingInvalid;
    }
    
    int pad = 0;
    for (int i = sLen; i > 1 && IA[sArr[--i]] <= 0;)
    {
        char c = sArr[i];
        if (c == '=')
            pad++;
    }
    
    int dLen = ((sLen - sepCnt) * 6 >> 3) - pad;
    
    unsigned char *dArr = (unsigned char *)calloc(dLen, sizeof(unsigned char));
    if (dArr == NULL)
    {
        return MIG_NoMemory;
    }
    
    for (int s = 0, d = 0; d < dLen;)
    {
        /* Assemble three bytes into an int from four "valid" characters. */
        int i = 0;
        for (int j = 0; j < 4; j++, s++)
        {
            /* j only increased if a valid char was found. */
            char r = sArr[s];
            int c = IA[r];
            if (c >= 0)
                i |= c << (18 - j * 6);
            else
                j--;
        }
        /* Add the bytes */
        dArr[d++] = (unsigned char) (i >> 16);
        if (d < dLen)
        {
            dArr[d++] = (unsigned char) (i >> 8);
            if (d < dLen)
            {
                dArr[d++] = (unsigned char) i;
            }
        }
    }
    
    *result = dArr;
    *resultLen = dLen;
    
    return MIG_OK;
}


/** Decodes a BASE64 encoded byte array that is known to be resonably well formatted. The method is about twice as
 * fast as {@link #decode(byte[])}. The preconditions are:<br>
 * + The array must have a line length of 76 chars OR no line separators at all (one line).<br>
 * + Line separator must be "\r\n", as specified in RFC 2045
 * + The array must not contain illegal characters within the encoded string<br>
 * + The array CAN have illegal characters at the beginning and end, those will be dealt with appropriately.<br>
 * @param sArr The source array. Length 0 will return an empty array. <code>null</code> will throw an exception.
 * @return The decoded array of bytes. May be of length 0.
 */
MIG_Result MIG_decodeAsBase64Fast(const char *sArr,
                                  unsigned int sLen,
                                  unsigned char **result,
                                  unsigned int *resultLen)
{
    /* Check special case */
    if (sArr == NULL)
    {
        return MIG_InputDataEmpty;
    }
    else if (sLen == 0)
    {
        /* Empty string -- return empty string according to RFC */
        *result = (unsigned char *)calloc(1, sizeof(unsigned char));
        *resultLen = 0;
        return MIG_OK;
    }
    
    int sIx = 0, eIx = sLen - 1;    /* Start and end index after trimming. */
    
    /* Trim illegal chars from start */
    while (sIx < eIx && IA[sArr[sIx] & 0xff] < 0)
        sIx++;
    
    /* Trim illegal chars from end */
    while (eIx > 0 && IA[sArr[eIx] & 0xff] < 0)
        eIx--;
    
    /* get the padding count (=) (0, 1 or 2) */
    int pad = sArr[eIx] == '=' ? (sArr[eIx - 1] == '=' ? 2 : 1) : 0;  /* Count '=' at end. */
    int cCnt = eIx - sIx + 1;   /* Content count including possible separators */
    int sepCnt = sLen > 76 ? (sArr[76] == '\r' ? cCnt / 78 : 0) << 1 : 0;
    
    int dLen = ((cCnt - sepCnt) * 6 >> 3) - pad; /* The number of decoded bytes */

    unsigned char *dArr = (unsigned char *)calloc(dLen, sizeof(unsigned char));
    if (dArr == NULL)
    {
        return MIG_NoMemory;
    }
    
    /* Decode all but the last 0 - 2 bytes. */
    int d = 0;
    for (int cc = 0, eLen = (dLen / 3) * 3; d < eLen;)
    {
        /* Assemble three bytes into an int from four "valid" characters. */
        int i = IA[sArr[sIx++]] << 18 | IA[sArr[sIx++]] << 12 | IA[sArr[sIx++]] << 6 | IA[sArr[sIx++]];
        
        /* Add the bytes */
        dArr[d++] = (unsigned char) (i >> 16);
        dArr[d++] = (unsigned char) (i >> 8);
        dArr[d++] = (unsigned char) i;
        
        /* If line separator, jump over it. */
        if (sepCnt > 0 && ++cc == 19) {
            sIx += 2;
            cc = 0;
        }
    }
    
    if (d < dLen)
    {
        /* Decode last 1-3 bytes (incl '=') into 1-3 bytes */
        int i = 0;
        for (int j = 0; sIx <= eIx - pad; j++)
            i |= IA[sArr[sIx++]] << (18 - j * 6);
        
        for (int r = 16; d < dLen; r -= 8)
            dArr[d++] = (unsigned char) (i >> r);
    }
    
    *result = dArr;
    *resultLen = dLen;
    
    return MIG_OK;
}


