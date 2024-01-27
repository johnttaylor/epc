#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sha512.h"
#include "../ed25519.h"
#include "docopt.h"
#include "docopt_value.h"

static const char USAGE[] =
R"(Firmware Image Signer.

    Usage:
      signer [options] sign <binfile> <secret>
      signer [options] verify <binfile> 

    Arguments:
      <binfile>     Binary input file to sign OR to verify
      <secret>      Secret used to generate the signing key pair.  Must be 
                    64 character HEXASCII string (i.e. 32 byte secret)

    Options:
      -o OUTBIN     File name of the signed binary image [Default: out.bin]
      -h,--help     Show this screen.

    Examples:
        ; Sign an image
        signer.exe sign ajax.bin efab5b0739a834bac702aeb5cd08ffe227908faaae501f910e7e07d8d41fbb06

        ; Verify the signed image from the above command
        signer.exe verify out.bin

)";



static int hex2bin( unsigned char *obuf, const char *ibuf, int len );
static unsigned char *ihex2bin_buf( unsigned int *start_address, int *dst_len, FILE *inFile );
static int bin2ihex_buf( unsigned char *src, int src_len, unsigned int start_address, FILE *outFile );
static int calc_checksum( unsigned char *inbuf, int len );

static std::map<std::string, docopt::value> g_args;

static size_t getFileLength( FILE* fd )
{
    if ( fseek( fd, 0, SEEK_END ) != 0 )
    {
        return 0;
    }
    size_t len = ftell( fd );
    bool reset_fileptr = fseek( fd, 0, SEEK_SET ) == 0;
    return reset_fileptr ? len : 0;
}

///////////////////////////////////////////////
int main( int argc, char **argv )
{
    // Parse the command line arguments
    g_args                     = docopt::docopt( USAGE, { argv + 1, argv + argc } );

    bool        verify_infile  = g_args["verify"].asBool();
    const char* inhex_fn       = g_args["<binfile>"].asString().c_str();
    const char* outbin_fn      = g_args["-o"].asString().c_str();
    const char* secret         = g_args["<secret>"].isString() ? g_args["<secret>"].asString().c_str() : nullptr;

    int            i, c;
    FILE*          inFile;
    FILE*          outFile;
    unsigned int   target_start_address;
    int            target_len;
    unsigned char* target_bin = nullptr;
    unsigned char  hash_buf[64];
    unsigned char  public_key[32], private_key[64], signature[64];


    unsigned char* ed25519_secret = nullptr;
    if ( secret )
    {
        ed25519_secret = (unsigned char*) malloc( strlen( secret ) );
        if ( ed25519_secret == nullptr )
        {
            fprintf( stderr, "Failed to allocate memory to store 'secret' as binary.\n" );
            return 0;
        }

        c = hex2bin( ed25519_secret, secret, strlen( secret ) );
        if ( c != 32 )
        {
            fprintf( stderr, "ED25519 'secret' have to be 32bytes long.\n" );
            return 0;
        }
        ed25519_create_keypair( public_key, private_key, ed25519_secret );
    }


    inFile       = fopen( inhex_fn, "rb" );
    size_t flen  = getFileLength( inFile );
    if ( flen == 0 )
    {
        fprintf( stderr, "Error reading length of file: %s.\n", inhex_fn );
        return 0;
    }

    size_t public_key_len = sizeof( public_key );
    size_t signature_len  = sizeof( signature );
    size_t total_size     = flen + public_key_len + signature_len;
    target_bin            = (unsigned char*) malloc( total_size );
    target_len            = fread( target_bin, 1, flen, inFile );
    fclose( inFile );

    if ( target_bin && (target_len > 0) )
    {

        // Sign the image
        if ( ed25519_secret )
        {
            // Append the public key to the 'raw image'         
            memmove( target_bin + target_len, public_key, public_key_len );

            // Sign the image (which now includes the public key)
            ed25519_sign( signature, target_bin, target_len + public_key_len, public_key, private_key );

            // Append the signature to the image
            memmove( target_bin + target_len + public_key_len, signature, signature_len );

            // Generate a SHA512 checksum for the BIN output file
            if ( sha512( target_bin, +target_len + public_key_len + signature_len, hash_buf ) != 0 )
            {
                fprintf( stderr, "The SHA512 on the final BIN image failed.\n" );
                return 0;
            }
        }

        // Verify the image
        if ( verify_infile )
        {
            unsigned char *public_key_verif, *signatureverif;

            public_key_verif = target_bin + target_len - (signature_len + public_key_len);
            signatureverif   = target_bin + target_len - signature_len;


            printf( "Signing PublicKey: " );
            for ( c=0; c < 32; c++ )
            {
                printf( "%02x", (unsigned char) public_key_verif[c] );
            }
            printf( "\n" );

            printf( "Target Signature:  " );
            for ( c=0; c < 64; c++ )
            {
                printf( "%02x", (unsigned char) signatureverif[c] );
            }
            printf( "\n" );


            c = ed25519_verify( signatureverif, target_bin, target_len - sizeof( signature ), public_key_verif );
            if ( c )
            {
                printf( "Signature:         OK\n" );
            }
            else
            {
                printf( "Signature:         FAILED\n" );
            }

            return 0;
        }

        // Generate the BINARY output file
        outFile = fopen( outbin_fn, "wb" );
        c = fwrite( target_bin, total_size, 1, outFile );
        fclose( outFile );
        if ( c != 1 )
        {
            printf( "error: write to output bin file\n" );
            return 1;
        }

        printf( "Original Image Length:     %lu B\n", target_len );
        printf( "New Image Length:          %lu B\n", target_len + public_key_len + signature_len );

        printf( "SHA512 of the output file: " );
        for ( c=0; c < 64; c++ )
        {
            printf( "%02x", (unsigned char) hash_buf[c] );
        }
        printf( "\n" );

        printf( "Signing PublicKey:         " );
        for ( c=0; c < 32; c++ )
        {
            printf( "%02x", (unsigned char) public_key[c] );
        }
        printf( "\n" );

        printf( "Target Signature:          " );
        for ( c=0; c < 64; c++ )
        {
            printf( "%02x", (unsigned char) signature[c] );
        }
        printf( "\n" );


    }
    else
    {
        printf( "error: processing input file\n" );
        return 1;
    }

    return 0;
}


