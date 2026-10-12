// GameArrays.c — persistent per-game state: collected objects, one-shot events, explored rooms,
// lit rooms, revealed map rooms and completed mansions; plus the save-game data block.

#include "MMGame.h"
#include "MMProtos.h"

#define kMaxObjects        30000
#define kMaxDoOnce         10000
#define kMaxRooms          10000
#define kMaxMansions       100
#define kNumDifficulties   3

#define kSaveDataVersion   0x70
#define kSaveDataEndMarker 0x7424

Boolean gArraysWereSaved;

long  gObjectArray[kMaxObjects];
long  gNextFreeObjectIndex;
long  gDoOnceArray[kMaxDoOnce];
long  gNextDoOnceIndex;
short gRoomArray[kMaxRooms];
short gNextRoomArrayIndex;
short gLitRoomArray[kMaxRooms];
short gNextLitRoomArrayIndex;
short gMapArray[kMaxRooms];
short gNextMapArrayIndex;
unsigned char gCompletedMansionsArray[kNumDifficulties * kMaxMansions];
short gNextCompletedMansionIndex[kNumDifficulties];

static long  gSaveObjectArray[kMaxObjects];
static long  gSaveNextFreeObjectIndex;
static long  gSaveDoOnceArray[kMaxDoOnce];
static long  gSaveNextDoOnceIndex;
static short gSaveRoomArray[kMaxRooms];
static short gSaveNextRoomArrayIndex;

// The save format stores these as 32-bit values; keep files compatible across 32/64-bit builds.
static void WriteLong(SDL_RWops *file, long value)
{
    Sint32 v = (Sint32)value;
    MyWriteDataToFile(file, 4, &v);
}

static long ReadLong(SDL_RWops *file)
{
    Sint32 v = 0;
    MyReadDataFromFile(file, 4, &v);
    return v;
}

static void WriteLongArray(SDL_RWops *file, const long *array, long count)
{
    long i;
    for (i = 0; i < count; i++)
        WriteLong(file, array[i]);
}

static void ReadLongArray(SDL_RWops *file, long *array, long count)
{
    long i;
    for (i = 0; i < count; i++)
        array[i] = ReadLong(file);
}

// ---------------------------------------------------------------------------
// Objects collected/destroyed (by objectID)

void InitObjectArray(void)
{
    int i;
    for (i = 0; i < kMaxObjects; i++)
        gObjectArray[i] = -1;
    gNextFreeObjectIndex = 0;
    gArraysWereSaved = false;
}

void AddSpriteToObjectArray(SpritePtr spriteP)
{
    AddObjectToArray(GAME_SPRITE(spriteP)->objectID);
}

void RemoveSpriteFromObjectArray(SpritePtr spriteP)
{
    RemoveObjectFromArray(GAME_SPRITE(spriteP)->objectID);
}

void AddObjectToArray(long objectID)
{
    if (!IsObjectInArray(objectID)) {
        gObjectArray[gNextFreeObjectIndex++] = objectID;
        SDL_assert(gNextFreeObjectIndex < kMaxObjects);
        if (gNextFreeObjectIndex >= kMaxObjects)
            gNextFreeObjectIndex = 0;
    }
}

void RemoveObjectFromArray(long objectID)
{
    long i, index;

    if (!IsObjectInArray(objectID))
        return;
    index = gNextFreeObjectIndex;
    for (i = 0; i < gNextFreeObjectIndex; i++) {
        if (gObjectArray[i] == objectID) {
            index = i;
            break;
        }
    }
    for (i = index; i < gNextFreeObjectIndex - 1; i++)
        gObjectArray[i] = gObjectArray[i + 1];
    gNextFreeObjectIndex--;
}

Boolean IsSpriteInObjectArray(SpritePtr spriteP)
{
    return IsObjectInArray(GAME_SPRITE(spriteP)->objectID);
}

Boolean IsObjectInArray(long objectID)
{
    long i;
    for (i = 0; i < gNextFreeObjectIndex; i++)
        if (gObjectArray[i] == objectID)
            return true;
    return false;
}

