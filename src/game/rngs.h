#ifndef RNGS_H
#define RNGS_H

double CustomRandom(void);
void   PlantSeeds(long x);
void   PutSeed(long x);
void   GetSeed(long *x);
void   SelectStream(int index);
void   TestRandom(void);

#endif
