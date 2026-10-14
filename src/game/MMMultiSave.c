/*
 * MMMultiSave.c — saved games for multiplayer, added by this version.
 *
 * A multiplayer game is saved for the group that played it, not for one profile: each
 * player has an ID (MMOnline.c), and the save goes in a folder named after the kind of game
 * and the IDs of everyone who played, sorted:
 *
 *     Multiplayer Saves/coop-ABCD-EFGH-JKLM+WXYZ-.../<save name>        the game (as any save)
 *     Multiplayer Saves/coop-ABCD-EFGH-JKLM+WXYZ-.../<save name>.txt    who played it
 *
 * So a save is only offered when exactly the same people are together again — the same
 * profiles on one computer, or the same two players on two computers (each computer keeps
 * its own copy) — and never mixes with anyone's own saved games. The .txt keeps each
 * player's name and character and whose turn it was, matched by ID when the game is
 * continued (so it doesn't matter who hosts this time or in which order the lobby lists
 * the players).
 */

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "MMGame.h"
#include "MMProtos.h"

static const char *ModeName(int mode)
{
    return mode == kMultiSaveArcade ? "arcade" : "coop";
}

static int CompareIDs(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

/// The group's folder (made if needed), or NULL when someone has no ID.
static const char *GroupFolder(int mode, char ids[][16], int count, Boolean create)
{
    static char folder[1400];
    char sorted[kMaxPlayers][16], key[200], base[1024];
    size_t n;
    int i;

    if (count < 2 || count > kMaxPlayers)
        return NULL;
    for (i = 0; i < count; i++) {
        if (strlen(ids[i]) != 14)
            return NULL;
        memcpy(sorted[i], ids[i], 16);
    }
    qsort(sorted, (size_t)count, sizeof(sorted[0]), CompareIDs);
    n = (size_t)snprintf(key, sizeof(key), "%s-", ModeName(mode));
    for (i = 0; i < count; i++)
        n += (size_t)snprintf(key + n, sizeof(key) - n, "%s%s", i ? "+" : "", sorted[i]);
    snprintf(base, sizeof(base), "%s", GetAppSupportPath(""));
    if (create) {
        MMCreateDirectory(base, "Multiplayer Saves");
        snprintf(folder, sizeof(folder), "%s/Multiplayer Saves", base);
        MMCreateDirectory(folder, key);
    }
    snprintf(folder, sizeof(folder), "%s/Multiplayer Saves/%s", base, key);
    return folder;
}

/// Who plays now (from the multiplayer setup): IDs, names, characters. Returns how many,
/// or 0 if a player has no ID (a game with an older version on the other computer).
static int CurrentGroup(char ids[][16], char names[][64], int *characters)
{
    int n = GetNumPlayers(), p;

    for (p = 0; p < n && p < kMaxPlayers; p++) {
        snprintf(names[p], 64, "%s", GetPlayerProfileName(p));
        characters[p] = GetPlayerCharacter(p);
        if (NetIsConnected() || NetGameActive())
            snprintf(ids[p], 16, "%s", NetPlayerID(p));
        else
            snprintf(ids[p], 16, "%s", OnlineProfileID(names[p]));
        if (strlen(ids[p]) != 14)
            return 0;
    }
    return n;
}

/// The folder this game's saves go in, or NULL if it can't have one.
const char *MultiSaveFolderForCurrentGame(int mode)
{
    char ids[kMaxPlayers][16], names[kMaxPlayers][64];
    int characters[kMaxPlayers], n = CurrentGroup(ids, names, characters);

    return n >= 2 ? GroupFolder(mode, ids, n, true) : NULL;
}

/// Saves the game being played for its group. Returns true if it was saved.
Boolean MultiSaveWrite(int mode, const char *saveName)
{
    char ids[kMaxPlayers][16], names[kMaxPlayers][64], mansion[255], path[1600];
    int characters[kMaxPlayers], n = CurrentGroup(ids, names, characters), p;
    const char *folder;
    FILE *file;

    if (n < 2 || (folder = GroupFolder(mode, ids, n, true)) == NULL)
        return false;
    snprintf(path, sizeof(path), "%s/%s", folder, saveName);
    if (MMCreateSaveGameFile(path) != 0)
        return false;
    snprintf(path, sizeof(path), "%s/%s.txt", folder, saveName);
    file = fopen(path, "w");
    if (file == NULL)
        return true;
    snprintf(mansion, sizeof(mansion), "%s", gCurOpenMansionFileName);
    StripStringFromEndOfCString(".zip", mansion, NULL);
    StripStringFromEndOfCString(".ZIP", mansion, NULL);
    fprintf(file, "# Midnight Mansion HD multiplayer save: who played\n");
    fprintf(file, "mode = %s\nmansion = %s\nsaved = %lu\nplayers = %d\n", ModeName(mode), mansion,
            (unsigned long)time(NULL), n);
    for (p = 0; p < n; p++)
        fprintf(file, "player = %s\t%d\t%ld\t%s\n", ids[p], characters[p], GetPlayerLoot(p), names[p]);
    fprintf(file, "turn = %s\n", ids[GetActivePlayer() >= 0 && GetActivePlayer() < n ? GetActivePlayer() : 0]);
    fclose(file);
    printf("Multiplayer game saved: %s/%s\n", folder, saveName);
    return true;
}

static Boolean ReadInfo(const char *folder, const char *name, MultiSaveInfo *info)
{
    char path[1600], line[400], *tab;
    FILE *file;
    struct stat st;

    memset(info, 0, sizeof(*info));
    snprintf(info->path, sizeof(info->path), "%s/%s", folder, name);
    snprintf(info->name, sizeof(info->name), "%s", name);
    if (stat(info->path, &st) != 0 || !S_ISREG(st.st_mode))
        return false;
    info->savedAt = (Uint32)st.st_mtime;
    snprintf(path, sizeof(path), "%s/%s.txt", folder, name);
    file = fopen(path, "r");
    if (file == NULL)
        return true;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        if (strncmp(line, "mansion = ", 10) == 0)
            snprintf(info->mansion, sizeof(info->mansion), "%s", line + 10);
        else if (strncmp(line, "saved = ", 8) == 0)
            info->savedAt = (Uint32)strtoul(line + 8, NULL, 10);
        else if (strncmp(line, "turn = ", 7) == 0)
            snprintf(info->turnID, sizeof(info->turnID), "%s", line + 7);
        else if (strncmp(line, "player = ", 9) == 0 && info->numPlayers < kMaxPlayers) {
            int p = info->numPlayers++;
            char *fields = line + 9;
            snprintf(info->ids[p], 16, "%.14s", fields);
            tab = strchr(fields, '\t');
            if (tab != NULL) {
                info->characters[p] = atoi(tab + 1);
                tab = strchr(tab + 1, '\t');
            }
            if (tab != NULL) {
                info->loot[p] = atol(tab + 1);
                tab = strchr(tab + 1, '\t');
            }
            if (tab != NULL)
                snprintf(info->names[p], sizeof(info->names[p]), "%s", tab + 1);
        }
    }
    fclose(file);
    return true;
}