// ---------------------------------------------------------------------------
// One-shot events (by objectID)

void InitDoOnceArray(void)
{
    int i;
    for (i = 0; i < kMaxDoOnce; i++)
        gDoOnceArray[i] = -1;
    gNextDoOnceIndex = 0;
}

void AddSpriteToDoOnceArray(SpritePtr spriteP)
{
    AddObjectToDoOnceArray(GAME_SPRITE(spriteP)->objectID);
}

void RemoveSpriteFromDoOnceArray(SpritePtr spriteP)
{
    RemoveObjectFromDoOnceArray(GAME_SPRITE(spriteP)->objectID);
}

void AddObjectToDoOnceArray(long objectID)
{
    if (!IsObjectInDoOnceArray(objectID)) {
        gDoOnceArray[gNextDoOnceIndex++] = objectID;
        if (gNextDoOnceIndex >= kMaxDoOnce - 1)
            gNextDoOnceIndex = 0;
    }
}

void RemoveObjectFromDoOnceArray(long objectID)
{
    long i, index;

    if (!IsObjectInDoOnceArray(objectID))
        return;
    index = gNextDoOnceIndex;
    for (i = 0; i < gNextDoOnceIndex; i++) {
        if (gDoOnceArray[i] == objectID) {
            index = i;
            break;
        }
    }
    for (i = index; i < gNextDoOnceIndex - 1; i++)
        gDoOnceArray[i] = gDoOnceArray[i + 1];
    gNextDoOnceIndex--;
}

Boolean IsSpriteInDoOnceArray(SpritePtr spriteP)
{
    return IsObjectInDoOnceArray(GAME_SPRITE(spriteP)->objectID);
}

Boolean IsObjectInDoOnceArray(long objectID)
{
    long i;
    for (i = 0; i < gNextDoOnceIndex; i++)
        if (gDoOnceArray[i] == objectID)
            return true;
    return false;
}

// ---------------------------------------------------------------------------
// Rooms explored (by level number)

void InitRoomArray(void)
{
    int i;
    for (i = 0; i < kMaxRooms; i++)
        gRoomArray[i] = -1;
    gNextRoomArrayIndex = 0;
}

void AddRoomToArray(short room)
{
    if (!IsRoomInArray(room)) {
        gRoomArray[gNextRoomArrayIndex++] = room;
        if (gNextRoomArrayIndex >= kMaxRooms)
            gNextRoomArrayIndex = 0;
    }
}

Boolean IsRoomInArray(short room)
{
    int i;
    for (i = 0; i < gNextRoomArrayIndex; i++)
        if (gRoomArray[i] == room)
            return true;
    return false;
}

short GetNumRoomsExplored(void)
{
    return gNextRoomArrayIndex;
}

short GetRoomArrayElement(short index)
{
    SDL_assert(index < kMaxRooms);
    return gRoomArray[index];
}

// ---------------------------------------------------------------------------
// Rooms whose lights were switched on (by level number)

void InitLitRoomArray(void)
{
    int i;
    for (i = 0; i < kMaxRooms; i++)
        gLitRoomArray[i] = -1;
    gNextLitRoomArrayIndex = 0;
}

void AddRoomToLitRoomArray(short room)
{
    if (!IsRoomInLitRoomArray(room)) {
        gLitRoomArray[gNextLitRoomArrayIndex++] = room;
        if (gNextLitRoomArrayIndex >= kMaxRooms)
            gNextLitRoomArrayIndex = 0;
    }
}

Boolean IsRoomResIDInLitRoomArray(short roomResID)
{
    return IsRoomInLitRoomArray(GEGetLevelNumber(roomResID, 1, 200));
}

Boolean IsRoomInLitRoomArray(short room)
{
    int i;
    for (i = 0; i < gNextLitRoomArrayIndex; i++)
        if (gLitRoomArray[i] == room)
            return true;
    return false;
}

// ---------------------------------------------------------------------------
// Rooms revealed on the in-game map (by level number)