int hex2bin( unsigned char *obuf, const char *ibuf, int len )
{
    unsigned char c, c2;

    len = len / 2;
    while ( *ibuf != 0 ) {
        c = *ibuf++;
        if ( c >= '0' && c <= '9' )
            c -= '0';
        else if ( c >= 'a' && c <= 'f' )
            c -= 'a' - 10;
        else if ( c >= 'A' && c <= 'F' )
            c -= 'A' - 10;
        else
            return -1;

        c2 = *ibuf++;
        if ( c2 >= '0' && c2 <= '9' )
            c2 -= '0';
        else if ( c2 >= 'a' && c2 <= 'f' )
            c2 -= 'a' - 10;
        else if ( c2 >= 'A' && c2 <= 'F' )
            c2 -= 'A' - 10;
        else
            return -1;

        *obuf++ = (c << 4) | c2;
    }
    return len;
}


int check_checksum( unsigned char *inbuf, int len )
{
    unsigned int check = 0;
    while ( len-- ) {
        check += *inbuf++;
    }
    return check & 0xFF;
}

// more details: http://en.wikipedia.org/wiki/Intel_HEX
unsigned char *ihex2bin_buf( unsigned int *start_address, int *dst_len, FILE *inFile )
{
    unsigned int  lines = 0, total = 0, oneline_len, elar = 0, pos, cnt;
    unsigned char oneline[512], raw[256], start_set = 0, *dst = NULL;

    *dst_len = 1024 * 1024 * 8;
    dst = (unsigned char*) malloc( *dst_len );                                                         //allocate 1mB of memory for bin data buffer
    if ( dst == NULL ) {
        *dst_len = -2;
        return NULL;
    }

    *start_address = 0;
    while ( fgets( (char*) oneline, sizeof( oneline ), inFile ) != NULL ) {
        if ( oneline[0] == ':' ) {                                                      //is valid record?
            oneline_len = strlen( (const char*) oneline ) - 2;                                            //get line length
            hex2bin( raw, (const char*)(oneline + 1), oneline_len );                                       //convert to bin
            if ( check_checksum( raw, oneline_len / 2 ) == 0 ) {                              //check cheksum validity
                if ( (raw[0] == 2) && (raw[1] == 0) && (raw[2] == 0) && (raw[3] == 4) ) {   //> Extended Linear Address Record  :020000040803EF
                    elar = (unsigned int) raw[4] << 24 | (unsigned int) raw[5] << 16;              //gen new address offset
                }
                else
                    if ( (raw[0] == 0) && (raw[1] == 0) && (raw[2] == 0) && (raw[3] == 1) ) {   //>End Of File record   :00000001FF
                        *dst_len = total;                                                       //return total size of bin data && start address
                        return dst;
                    }
                    else
                        if ( (raw[0] == 0) && (raw[1] == 0) && (raw[2] == 0) && (raw[3] == 0x10) ) {  //Encedo Special Record - Flash Initialization 
                            //do nothing here
                        }
                        else
                            if ( raw[3] == 0 ) {                                                        //>Data record - process
                                pos = elar + ((unsigned int) raw[1] << 8 | (unsigned int) raw[2]);          //get start address of this chunk
                                if ( start_set == 0 ) {
                                    *start_address = pos;                                                     //set it as new start addres - only possible for first data record
                                    start_set = 1;                                                             //only once - this is start address of thye binary data
                                }
                                pos -= *start_address;
                                cnt = raw[0];                                                                //get chunk size/length
                                if ( pos + cnt > *dst_len ) {                                                    //enlarge buffer if required
                                    unsigned char *dst_new = (unsigned char*) realloc( dst, *dst_len + 8192 );                //add 8kB of new space
                                    if ( dst_new == NULL ) {
                                        *dst_len = -2;                                                         //allocation error - exit
                                        free( dst );
                                        return NULL;
                                    }
                                    else {
                                        *dst_len += 8192;
                                        dst = dst_new;                                                         //allocation succesed - copy new pointer
                                    }
                                }
                                memmove( dst + pos, raw + 4, cnt );
                                if ( pos + cnt > total ) {                                                       //set new total variable
                                    total = pos + cnt;                                                           //tricky way - file can be none linear!
                                }
                            }
            }
            else {
                *dst_len = -1;                                                             //checksum error - exit
                return NULL;
            }
        }
        lines++;                                                                       //not a IntelHex line - comment?
    }
    *dst_len = -3;                                                                   //fatal error - no valid intel hex file processed
    free( dst );
    return NULL;
}


