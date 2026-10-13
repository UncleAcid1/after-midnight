/*
 * PS3Files.c — the files inside the mansions' "Mansion.pack" files (see PS3Files.h).
 *
 * A path is looked up in a pack when one of the folders on its way holds a "Mansion.pack": the
 * rest of the path is the file's name inside it. Which folders hold one is learned as paths
 * come (one check per folder, remembered). A pack's list of files is read the first time it's
 * needed and kept; a file opened from it is read whole into memory (a room is a few KB).
 */
#define PS3_PLATFORM_IMPLEMENTATION
#include "PS3Platform.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ppu-types.h>

#define kPackName       "Mansion.pack"
#define kFolderSlots    8192            // folders checked for a pack (a power of 2)

typedef struct {
    char       *path;                   // the pack file
    u32         count;
    u8         *index;                  // count entries of 12 bytes, then the names
    const char *names;
} Pack;

typedef struct {
    char       *folder;                 // without the slash at the end
    Pack       *pack;                   // or NULL: no pack in it
} FolderSlot;

struct PS3Dir {
    DIR            *real;
    Pack           *pack;
    char           *prefix;             // inside the pack: "" or "folder/"
    u32             next;               // next entry of the pack to look at
    char            last[MAXPATHLEN + 1];
    struct dirent   entry;
};

static FolderSlot   gFolders[kFolderSlots];
static int          gFolderCount;
static SDL_mutex   *gLock;
static FILE        *gOpenPackFile;      // the pack read last, kept open
static Pack        *gOpenPack;

static void Lock(void)
{
    if (gLock == NULL)
        gLock = SDL_CreateMutex();
    if (gLock != NULL)
        SDL_LockMutex(gLock);
}

static void Unlock(void)
{
    if (gLock != NULL)
        SDL_UnlockMutex(gLock);
}

