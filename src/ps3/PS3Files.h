/*
 * PS3Files.h — the game's files on the PS3, with each mansion packed into one file.
 *
 * A mansion is a folder of small files, about 120,000 for all the mansions, and the PS3
 * installs a package of that many files very slowly. So the PS3 package holds each mansion
 * folder as one file, "Mansion.pack" (made by platform/ps3/pack_mansions.py), and the macros below
 * (seen by the game's files alone, through PS3Platform.h) open, list and look up the files
 * inside it as if they were still there. Everything else goes to the real files unchanged.
 */
#ifndef PS3FILES_H
#define PS3FILES_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct PS3Dir PS3Dir;

FILE          *PS3_fopen(const char *path, const char *mode);
SDL_RWops     *PS3_RWFromFile(const char *path, const char *mode);
int            PS3_stat(const char *path, struct stat *info);
PS3Dir        *PS3_opendir(const char *path);
struct dirent *PS3_readdir(PS3Dir *dir);
int            PS3_closedir(PS3Dir *dir);

#ifndef PS3_PLATFORM_IMPLEMENTATION
#define fopen               PS3_fopen
#define SDL_RWFromFile      PS3_RWFromFile
#define stat(path, info)    PS3_stat(path, info)
#define DIR                 PS3Dir
#define opendir             PS3_opendir
#define readdir             PS3_readdir
#define closedir            PS3_closedir
#endif

#ifdef __cplusplus
}
#endif

#endif
