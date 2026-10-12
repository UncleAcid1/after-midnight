/*
 * rngs.c — multi-stream Lehmer random number generator (Park & Miller,
 * "Random Number Generators: Good Ones Are Hard To Find", with Park & Geyer's
 * stream extension), as used by Midnight Mansion. The game calls Random() under
 * the name CustomRandom().
 */
#include <stdio.h>
#include <time.h>
#include "rngs.h"

#define MODULUS     2147483647L     // 2^31 - 1
#define MULTIPLIER  48271           // 0xbc8f
#define CHECK       399268537L
#define STREAMS     256
#define A256        22925           // jump multiplier between streams (0x598d)
#define DEFAULT     123456789L      // 0x75bcd15

static long seed[STREAMS] = { DEFAULT };
static int  stream = 0;
static int  initialized = 0;

double CustomRandom(void)
{
    const long Q = MODULUS / MULTIPLIER;
    const long R = MODULUS % MULTIPLIER;
    long t;

    t = MULTIPLIER * (seed[stream] % Q) - R * (seed[stream] / Q);
    if (t > 0)
        seed[stream] = t;
    else
        seed[stream] = t + MODULUS;
    return (double)seed[stream] / MODULUS;
}

void PlantSeeds(long x)
{
    const long Q = MODULUS / A256;
    const long R = MODULUS % A256;
    int j, s;
    long t;

    initialized = 1;
    s = stream;
    SelectStream(0);
    PutSeed(x);
    stream = s;
    for (j = 1; j < STREAMS; j++) {
        t = A256 * (seed[j - 1] % Q) - R * (seed[j - 1] / Q);
        if (t > 0)
            seed[j] = t;
        else
            seed[j] = t + MODULUS;
    }
}

/// x > 0 sets the seed (mod 2^31-1), x < 0 seeds from the clock. The original
/// prompted on stdin for x == 0; that case uses the clock too.
void PutSeed(long x)
{
    if (x > 0)
        x = x % MODULUS;
    if (x <= 0)
        x = ((unsigned long)time(NULL)) % MODULUS;
    if (x == 0)
        x = DEFAULT;
    seed[stream] = x;
}

void GetSeed(long *x)
{
    *x = seed[stream];
}

void SelectStream(int index)
{
    stream = ((unsigned int)index) % STREAMS;
    if (initialized == 0 && stream != 0)
        PlantSeeds(DEFAULT);
}

void TestRandom(void)
{
    long i, x;
    int ok;

    SelectStream(0);
    PutSeed(1);
    for (i = 0; i < 10000; i++)
        CustomRandom();
    GetSeed(&x);
    ok = (x == CHECK);
    SelectStream(1);
    PlantSeeds(1);
    GetSeed(&x);
    ok = ok && (x == A256);
    if (!ok)
        puts("\n\a ERROR -- the implementation of rngs.c is not correct.\n");
    else
        puts("\n The implementation of rngs.c is correct.\n");
}