void InitMapArray(void)
{
    int i;
    for (i = 0; i < kMaxRooms; i++)
        gMapArray[i] = -1;
    gNextMapArrayIndex = 0;
}

void AddRoomToMapArray(short room)
{
    if (!IsRoomInMapArray(room)) {
        gMapArray[gNextMapArrayIndex++] = room;
        if (gNextMapArrayIndex >= kMaxRooms)
            gNextMapArrayIndex = 0;
    }
}

Boolean IsRoomInMapArray(short room)
{
    int i;
    for (i = 0; i < gNextMapArrayIndex; i++)
        if (gMapArray[i] == room)
            return true;
    return false;
}

short GetNumRoomsInMapArray(void)
{
    return gNextMapArrayIndex;
}

short GetMapArrayElement(short index)
{
    SDL_assert(index < kMaxRooms);
    return gMapArray[index];
}

// ---------------------------------------------------------------------------
// Completed mansions, per difficulty. Mansion number 0 (the tutorial) counts for every difficulty.

void InitCompletedMansionsArray(void)
{
    int d, i;
    for (d = 0; d < kNumDifficulties; d++) {
        for (i = 0; i < kMaxMansions; i++)
            gCompletedMansionsArray[d * kMaxMansions + i] = 0;
        gNextCompletedMansionIndex[d] = 0;
    }
}

void AddMansionToArray(short difficulty, short mansionNumber)
{
    int d;

    SDL_assert(difficulty >= 0);
    SDL_assert(difficulty <= 2);
    if (IsMansionInArray(difficulty, mansionNumber))
        return;

    if (mansionNumber == 0) {
        for (d = 0; d < kNumDifficulties; d++) {
            if (!IsMansionInArray(d, 0))
                gCompletedMansionsArray[d * kMaxMansions + gNextCompletedMansionIndex[d]++] = 0;
        }
    }
    else {
        gCompletedMansionsArray[difficulty * kMaxMansions + gNextCompletedMansionIndex[difficulty]++] =
            (unsigned char)mansionNumber;
        if (gNextCompletedMansionIndex[difficulty] >= kMaxMansions)
            gNextCompletedMansionIndex[difficulty] = 0;
    }
}

Boolean IsMansionInArray(short difficulty, short mansionNumber)
{
    int i;

    SDL_assert(difficulty >= 0);
    SDL_assert(difficulty <= 2);
    for (i = 0; i < gNextCompletedMansionIndex[difficulty]; i++)
        if (gCompletedMansionsArray[difficulty * kMaxMansions + i] == (unsigned short)mansionNumber)
            return true;
    return false;
}

void WriteMansionArrayToFile(SDL_RWops *file)
{
    MyWriteDataToFile(file, sizeof(gNextCompletedMansionIndex), gNextCompletedMansionIndex);
    MyWriteDataToFile(file, sizeof(gCompletedMansionsArray), gCompletedMansionsArray);
}

void ReadMansionArrayFromFile(SDL_RWops *file)
{
    MyReadDataFromFile(file, sizeof(gNextCompletedMansionIndex), gNextCompletedMansionIndex);
    MyReadDataFromFile(file, sizeof(gCompletedMansionsArray), gCompletedMansionsArray);
}

// ---------------------------------------------------------------------------
// Snapshot taken on entering a room so dying restores the room's objects.

void SaveAllGameArrays(void)
{
    memcpy(gSaveObjectArray, gObjectArray, sizeof(gObjectArray));
    gSaveNextFreeObjectIndex = gNextFreeObjectIndex;
    memcpy(gSaveDoOnceArray, gDoOnceArray, sizeof(gDoOnceArray));
    gSaveNextDoOnceIndex = gNextDoOnceIndex;
    memcpy(gSaveRoomArray, gRoomArray, sizeof(gRoomArray));
    gSaveNextRoomArrayIndex = gNextRoomArrayIndex;
}