static int CompareNewestFirst(const void *a, const void *b)
{
    const MultiSaveInfo *x = a, *y = b;
    return x->savedAt < y->savedAt ? 1 : x->savedAt > y->savedAt ? -1 : 0;
}

/// The saves of exactly this group (newest first).
int MultiSaveList(int mode, char ids[][16], int count, MultiSaveInfo *out, int max)
{
    const char *folderP = GroupFolder(mode, ids, count, false);
    char folder[1400];
    struct dirent *entry;
    size_t len;
    DIR *dir;
    int n = 0;

    if (folderP == NULL)
        return 0;
    snprintf(folder, sizeof(folder), "%s", folderP);
    dir = opendir(folder);
    if (dir == NULL)
        return 0;
    while ((entry = readdir(dir)) != NULL && n < max) {
        len = strlen(entry->d_name);
        if (entry->d_name[0] == '.' || (len > 4 && strcmp(entry->d_name + len - 4, ".txt") == 0))
            continue;
        if (len > 15 && entry->d_name[len - 15] == '.' && entry->d_name[len - 10] == '-' && entry->d_name[len - 5] == '-')
            continue;       // one player's part of an Arcade Split Screen save ("<save>.<ID>")
        if (ReadInfo(folder, entry->d_name, &out[n]))
            n++;
    }
    closedir(dir);
    qsort(out, (size_t)n, sizeof(MultiSaveInfo), CompareNewestFirst);
    return n;
}

