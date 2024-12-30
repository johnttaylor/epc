#include "Cpl/System/Api.h"
#include "Cpl/Io/File/Littlefs/_bdfile/driver.h"
#include "Cpl/Io/File/Littlefs/Api.h"

extern void run_readwrite_tests( Cpl::Io::File::Littlefs::Api::Volume_T& uut1, Cpl::Io::File::Littlefs::Api::Volume_T& uut2  );


#define CACHE_SIZE       16
#define READ_SIZE        CACHE_SIZE
#define PROG_SIZE        CACHE_SIZE
#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 128

// block driver
static lfs_filebd_t                   driverInstance1_;
static lfs_filebd_t                   driverInstance2_;
static const struct lfs_filebd_config bdCfg_ = {
    READ_SIZE,
    PROG_SIZE,
    ERASE_SIZE,
    NUM_ERASE_BLOCKS
};

int main( int argc, char* argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();
    Cpl::System::Api::enableScheduling();

    // Create my block driver
    int err = lfs_filebd_create( &driverInstance1_, "littlefs1.bin", &bdCfg_ );
    if ( err )
    {
        printf( "\nFailed to create the block driver#1: %d", err );
        return 1;
    }
    err = lfs_filebd_create( &driverInstance2_, "littlefs2.bin", &bdCfg_ );
    if ( err )
    {
        printf( "\nFailed to create the block driver#2: %d", err );
        return 1;
    }

    // Create my volumes
    Cpl::Io::File::Littlefs::Api::Volume_T fs1Cfg( &driverInstance1_,
                                                   lfs_filebd_read,
                                                   lfs_filebd_prog,
                                                   lfs_filebd_erase,
                                                   lfs_filebd_sync,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    Cpl::Io::File::Littlefs::Api::initVolume( fs1Cfg, "vola", true );

    Cpl::Io::File::Littlefs::Api::Volume_T fs2Cfg( &driverInstance2_,
                                                   lfs_filebd_read,
                                                   lfs_filebd_prog,
                                                   lfs_filebd_erase,
                                                   lfs_filebd_sync,
                                                   ERASE_SIZE,
                                                   NUM_ERASE_BLOCKS,
                                                   500 );
    Cpl::Io::File::Littlefs::Api::initVolume( fs2Cfg, "volb", true );

    run_readwrite_tests( fs1Cfg, fs2Cfg );
    printf( "\nReadWrite tests passed" );

    Cpl::Io::File::Littlefs::Api::shutdownVolume( fs1Cfg );
    Cpl::Io::File::Littlefs::Api::shutdownVolume( fs2Cfg );
    err = lfs_filebd_destroy( &driverInstance1_ );
    if ( err )
    {
        printf( "Failed to destroy the block driver#1: %d\n", err );
        return 1;
    }
    lfs_filebd_destroy( &driverInstance2_ );
    if ( err )
    {
        printf( "Failed to destroy the block driver#2: %d\n", err );
        return 1;
    }
}
