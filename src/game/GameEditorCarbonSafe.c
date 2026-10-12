// GameEditorCarbonSafe.c — level/room numbering helpers shared by the game and the editor.
//
// A "level" is one room of the mansion map. Level numbers are 1-based and laid out row-major on
// a grid `numCols` wide. Room resIDs start at `firstResID` (200) with `roomsPerLevel` (1) rooms
// per level.

#include "MMGame.h"
#include "MMProtos.h"

short GEGetLevelResID(short levelNumber, short roomsPerLevel, short firstResID)
{
    return (short)(levelNumber * roomsPerLevel + firstResID - roomsPerLevel);
}

short GEGetLevelNumber(short resID, short roomsPerLevel, short firstResID)
{
    return (short)((resID - firstResID) / roomsPerLevel + 1);
}

short GEGetRoomResID(short roomNumber, short levelResID)
{
    SDL_assert(roomNumber >= 0);
    if (roomNumber == 0)
        return 0;
    return (short)(roomNumber + levelResID - 1);
}

short GEGetRoomNumber(short roomResID, short levelResID)
{
    if (roomResID == 0)
        return 0;
    return (short)(roomResID - levelResID + 1);
}

short GEGetResIDFromLevelAndRoom(short levelNumber, short roomNumber, short roomsPerLevel, short firstResID)
{
    SDL_assert(roomNumber >= 1);
    SDL_assert(levelNumber >= 1);
    return (short)(GEGetLevelResID(levelNumber, roomsPerLevel, firstResID) + roomNumber - 1);
}

void GEGetRowAndColFromLevelNumber(short levelNumber, short *row, short *col, short numCols)
{
    *row = (short)((levelNumber - 1) / numCols);
    *col = (short)((levelNumber - 1) - *row * numCols);
}

void GEGetRowAndColFromResID(short resID, short *row, short *col, short numCols,
                             short roomsPerLevel, short firstResID)
{
    short level = (short)((resID - firstResID) / roomsPerLevel);
    *row = level / numCols;
    *col = level - *row * numCols;
}

short GEGetLevelNumberFromRowAndCol(short row, short col, short numCols)
{
    return (short)(row * numCols + col + 1);
}
