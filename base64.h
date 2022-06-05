#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<cctype> /* isalnum */
#include<cassert>
#include<cctype> /* isprint */

/* An implementation for base64 encoding and decoding
   Reference: https://en.wikipedia.org/wiki/Base64
   Implements https://datatracker.ietf.org/doc/html/rfc4648#section-4 */

/* Map all possible 6 bit combinations to base64 characters */
std::vector< char > base64_encode_map = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+',
                                          '/'
                                        };

/* Map all base64 characters to numbers */
std::unordered_map< char, uint8_t > base64_decode_map =
{
    { 'A',  0 }, { 'B',  1 }, { 'C',  2 }, { 'D',  3 }, { 'E',  4 },
    { 'F',  5 }, { 'G',  6 }, { 'H',  7 }, { 'I',  8 }, { 'J',  9 },
    { 'K', 10 }, { 'L', 11 }, { 'M', 12 }, { 'N', 13 }, { 'O', 14 },
    { 'P', 15 }, { 'Q', 16 }, { 'R', 17 }, { 'S', 18 }, { 'T', 19 },
    { 'U', 20 }, { 'V', 21 }, { 'W', 22 }, { 'X', 23 }, { 'Y', 24 },
    { 'Z', 25 }, { 'a', 26 }, { 'b', 27 }, { 'c', 28 }, { 'd', 29 },
    { 'e', 30 }, { 'f', 31 }, { 'g', 32 }, { 'h', 33 }, { 'i', 34 },
    { 'j', 35 }, { 'k', 36 }, { 'l', 37 }, { 'm', 38 }, { 'n', 39 },
    { 'o', 40 }, { 'p', 41 }, { 'q', 42 }, { 'r', 43 }, { 's', 44 },
    { 't', 45 }, { 'u', 46 }, { 'v', 47 }, { 'w', 48 }, { 'x', 49 },
    { 'y', 50 }, { 'z', 51 }, { '0', 52 }, { '1', 53 }, { '2', 54 },
    { '3', 55 }, { '4', 56 }, { '5', 57 }, { '6', 58 }, { '7', 59 },
    { '8', 60 }, { '9', 61 }, { '+', 62 }, { '/', 63 }
};

/* Encodes 'bin_data' to base64 format and puts it into 'result' */
std::string base64_encode( std::vector< bool > bin_data )
{
    if( bin_data.size() % 8 != 0 )
    {
        throw std::runtime_error( "Input should be a multiple of 8" );
    }

    int rem = bin_data.size() % 6;
    if( rem == 2 )
    {
        bin_data.insert( bin_data.end(), 4, 0 );
    }
    else if( rem == 4 )
    {
        bin_data.insert( bin_data.end(), 2, 0 );
    }

    int sz = bin_data.size();
    std::string en_res;
    for( int i = 0; i <= sz - 1; i += 6 )
    {
        /* Take 6 bits at a time and use the corresponding base64 character */
        uint8_t group = 0;
        for( int j = i; j <= i + 5; ++j )
        {
            group <<= 1;
            group += bin_data[ j ];
        }
        en_res += base64_encode_map[ group ];
    }

    /* Add "=" padding based on remainder */
    if( rem == 2 )
    {
        en_res += "==";
    }
    else if( rem == 4 )
    {
        en_res += "=";
    }

    return en_res;
}

/* Decodes 'base64_data' to binary format and puts it into 'result' */
std::vector< bool > base64_decode( std::string base64_data )
{
    int sz = base64_data.size();
    /* All base64 encoding lengths have to be a multiple of 4 */
    if( sz % 4 != 0 )
    {
        throw std::runtime_error( "Input should be a multiple of 4" );
    }

    /* Count the number of '='s found in base64_data */
    int equal_cnt = 0;
    for( int i = 0; i <= sz - 1; ++i )
    {
        /* Make sure current character is one of base64 characters */
        if( !isalnum( base64_data[ i ] ) && base64_data[ i ] != '+' &&
            base64_data[ i ] != '/' && base64_data[ i ] != '=' )
        {
            throw std::runtime_error( "Input contains non base64 character" );
        }
        if( base64_data[ i ] == '=' )
        {
            ++equal_cnt;
        }
    }
    if( equal_cnt > 2 )
    {
        throw std::runtime_error( "Input contains more than 2 '='s" );
    }
    /* Make sure that no '=' is present in the midst of base64 characters
       aka. all '='s are to the utmost right */
    if( equal_cnt >= 1 && base64_data[ sz - 1 ] != '=' )
    {
        throw std::runtime_error( "'=' is not the last character" );
    }
    if( equal_cnt == 2 && base64_data[ sz - 2 ] != '=' )
    {
        throw std::runtime_error( "'=' is not the second last character" );
    }

    std::vector< bool > de_res;
    for( int i = 0; i <= sz - 1 - equal_cnt; ++i )
    {
        /* Get the value and put its binary representation in result */
        uint8_t val = base64_decode_map[ base64_data[ i ] ]; // O( 1 ) access
        uint8_t mask = 0b00100000;
        for( int j = 1; j <= 6; ++j )
        {
            de_res.emplace_back( ( ( val & mask ) ? 1 : 0 ) );
            mask >>= 1;
        }
    }

    /* Erase additional zeroes that were added during encoding */
    if( equal_cnt == 1 )
    {
        de_res.erase( de_res.begin() + de_res.size() - 2 , de_res.end() );
    }
    else if( equal_cnt == 2 )
    {
        de_res.erase( de_res.begin() + de_res.size() - 4, de_res.end() );
    }

    /* Sanity check */
    assert( de_res.size() % 8 == 0 );
    return de_res;
}
