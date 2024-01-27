#include "../ed25519.h"
#include <windows.h>
#include <wincrypt.h>

int ed25519_create_seed(unsigned char *seed) 
{
    HCRYPTPROV prov;

    if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))  {
        return 1;
    }

    if (!CryptGenRandom(prov, 32, seed))  {
        CryptReleaseContext(prov, 0);
        return 1;
    }

    CryptReleaseContext(prov, 0);
    return 0;
}
