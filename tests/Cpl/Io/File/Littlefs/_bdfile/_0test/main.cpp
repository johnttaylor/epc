#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/File/Littlefs/_bdfile/driver.h"

extern void runtest( lfs_t *cfg );

#define CACHE_SIZE 16
#define READ_SIZE CACHE_SIZE
#define PROG_SIZE CACHE_SIZE
#define ERASE_SIZE 4096
#define NUM_ERASE_BLOCKS 128

// block driver configuration
static const struct lfs_filebd_config bdCfg = {
    READ_SIZE,
    PROG_SIZE,
    ERASE_SIZE,
    NUM_ERASE_BLOCKS
};

// Block driver instance
static lfs_filebd_t driverInstance;

// buffers
static uint8_t readBuffer[CACHE_SIZE];
static uint8_t progBuffer[CACHE_SIZE];
static uint8_t lookaheadBuffer[CACHE_SIZE];

// configuration of the filesystem is provided by this struct
static const struct lfs_config fsCfg = {
    &driverInstance, // filesystem context, aka the block driver

    // block device operations
    lfs_filebd_read,
    lfs_filebd_prog,
    lfs_filebd_erase,
    lfs_filebd_sync,

    // block device configuration
    READ_SIZE,
    PROG_SIZE,
    ERASE_SIZE,
    NUM_ERASE_BLOCKS,
    500, // Block cycles
    CACHE_SIZE,
    16, // Lookahead buffer size
    0, // compact_threshold
    readBuffer, // read_buffer
    progBuffer, // program_buffer
    lookaheadBuffer, // lookahead_buffer
};

extern void runtest( const lfs_config& cfg );

int main( int argc, char *argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();
    Cpl::System::Api::enableScheduling();

    int err = lfs_filebd_create( &driverInstance, "littlefs.bin", &bdCfg );
    if ( err )
    {
        printf("Failed to create the block driver: %d\n", err );
        exit(1);
    }

    runtest( fsCfg );

    err = lfs_filebd_destroy( &driverInstance );
    if ( err )
    {
        printf("Failed to destroy the block driver: %d\n", err );
        exit(1);
    }

}
