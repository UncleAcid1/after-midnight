/*
 * MyFileUtils.c — the parts of "MyFileUtilsMac.c" the game uses, in portable C.
 *
 * The original wrapped Carbon File Manager calls (FSRef, Navigation Services,
 * resource forks, creator/type codes). Navigation dialogs and resource-fork editing
 * were only used by the mansion editor and are not carried over.
 */
#include "MMGame.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

/// mkdir -p
static int MakeDirectories(const char *path)
{
    char temp[1024];
    char *p;
    size_t len;

    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);
    while (len > 1 && temp[len - 1] == '/')
        temp[--len] = '\0';
    for (p = temp + 1; *p != '\0'; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(temp, 0755) != 0 && errno != EEXIST)
                return errno;
            *p = '/';
        }
    }
    if (mkdir(temp, 0755) != 0 && errno != EEXIST)
        return errno;
    return 0;
}

/// Creates folderName inside parentPath (and parentPath itself, if needed).
int MMCreateDirectory(const char *parentPath, const char *folderName)
{
    char path[1024];
    if (DoesFileOrFolderExistInPath(parentPath, folderName))
        return 0;
    snprintf(path, sizeof(path), "%s/%s", parentPath, folderName);
    printf("Creating directory: %s\n", path);
    return MakeDirectories(path);
}

Boolean DoesFileOrFolderExistInPath(const char *parentPath, const char *name)
{
    char path[1024];
    struct stat st;
    snprintf(path, sizeof(path), "%s/%s", parentPath, name);
    return stat(path, &st) == 0;
}

Boolean DoesFileOrFolderExist(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0;
}

/// Mac creator/type codes have no equivalent elsewhere.
void SetFileCreatorAndTypeForPath(const char *path, Uint32 creator, Uint32 fileType)
{
    (void)path; (void)creator; (void)fileType;
}

/// Used only to pick defaults for slow machines; any modern CPU is fast enough.
int getProcessorSpeed(void)
{
    return 2000;
}