int bin2ihex( char *obuf, const unsigned char *ibuf, int len )
{
    int ret = len / 2;

    *obuf = ':';
    while ( len-- ) {
        sprintf( obuf + strlen( obuf ), "%02X", *ibuf++ );
    }
    sprintf( obuf + strlen( obuf ), "\r\n" );

    return ret;
}


int calc_checksum( unsigned char *inbuf, int len )
{
    unsigned char check = 0;
    while ( len-- ) {
        check += *inbuf++;
    }
    return 0 - check;
}


int bin2ihex_buf( unsigned char *src, int src_len, unsigned int start_address, FILE *outFile )
{
    int loop64k, rest64k, inner64k, ptr, c, segment, chunk;
    unsigned char buf[64];
    char sbuf[128];


    if ( start_address % 64 * 1024 ) {
        return -1;
    }
    segment = start_address >> 16;

    loop64k = src_len / (64 * 1024);
    rest64k = src_len % 64 * 1024;

    sprintf( sbuf, ":00000010F0\r\n" );
    c = fwrite( sbuf, strlen( sbuf ), 1, outFile );

    ptr = 0;
    while ( loop64k-- ) {
        buf[0] = 0x02;
        buf[1] = buf[2] = 0;
        buf[3] = 0x04;
        buf[4] = segment >> 8;
        buf[5] = segment & 0xff;
        buf[6] = calc_checksum( buf, 6 );
        memset( sbuf, 0, sizeof( sbuf ) );
        c = bin2ihex( sbuf, buf, 7 );
        c = fwrite( sbuf, strlen( sbuf ), 1, outFile );

        chunk = 16;
        for ( inner64k=0; inner64k < (64 * 1024) / chunk; inner64k++ ) {
            buf[0] = chunk;
            buf[1] = (inner64k * chunk) >> 8;
            buf[2] = (inner64k * chunk) & 0xff;
            buf[3] = 0;
            memmove( buf + 4, src + ptr, chunk );
            buf[4 + chunk] = calc_checksum( buf, chunk + 4 );
            memset( sbuf, 0, sizeof( sbuf ) );
            c = bin2ihex( sbuf, buf, chunk + 5 );
            c = fwrite( sbuf, strlen( sbuf ), 1, outFile );
            ptr += chunk;
        }
        segment++;
    }

    if ( rest64k ) {
        buf[0] = 0x02;
        buf[1] = buf[2] = 0;
        buf[3] = 0x04;
        buf[4] = segment >> 8;
        buf[5] = segment & 0xff;
        buf[6] = calc_checksum( buf, 6 );
        c = bin2ihex( sbuf, buf, 7 );
        c = fwrite( sbuf, strlen( sbuf ), 1, outFile );

        chunk = 16;
        inner64k = (64 * 1024) / chunk;
        for ( inner64k=0; (64 * 1024) / chunk; inner64k++ ) {
            buf[0] = chunk;
            buf[1] = (inner64k * chunk) >> 8;
            buf[2] = (inner64k * chunk) & 0xff;
            buf[3] = 0;
            memmove( buf + 4, src + ptr, chunk );
            buf[4 + chunk] = calc_checksum( buf, chunk + 4 );
            memset( sbuf, 0, sizeof( sbuf ) );
            c = bin2ihex( sbuf, buf, chunk + 5 );
            c = fwrite( sbuf, strlen( sbuf ), 1, outFile );
            ptr += chunk;
        }
    }

    //TBD :040000050042EA4982

    sprintf( sbuf, ":00000001FF\r\n" );
    c = fwrite( sbuf, strlen( sbuf ), 1, outFile );

    return 1;
}
