#include "Cpl/System/Api.h"
#include "Cpl/System/Trace.h"
#include "Cpl/Io/File/Littlefs/_bdfile/BdFile.h"
#include "Cpl/Io/Serial/PhonyStdio/InputOutput.h"

extern void runtest( lfs_t* cfg );

#define SECT_   "_0test"

#define CACHE_SIZE       16
#define READ_SIZE        CACHE_SIZE
#define PROG_SIZE        CACHE_SIZE
#define ERASE_SIZE       4096
#define NUM_ERASE_BLOCKS 4096


// Block driver instance
static Cpl::Io::File::Littlefs::BdFile driverInstance_( "littlefs.bin" );

// buffers
static uint8_t readBuffer[CACHE_SIZE];
static uint8_t progBuffer[CACHE_SIZE];
static uint8_t lookaheadBuffer[CACHE_SIZE];

// configuration of the filesystem is provided by this struct
static const struct lfs_config fsCfg = {
    &driverInstance_,  // filesystem context, aka the block driver

    // block device operations
    Cpl::Io::File::Littlefs::BdFile::callback_readfn,
    Cpl::Io::File::Littlefs::BdFile::callback_progfn,
    Cpl::Io::File::Littlefs::BdFile::callback_erasefn,
    Cpl::Io::File::Littlefs::BdFile::callback_syncfn,

    // block device configuration
    READ_SIZE,
    PROG_SIZE,
    ERASE_SIZE,
    NUM_ERASE_BLOCKS,
    500,  // Block cycles
    CACHE_SIZE,
    16,               // Lookahead buffer size
    0,                // compact_threshold
    readBuffer,       // read_buffer
    progBuffer,       // program_buffer
    lookaheadBuffer,  // lookahead_buffer
};

extern void runtest( const lfs_config& cfg );

Cpl::Io::Serial::PhonyStdio::InputOutput fd_;   // Bypass the CPL IO File abstractions for stdout

int main( int argc, char* argv[] )
{
    // Initialize Colony
    Cpl::System::Api::initialize();
    CPL_SYSTEM_TRACE_ENABLE();
    CPL_SYSTEM_TRACE_ENABLE_SECTION( SECT_ );
    //CPL_SYSTEM_TRACE_ENABLE_SECTION( "Cpl::Io::File::Littlefs::BdFile" );
    Cpl::System::Api::enableScheduling();

    driverInstance_.start();
    runtest( fsCfg );
    driverInstance_.stop();
}

Cpl::Io::Output* Cpl::System::Trace::getDefaultOutputStream_( void ) noexcept
{
    return &fd_;
}