static u32 ReadU32(const u8 *p)
{
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static u32 HashOf(const char *s, size_t len)
{
    u32 h = 2166136261u;
    while (len-- > 0)
        h = (h ^ (u8)*s++) * 16777619u;
    return h;
}

static Pack *LoadPack(const char *packPath)
{
    FILE *file = fopen(packPath, "rb");
    u8 header[16];
    Pack *pack = NULL;
    u32 count, namesSize;

    if (file == NULL)
        return NULL;
    if (fread(header, 1, 16, file) == 16 && memcmp(header, "AMPK", 4) == 0 && ReadU32(header + 4) == 1) {
        count = ReadU32(header + 8);
        namesSize = ReadU32(header + 12);
        pack = calloc(1, sizeof(Pack));
        if (pack != NULL) {
            pack->index = malloc((size_t)count * 12 + namesSize + 1);
            pack->path = strdup(packPath);
            if (pack->index == NULL || pack->path == NULL ||
                fread(pack->index, 1, (size_t)count * 12 + namesSize, file) != (size_t)count * 12 + namesSize) {
                free(pack->index);
                free(pack->path);
                free(pack);
                pack = NULL;
            } else {
                pack->count = count;
                pack->names = (const char *)pack->index + (size_t)count * 12;
                pack->index[(size_t)count * 12 + namesSize] = 0;
            }
        }
    }
    if (pack == NULL)
        printf("PS3Files: %s isn't a pack\n", packPath);
    fclose(file);
    return pack;
}

/// The pack in a folder (len characters of path), or NULL. Remembered for each folder.
static Pack *PackInFolder(const char *path, size_t len)
{
    u32 slot = HashOf(path, len) & (kFolderSlots - 1);
    char packPath[MAXPATHLEN + 16];
    struct stat info;
    Pack *pack = NULL;

    while (gFolders[slot].folder != NULL) {
        if (strncmp(gFolders[slot].folder, path, len) == 0 && gFolders[slot].folder[len] == 0)
            return gFolders[slot].pack;
        slot = (slot + 1) & (kFolderSlots - 1);
    }
    if (len + sizeof(kPackName) + 1 > sizeof(packPath))
        return NULL;
    memcpy(packPath, path, len);
    packPath[len] = '/';
    memcpy(packPath + len + 1, kPackName, sizeof(kPackName));
    if (stat(packPath, &info) == 0 && S_ISREG(info.st_mode))
        pack = LoadPack(packPath);
    if (gFolderCount < kFolderSlots / 2) {      // past half full, stop remembering
        gFolders[slot].folder = malloc(len + 1);
        if (gFolders[slot].folder != NULL) {
            memcpy(gFolders[slot].folder, path, len);
            gFolders[slot].folder[len] = 0;
            gFolders[slot].pack = pack;
            gFolderCount++;
        }
    }
    return pack;
}

/// The pack a path is in, and the path inside it (in name, "" for the pack's folder itself).
/// For a file, only the folders above it can hold its pack (orFolder 0).
static Pack *FindPack(const char *path, int orFolder, char *name, size_t nameSize)
{
    char clean[MAXPATHLEN + 1];
    size_t len = 0;
    const char *p;
    long i;

    if (path == NULL)
        return NULL;
    for (p = path; *p != 0 && len < sizeof(clean) - 1; p++) {   // "a//b/" is "a/b"
        if (*p == '/' && len > 0 && clean[len - 1] == '/')
            continue;
        clean[len++] = *p;
    }
    if (*p != 0)
        return NULL;
    while (len > 1 && clean[len - 1] == '/')
        len--;
    clean[len] = 0;
        // the folders on the way, from the top: the first holding a pack has it
    for (i = 1; i <= (long)len; i++) {
        if ((i == (long)len && orFolder) || clean[i] == '/') {
            Pack *pack = PackInFolder(clean, (size_t)i);
            if (pack != NULL) {
                const char *rest = clean + i + (i < (long)len ? 1 : 0);
                if (strlen(rest) + 1 > nameSize)
                    return NULL;
                strcpy(name, rest);
                return pack;
            }
        }
    }
    return NULL;
}

static const char *EntryName(const Pack *pack, u32 i)
{
    return pack->names + ReadU32(pack->index + (size_t)i * 12);
}

/// The entry with this name, or -1.
static long FindEntry(const Pack *pack, const char *name)
{
    long lo = 0, hi = (long)pack->count - 1;

    while (lo <= hi) {
        long mid = (lo + hi) / 2;
        int c = strcmp(EntryName(pack, (u32)mid), name);
        if (c == 0)
            return mid;
        if (c < 0)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}

/// Is name a folder inside the pack ("" is the pack's own folder)?
static int IsPackFolder(const Pack *pack, const char *name)
{
    size_t len = strlen(name);
    long lo = 0, hi = (long)pack->count - 1, first = (long)pack->count;

    if (len == 0)
        return 1;
    while (lo <= hi) {                  // the first entry not before "name/"
        long mid = (lo + hi) / 2;
        const char *e = EntryName(pack, (u32)mid);
        int c = strncmp(e, name, len);
        if (c == 0)
            c = (unsigned char)e[len] < (unsigned char)'/' ? -1 : 0;
        if (c < 0) {
            lo = mid + 1;
        } else {
            first = mid;
            hi = mid - 1;
        }
    }
    if (first >= (long)pack->count)
        return 0;
    {
        const char *e = EntryName(pack, (u32)first);
        return strncmp(e, name, len) == 0 && e[len] == '/';
    }
}

/// Reads an entry whole; NULL when it can't.
static u8 *ReadEntry(Pack *pack, long i, u32 *size)
{
    const u8 *entry = pack->index + (size_t)i * 12;
    u32 offset = ReadU32(entry + 4);
    u8 *data;

    *size = ReadU32(entry + 8);
    if (gOpenPack != pack) {
        if (gOpenPackFile != NULL)
            fclose(gOpenPackFile);
        gOpenPackFile = fopen(pack->path, "rb");
        gOpenPack = gOpenPackFile != NULL ? pack : NULL;
        if (gOpenPackFile == NULL)
            return NULL;
    }
    data = malloc(*size + 1);
    if (data == NULL)
        return NULL;
    if (fseek(gOpenPackFile, (long)offset, SEEK_SET) != 0 || fread(data, 1, *size, gOpenPackFile) != *size) {
        free(data);
        return NULL;
    }
    data[*size] = 0;
    return data;
}

static int IsReading(const char *mode)
{
    return mode != NULL && mode[0] == 'r' && strchr(mode, '+') == NULL;
}

/// Looks a path up for reading: 1 found (data filled in), 0 not in a pack, -1 in a pack's
/// folder but not there.
static int ReadFromPack(const char *path, u8 **data, u32 *size)
{
    char name[MAXPATHLEN + 1];
    Pack *pack;
    long i;
    int result;

    Lock();
    pack = FindPack(path, 0, name, sizeof(name));
    if (pack == NULL) {
        result = 0;
    } else if ((i = FindEntry(pack, name)) < 0) {
        result = -1;
    } else {
        *data = ReadEntry(pack, i, size);
        result = *data != NULL ? 1 : -1;
    }
    Unlock();
    return result;
}

// ---------------------------------------------------------------------------
// FILE * and SDL_RWops over a file read into memory

typedef struct {
    u8     *data;
    u32     size, pos;
} MemFile;

static int MemRead(void *cookie, char *buf, int n)
{
    MemFile *m = cookie;
    u32 left = m->size - m->pos;

    if (n < 0)
        return -1;
    if ((u32)n > left)
        n = (int)left;
    memcpy(buf, m->data + m->pos, (size_t)n);
    m->pos += (u32)n;
    return n;
}

static fpos_t MemSeek(void *cookie, fpos_t offset, int whence)
{
    MemFile *m = cookie;
    long long to = whence == SEEK_SET ? (long long)offset :
                   whence == SEEK_CUR ? (long long)m->pos + offset : (long long)m->size + offset;

    if (to < 0 || to > (long long)m->size) {
        errno = EINVAL;
        return -1;
    }
    m->pos = (u32)to;
    return (fpos_t)to;
}

static int MemClose(void *cookie)
{
    MemFile *m = cookie;
    free(m->data);
    free(m);
    return 0;
}

FILE *PS3_fopen(const char *path, const char *mode)
{
    u8 *data;
    u32 size;
    MemFile *m;
    FILE *file;

    if (!IsReading(mode))
        return fopen(path, mode);
    switch (ReadFromPack(path, &data, &size)) {
    case 0:
        return fopen(path, mode);
    case -1:
        errno = ENOENT;
        return NULL;
    }
    m = malloc(sizeof(MemFile));
    if (m == NULL) {
        free(data);
        return NULL;
    }
    m->data = data;
    m->size = size;
    m->pos = 0;
    file = funopen(m, MemRead, NULL, MemSeek, MemClose);
    if (file == NULL)
        MemClose(m);
    return file;
}

static Sint64 RWSize(SDL_RWops *rw)
{
    return ((MemFile *)rw->hidden.unknown.data1)->size;
}

static Sint64 RWSeek(SDL_RWops *rw, Sint64 offset, int whence)
{
    MemFile *m = rw->hidden.unknown.data1;
    Sint64 to = whence == RW_SEEK_SET ? offset : whence == RW_SEEK_CUR ? (Sint64)m->pos + offset :
                (Sint64)m->size + offset;

    if (to < 0 || to > (Sint64)m->size)
        return SDL_SetError("seek out of the file");
    m->pos = (u32)to;
    return to;
}

static size_t RWRead(SDL_RWops *rw, void *ptr, size_t size, size_t num)
{
    MemFile *m = rw->hidden.unknown.data1;
    size_t n;

    if (size == 0)
        return 0;
    n = (m->size - m->pos) / size;
    if (n > num)
        n = num;
    memcpy(ptr, m->data + m->pos, n * size);
    m->pos += (u32)(n * size);
    return n;
}

static size_t RWWrite(SDL_RWops *rw, const void *ptr, size_t size, size_t num)
{
    (void)rw; (void)ptr; (void)size; (void)num;
    SDL_SetError("the file can't be written");
    return 0;
}

static int RWClose(SDL_RWops *rw)
{
    if (rw != NULL) {
        MemClose(rw->hidden.unknown.data1);
        SDL_FreeRW(rw);
    }
    return 0;
}

SDL_RWops *PS3_RWFromFile(const char *path, const char *mode)
{
    u8 *data;
    u32 size;
    MemFile *m;
    SDL_RWops *rw;

    if (!IsReading(mode))
        return SDL_RWFromFile(path, mode);
    switch (ReadFromPack(path, &data, &size)) {
    case 0:
        return SDL_RWFromFile(path, mode);
    case -1:
        SDL_SetError("Couldn't open %s", path);
        return NULL;
    }
    m = malloc(sizeof(MemFile));
    rw = SDL_AllocRW();
    if (m == NULL || rw == NULL) {
        free(m);
        free(data);
        if (rw != NULL)
            SDL_FreeRW(rw);
        return NULL;
    }
    m->data = data;
    m->size = size;
    m->pos = 0;
    rw->size = RWSize;
    rw->seek = RWSeek;
    rw->read = RWRead;
    rw->write = RWWrite;
    rw->close = RWClose;
    rw->type = SDL_RWOPS_UNKNOWN;
    rw->hidden.unknown.data1 = m;
    return rw;
}

// ---------------------------------------------------------------------------
// stat and folders

int PS3_stat(const char *path, struct stat *info)
{
    char name[MAXPATHLEN + 1];
    Pack *pack;
    long i;
    int result = -2;

    Lock();
    pack = FindPack(path, 1, name, sizeof(name));
    if (pack != NULL && name[0] != 0) {             // the pack's own folder is a real one
        memset(info, 0, sizeof(*info));
        if ((i = FindEntry(pack, name)) >= 0) {
            info->st_mode = S_IFREG | 0444;
            info->st_size = (off_t)ReadU32(pack->index + (size_t)i * 12 + 8);
            result = 0;
        } else if (IsPackFolder(pack, name)) {
            info->st_mode = S_IFDIR | 0555;
            result = 0;
        } else {
            result = -1;
        }
    }
    Unlock();
    if (result == -2)
        return stat(path, info);
    if (result == -1)
        errno = ENOENT;
    return result;
}

PS3Dir *PS3_opendir(const char *path)
{
    char name[MAXPATHLEN + 1];
    PS3Dir *dir = calloc(1, sizeof(PS3Dir));
    Pack *pack;

    if (dir == NULL)
        return NULL;
    Lock();
    pack = FindPack(path, 1, name, sizeof(name));
    if (pack != NULL && IsPackFolder(pack, name)) {
        size_t len = strlen(name);
        dir->prefix = malloc(len + 2);
        if (dir->prefix != NULL) {
            memcpy(dir->prefix, name, len);
            if (len > 0)
                dir->prefix[len++] = '/';
            dir->prefix[len] = 0;
            dir->pack = pack;
        }
    }
    Unlock();
    if (dir->pack == NULL || dir->prefix[0] == 0)   // the pack's folder also has real files
        dir->real = opendir(path);
    if (dir->real == NULL && dir->pack == NULL) {
        free(dir);
        return NULL;
    }
    return dir;
}

struct dirent *PS3_readdir(PS3Dir *dir)
{
    struct dirent *real;
    size_t len;

    while (dir->real != NULL && (real = readdir(dir->real)) != NULL) {
        if (dir->pack == NULL || strcmp(real->d_name, kPackName) != 0)
            return real;
    }
    if (dir->pack == NULL)
        return NULL;
    len = strlen(dir->prefix);
    Lock();
    while (dir->next < dir->pack->count) {
        const char *e = EntryName(dir->pack, dir->next++);
        const char *slash;
        size_t n;
        if (strncmp(e, dir->prefix, len) != 0)
            continue;
        e += len;
        slash = strchr(e, '/');
        n = slash != NULL ? (size_t)(slash - e) : strlen(e);
        if (n == 0 || n >= sizeof(dir->last) || n >= sizeof(dir->entry.d_name))
            continue;
        if (strncmp(dir->last, e, n) == 0 && dir->last[n] == 0)
            continue;                               // a folder already listed
        memcpy(dir->last, e, n);
        dir->last[n] = 0;
        memset(&dir->entry, 0, sizeof(dir->entry));
        memcpy(dir->entry.d_name, e, n);
        dir->entry.d_namlen = (uint16_t)n;
        dir->entry.d_type = slash != NULL ? DT_DIR : DT_REG;
        Unlock();
        return &dir->entry;
    }
    Unlock();
    return NULL;
}

int PS3_closedir(PS3Dir *dir)
{
    if (dir == NULL)
        return -1;
    if (dir->real != NULL)
        closedir(dir->real);
    free(dir->prefix);
    free(dir);
    return 0;
}
