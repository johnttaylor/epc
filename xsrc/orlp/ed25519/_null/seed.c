#include "../ed25519.h"
#include <string.h>

/// Always return a seed of all zeros
int ed25519_create_seed( unsigned char *seed )
{
    memset( seed, 0, 32 );
    return 0;
}