void RestoreAllGameArrays(void)
{
    memcpy(gObjectArray, gSaveObjectArray, sizeof(gObjectArray));
    gNextFreeObjectIndex = gSaveNextFreeObjectIndex;
    memcpy(gDoOnceArray, gSaveDoOnceArray, sizeof(gDoOnceArray));
    gNextDoOnceIndex = gSaveNextDoOnceIndex;
    memcpy(gRoomArray, gSaveRoomArray, sizeof(gRoomArray));
    gNextRoomArrayIndex = gSaveNextRoomArrayIndex;
}

// ---------------------------------------------------------------------------
// Save-game data block (shared by save files, suspended games and movie files).
// Values are little-endian, as written by the original i386 build.

void WriteSaveDataToFile(SDL_RWops *file, float heroHoriz, float heroVert, Boolean isSaveFile)
{
    Sint32 size;
    Uint8 flag = isSaveFile;
    int i;

    WriteLong(file, kSaveDataVersion);
    WriteLong(file, gCurrentSaveIndex);

    size = gNextLitRoomArrayIndex * 2;
    MyWriteDataToFile(file, 4, &size);
    MyWriteDataToFile(file, 2, &gNextLitRoomArrayIndex);
    MyWriteDataToFile(file, size, gLitRoomArray);

    MyWriteDataToFile(file, 1, &flag);
    MyWriteDataToFile(file, 255, gMansionFilename);
    printf("Writing gMansionFilename to save file as: %s\n", gMansionFilename);
    WriteLong(file, gNumSecretsFound);
    MyWriteDataToFile(file, 2, &gCurrentMansionNumber);
    MyWriteDataToFile(file, 1, &gDidCheat);
    MyWriteDataToFile(file, 1, &gCurrentlyInMansion);
    MyWriteDataToFile(file, 2, &gCurrentMusicIndex);
    WriteLong(file, gTimeSpentInMansion);
    MyWriteDataToFile(file, 1, &gDifficultySetting);
    MyWriteDataToFile(file, 1, &gSilverFloorIsActive);
    MyWriteDataToFile(file, 1, &gGoldFloorIsActive);
    MyWriteDataToFile(file, 2, &gNumLivesLeft);
    MyWriteDataToFile(file, 4, &heroHoriz);
    MyWriteDataToFile(file, 4, &heroVert);
    MyWriteDataToFile(file, 2, &gCurrentLevel);
    for (i = 0; i < 7; i++)
        MyWriteDataToFile(file, 2, &gObjects.counts[i]);
    MyWriteDataToFile(file, 1, &gObjects.hasLantern);
    MyWriteDataToFile(file, 1, &gObjects.hasBackpack);
    WriteLong(file, gScore);
    WriteLong(file, gLastSavePedestalObjectID);

    WriteLong(file, gNextFreeObjectIndex * 4);
    WriteLong(file, gNextFreeObjectIndex);
    WriteLongArray(file, gObjectArray, gNextFreeObjectIndex);

    WriteLong(file, gNextDoOnceIndex * 4);
    WriteLong(file, gNextDoOnceIndex);
    WriteLongArray(file, gDoOnceArray, gNextDoOnceIndex);

    size = gNextRoomArrayIndex * 2;
    MyWriteDataToFile(file, 4, &size);
    MyWriteDataToFile(file, 2, &gNextRoomArrayIndex);
    MyWriteDataToFile(file, size, gRoomArray);

    size = gNextMapArrayIndex * 2;
    MyWriteDataToFile(file, 4, &size);
    MyWriteDataToFile(file, 2, &gNextMapArrayIndex);
    MyWriteDataToFile(file, size, gMapArray);

    WriteLong(file, kSaveDataEndMarker);
}

