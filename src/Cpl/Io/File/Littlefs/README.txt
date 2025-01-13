/** @namespace Cpl::Io::File::Littlefs

The 'Littlefs' namespace implements the CPL file abstractions using the open 
source 'littlefs' file system (https://github.com/littlefs-project/littlefs)

The 'littlefs' file system is currently support with the following caveats:

    o There is no concept of a 'file extension', the LFS_NAME_MAX is the limit 
      for individual names (sans any path info)
    
    o The canonicalPath() method is not supported

    o The getCwd() method is not supported

    o The directory traversal depth is limited to OPTION_CPL_IO_FILE_DIRLIST_MAX_DEPTH

    o Directory traversal is (relatively) memory expensive! In addition there 
      can only be one concurrent directory traversal at a time (mutex enforced).

    o Directory traversals do NOT span volumes 

    o Currently no file attributes are supported, e.g. no created, updated
      timestamps
    

The implementation of the CPL file abstraction SUPPORTS multiple littlefs 
instances where each instance has a unique block driver instance.  Or said
another way, you have a single logical file system with different 'volumes
mounted' for each physical persistent storage media (e.g. multiple SPI data
flash ICs)

The default behavior is for a single littlefs instance and single physical
persistent media.  To use multiple littlefs instances the Api::initVolume()
method needs to be called multiple times - once for each littlefs instance.
In addition, each call to Api::initVolume() must supply a 'volumeName' which
becomes the logical root directory for the particular directory. ALL files and
directory names MUST be prefix with a 'volumeName' (i.e. the volumeName identifies
which littlefs to use).  For example:

    Cpl::Io::File::Littlefs::Api::initVolume( volume1, "A", true );
    Cpl::Io::File::Littlefs::Api::initVolume( volume2, "B", true );
    ...
    Cpl::Io::File::Output fd1( "/A/myfile", ... );  // Opened on volume1
    Cpl::Io::File::Output fd2( "/B/myfile", ... );  // Opened on volume2
    Cpl::Io::File::Api::createDirectory( "/A/mydirectory" );    // Created on volume1


*/  



  