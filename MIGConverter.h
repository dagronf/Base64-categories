/*
    MIGConverter.h
    A C port of the fast Base64 encoding/decoding routines in the migbase64 project
 
    Port created by Darren Ford on 22/10/12.

    Ported from
    http://migbase64.cvs.sourceforge.net/viewvc/migbase64/migbase64/src/util/Base64.java?revision=1.2&content-type=text%2Fplain

    As much of the original code style and allocation is preserved as possible
    Note that this port is in no way associated with the original Java code or the migbase64 author
*/

#pragma mark -
#pragma mark MIGBase64 licence agreement

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

#ifndef MIGConverter_h
#define MIGConverter_h

typedef enum eMIG_Result
{
    MIG_OK = 0,                         /* Conversion successful */
    MIG_InputDataEmpty = -1,            /* No input data was supplied (sArr == NULL) */
    MIG_NoMemory = -2,                  /* Could not allocate memory for result array */
    MIG_Base64StringEmpty = -3,         /* Supplied Base64 string for decoding was NULL */
    MIG_Base64EncodingInvalid = -4,     /* Base64 string for decoding wasn't valid Base64 */
    MIG_Base64UnknownError = -5,        /* An unknown error occurred */
} MIG_Result;

/** 
    Encodes the supplied byte array 'sArr' into the result array 'result'.
    Parameters :-
      useOptionalLineEndings:  0 == unformated, all else == formatted
      sArr: the byte array to be converted
      sLen: the length of the supplied array 'sArr'
      result: the resulting encoded array.  Caller must free() the returned memory.
      resultLen: the length (in bytes) of the result array 'result'.
    Returns :-
      The status of the call (see eMIG_Result enum)
*/
MIG_Result MIG_encodeAsBase64(int useOptionalLineEndings,
                              const unsigned char *sArr,
                              unsigned int sLen,
                              char **result,
                              unsigned int *resultLen);

/** 
    Decodes the supplied Base64 encoded array 'sArr' into the result array 'result'.
    Parameters :-
      sArr: the byte array to be decoded
      sLen: the length of the supplied array 'sArr'
      result: the resulting decoded array.  Caller must free() the returned memory.
      resultLen: the length (in bytes) of the result array 'result'.
    Returns :-
      The status of the call (see eMIG_Result enum)
*/
MIG_Result MIG_decodeAsBase64(const char *sArr,
                              unsigned int sLen,
                              unsigned char **result,
                              unsigned int *resultLen);


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
                                  unsigned int *resultLen);

#endif