/// The saves of whoever plays now (the multiplayer setup's players).
int MultiSaveListForCurrentGroup(int mode, MultiSaveInfo *out, int max)
{
    char ids[kMaxPlayers][16], names[kMaxPlayers][64];
    int characters[kMaxPlayers], n = CurrentGroup(ids, names, characters);

    return n >= 2 ? MultiSaveList(mode, ids, n, out, max) : 0;
}

/// A save's details, from its path (the game file; the .txt beside it says who played).
Boolean MultiSaveReadInfo(const char *path, MultiSaveInfo *info)
{
    char folder[1500];
    const char *slash = strrchr(path, '/');

    if (slash == NULL)
        return false;
    snprintf(folder, sizeof(folder), "%.*s", (int)(slash - path), path);
    return ReadInfo(folder, slash + 1, info);
}

/// Keeps a save the other computer sent (it's continuing it): returns where it went.
Boolean MultiSaveStoreReceived(int mode, const char *saveName, const Uint8 *data, int length, const Uint8 *sidecar,
                               int sidecarLength, char *pathOut, size_t size)
{
    char sidecarPath[1600];
    char ids[kMaxPlayers][16], names[kMaxPlayers][64];
    int characters[kMaxPlayers], n = CurrentGroup(ids, names, characters);
    const char *folder = n >= 2 ? GroupFolder(mode, ids, n, true) : NULL;
    FILE *file;

    if (folder == NULL)
        return false;
    snprintf(pathOut, size, "%s/%s", folder, saveName);
    file = fopen(pathOut, "wb");
    if (file == NULL)
        return false;
    fwrite(data, 1, (size_t)length, file);
    fclose(file);
    snprintf(sidecarPath, sizeof(sidecarPath), "%s.txt", pathOut);
    file = fopen(sidecarPath, "wb");
    if (file != NULL) {
        fwrite(sidecar, 1, (size_t)sidecarLength, file);
        fclose(file);
    }
    return true;
}

/// Keeps one player's part of an Arcade Split Screen save ("<save>.<ID>"), sent by the computer
/// they play on.
Boolean MultiSaveStorePart(int mode, const char *saveName, const char *playerID, const Uint8 *data, int length)
{
    char ids[kMaxPlayers][16], names[kMaxPlayers][64], path[1600];
    int characters[kMaxPlayers], n = CurrentGroup(ids, names, characters);
    const char *folder = n >= 2 ? GroupFolder(mode, ids, n, true) : NULL;
    FILE *file;

    if (folder == NULL || strlen(playerID) != 14 || strchr(saveName, '/') != NULL || strchr(playerID, '/') != NULL ||
        length <= 0)
        return false;
    snprintf(path, sizeof(path), "%s/%s.%s", folder, saveName, playerID);
    file = fopen(path, "wb");
    if (file == NULL)
        return false;
    fwrite(data, 1, (size_t)length, file);
    fclose(file);
    return true;
}

/// "Falcon Manor 3 — 2 hours ago".
void MultiSaveDescribe(const MultiSaveInfo *info, char *text, size_t size)
{
    time_t now = time(NULL);
    long ago = (long)(now - (time_t)info->savedAt);
    char when[64];

    if (ago < 120)
        snprintf(when, sizeof(when), "just now");
    else if (ago < 7200)
        snprintf(when, sizeof(when), "%ld minutes ago", ago / 60);
    else if (ago < 2 * 86400)
        snprintf(when, sizeof(when), "%ld hours ago", ago / 3600);
    else
        snprintf(when, sizeof(when), "%ld days ago", ago / 86400);
    snprintf(text, size, "%s%s%s", info->mansion[0] ? info->mansion : "", info->mansion[0] ? ", " : "", when);
}