/// Returns 0 on success, -2 if the data is from a version too old to load.
int LoadSaveDataFromFile(SDL_RWops *file, float *heroHoriz, float *heroVert, Boolean *isSaveFile,
                         long *version)
{
    Sint32 size;
    long fileVersion, count;
    Uint8 flag;
    int i;

    fileVersion = ReadLong(file);
    if (fileVersion < kSaveDataVersion)
        return -2;

    gCurrentSaveIndex = ReadLong(file);
    InitLitRoomArray();
    MyReadDataFromFile(file, 4, &size);
    SDL_assert(size >= 0 && size <= (Sint32)sizeof(gLitRoomArray));
    MyReadDataFromFile(file, 2, &gNextLitRoomArrayIndex);
    MyReadDataFromFile(file, size, gLitRoomArray);

    MyReadDataFromFile(file, 1, &flag);
    MyReadDataFromFile(file, 255, gMansionFilename);
    gMansionFilename[254] = 0;
    gNumSecretsFound = ReadLong(file);
    MyReadDataFromFile(file, 2, &gCurrentMansionNumber);
    MyReadDataFromFile(file, 1, &gDidCheat);
    MyReadDataFromFile(file, 1, &gCurrentlyInMansion);
    MyReadDataFromFile(file, 2, &gCurrentMusicIndex);
    gTimeSpentInMansion = ReadLong(file);
    MyReadDataFromFile(file, 1, &gDifficultySetting);
    MyReadDataFromFile(file, 1, &gSilverFloorIsActive);
    MyReadDataFromFile(file, 1, &gGoldFloorIsActive);
    MyReadDataFromFile(file, 2, &gNumLivesLeft);
    MyReadDataFromFile(file, 4, heroHoriz);
    MyReadDataFromFile(file, 4, heroVert);
    MyReadDataFromFile(file, 2, &gCurrentLevel);
    for (i = 0; i < 7; i++)
        MyReadDataFromFile(file, 2, &gObjects.counts[i]);
    MyReadDataFromFile(file, 1, &gObjects.hasLantern);
    MyReadDataFromFile(file, 1, &gObjects.hasBackpack);
    gScore = ReadLong(file);
    gLastSavePedestalObjectID = ReadLong(file);
    gOldScore = gScore;

    InitObjectArray();
    InitDoOnceArray();
    InitRoomArray();
    InitMapArray();

    ReadLong(file);
    count = ReadLong(file);
    SDL_assert(count >= 0 && count <= kMaxObjects);
    if (count < 0 || count > kMaxObjects) count = 0;
    gNextFreeObjectIndex = count;
    ReadLongArray(file, gObjectArray, count);

    ReadLong(file);
    count = ReadLong(file);
    SDL_assert(count >= 0 && count <= kMaxDoOnce);
    if (count < 0 || count > kMaxDoOnce) count = 0;
    gNextDoOnceIndex = count;
    ReadLongArray(file, gDoOnceArray, count);

    MyReadDataFromFile(file, 4, &size);
    SDL_assert(size >= 0 && size <= (Sint32)sizeof(gRoomArray));
    MyReadDataFromFile(file, 2, &gNextRoomArrayIndex);
    MyReadDataFromFile(file, size, gRoomArray);

    MyReadDataFromFile(file, 4, &size);
    SDL_assert(size >= 0 && size <= (Sint32)sizeof(gMapArray));
    MyReadDataFromFile(file, 2, &gNextMapArrayIndex);
    MyReadDataFromFile(file, size, gMapArray);

    count = ReadLong(file);
    SDL_assert(count == kSaveDataEndMarker);

    *isSaveFile = flag;
    *version = fileVersion;
    return 0;
}

/// Split screen co-op: one mansion for the whole team.
void GameArraysShareState(void (*share)(void *, size_t))
{
    share(gObjectArray, sizeof(gObjectArray));
    share(&gNextFreeObjectIndex, sizeof(gNextFreeObjectIndex));
    share(gDoOnceArray, sizeof(gDoOnceArray));
    share(&gNextDoOnceIndex, sizeof(gNextDoOnceIndex));
    share(gRoomArray, sizeof(gRoomArray));
    share(&gNextRoomArrayIndex, sizeof(gNextRoomArrayIndex));
    share(gLitRoomArray, sizeof(gLitRoomArray));
    share(&gNextLitRoomArrayIndex, sizeof(gNextLitRoomArrayIndex));
    share(gMapArray, sizeof(gMapArray));
    share(&gNextMapArrayIndex, sizeof(gNextMapArrayIndex));
}
