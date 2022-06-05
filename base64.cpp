#include"base64.h"

int main( int argc, char *argv[] )
{
    if( argc != 3 && argc != 4 )
    {
        std::cout << "base64 <operation> <representation> [input]\n"
                     "\n"
                     "operation:       -e  Encode input to base64 format\n"
                     "                 -d  Decode input from base64 format\n"
                     "\n"
                     "representation:  -a  Use ascii representation for IO\n"
                     "                 -b  Use binary representation for IO\n"
                     "\n"
                     "input:               For encoding: Any valid ascii/\n"
                     "                     binary data   \n"
                     "                     For decoding: Any valid base64\n"
                     "                     data\n"
                     "                     If left empty, would wait for\n"
                     "                     input from terminal\n";
        return -1;
    }
    std::vector< std::string > cmd{ argv[ 0 ], argv[ 1 ], argv[ 2 ] };

    /* 2nd parameter indicates whether to encode or decode */
    bool encode = true;
    if( cmd[ 1 ] == "-d" )
    {
        encode = false;
    }
    else if( cmd[ 1 ] != "-e" )
    {
        std::cout << "2nd parameter should be either of '-e' or '-d'" << "\n";
        return -1;
    }

    /* 3rd parameter indicates whether the input/output should be in
       ascii or binary representation */
    bool ascii = true;
    if( cmd[ 2 ] == "-b" )
    {
        ascii = false;
    }
    else if( cmd[ 2 ] != "-a" )
    {
        std::cout << "3rd parameter should be either of '-a' or '-b'" << "\n";
        return -1;
    }

    /* Either take the one provided before invoking or read through getline */
    if( argc == 4 )
    {
        cmd.emplace_back( argv[ 3 ] );
    }
    else
    {
        std::string cli_in;
        /* Read until a newline is found */
        std::getline( std::cin, cli_in );
        cmd.emplace_back( cli_in );
    }
    if( encode )
    {
        std::vector< bool > input;
        for( int i = 0; i <= int( cmd[ 3 ].size() ) - 1; ++i )
        {
            char val = cmd[ 3 ][ i ];
            if( ascii )
            {
                uint8_t mask = 0b10000000;
                for( int j = 1; j <= 8; ++j )
                {
                   input.emplace_back( ( ( val & mask ) ? 1 : 0 ) );
                   mask >>= 1;
                }
            }
            else
            {
                if( val != '0' && val != '1' )
                {
                    std::cout << "Non-binary character found in input" << "\n";
                    return -1;
                }
                input.emplace_back( val - 48 );
            }
        }
        try
        {
            std::string en_res = base64_encode( input );
            std::cout << en_res << "\n";
        }
        catch( std::runtime_error &err )
        {
            std::cout << "Exception occurred: " << err.what() << "\n";
            return -1;
        }
    }
    else
    {
        std::vector< bool > de_res;
        try
        {
            de_res = base64_decode( cmd[ 3 ] );
        }
        catch( std::runtime_error &err )
        {
            std::cout << "Exception occurred: " << err.what() << "\n";
            return -1;
        }
        if( !ascii )
        {
            for( int i = 0; i <= int( de_res.size() ) - 1; ++i )
            {
                std::cout << de_res[ i ];
            }
            std::cout << "\n";
            return 0;
        }
        std::string de_res_ascii;
        for( int i = 0; i <= int( de_res.size() ) - 1; i += 8 )
        {
            /* Take 8 bits at a time and convert to ascii representation */
            uint8_t group = 0;
            for( int j = i; j <= i + 7; ++j )
            {
                group <<= 1;
                group += de_res[ j ];
            }
            /* Make sure 'group' is valid i.e. printable on screen */
            if( !isprint( group ) )
            {
                std::cout << "Bits from " << i << " to " << i + 7 << " cannot "
                             "be printed as an ASCII character: use -b option "
                             "to get the binary output" << "\n";
                return -1;
            }
            de_res_ascii += char( group );
        }
        std::cout << de_res_ascii << "\n";
    }
    return 0;
}
