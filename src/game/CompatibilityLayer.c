/*
 * CompatibilityLayer.c — paths, resource files and text helpers.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/Compatibility Layer.c").
 * Classic Mac resources live as files: <mansion folder>'TYPE'/<resID, 5 digits>.
 * CoreFoundation bundle lookups are replaced by SDL_GetBasePath, and the
 * SDL_RWFromZZIP reader (which also looked inside zip files) by MMOpenResourceFile.
 */
#include "MMGame.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#ifndef _WIN32
#include <pwd.h>
#endif

#define kMaxValidResIDs         5000
#define kNumCachedBackgrounds   10

SpritePtr gCurBackgroundSprite = NULL;
static SpritePtr gBackgroundSpriteCache[kNumCachedBackgrounds];
static Boolean gBackgroundSpriteCacheWasInited;
static char gDirPath[1024];
static char gImageFilePath[1024];
static char gMansionDataPath[1024];
static char gMansionPath[1024];
static char gReturnedFilename[1024];
static char gSavedGameFilePath[1024];
static char gScannedResType[32];
static short gLastValidLevelResIDIndex;
static short gValidLevelResIDs[kMaxValidResIDs];

// ---------------------------------------------------------------------------
// Text

void MyDrawText(const char *text, MMTextStyleRec *style, short h, short v, SWRGBColor *color)
{
    SWRect r;
    r.left = h;
    r.top = v;
    r.right = h + 200;
    r.bottom = v;
    MyDrawCTextJustified(text, style, &r, color);
}

SpritePtr MyDrawPascalTextJustified(ConstStringPtr text, MMTextStyleRec *style, SWRect *rectP, SWRGBColor *color)
{
    char cText[608];
    SWCopyPascalStringToCString(text, cText);
    return MyDrawCTextJustified(cText, style, rectP, color);
}

/// Adds a text sprite for one line of text in rectP to gTextSpriteLayerP, shrinking
/// the font until the text fits.
SpritePtr MyDrawCTextJustified(const char *text, MMTextStyleRec *style, SWRect *rectP, SWRGBColor *color)
{
    SpritePtr spriteP = NULL;
    char fontName[256];
    unsigned char rgb[3];
    short size, width;

    SWCopyPascalStringToCString(style->fontName, fontName);
    size = style->size;
    rgb[0] = (unsigned char)(color->red >> 8);
    rgb[1] = (unsigned char)(color->green >> 8);
    rgb[2] = (unsigned char)(color->blue >> 8);

    do {
        width = (short)GetTextSpriteRenderWidth(fontName, size, text);
        if (width > rectP->right - rectP->left)
            size--;
    } while (width > rectP->right - rectP->left && size > 1);

    if (SWCreateTextSprite(&spriteP, text, fontName, size, style->style, rgb, style->dropShadow) == kNoError) {
        SetTextSpriteRect(spriteP, rectP, style->justification, false);
        SWSetSpriteLocation(spriteP, rectP->left, rectP->bottom - size);
        SWAddSprite(gTextSpriteLayerP, spriteP);
    }
    return spriteP;
}

/**
 * Word-wraps text ('\r' separates lines, "\r\r" adds a blank line) into rectP.
 * With renderToSprite the lines are rendered once and captured as a single sprite;
 * otherwise each line becomes a text sprite. Either way they go into layerP.
 */
static Boolean gTextBoxMeasureOnly = false;     // MeasureTextBox: lay out, draw nothing
static short gTextBoxBottom;                    // and where the last line ended

SpritePtr MyDrawTextBox(SpriteWorldPtr spriteWorldP, const char *text, MMTextStyleRec *style, SWRect *rectP,
                        SWRGBColor *color, SpriteLayerPtr layerP, Boolean renderToSprite)
{
    Boolean paragraphBreak[100];
    char *lines[100];
    char lineText[400], testText[400], fontName[256], buffer[2400];
    unsigned char rgb[3];
    SpriteLayerPtr tempLayerP = NULL;
    SpritePtr resultSpriteP = NULL, lineSpriteP;
    SWRect r, captureRect;
    short numLines, lineIndex, boxWidth, textWidth, lineSpacing;
    char *p, *word;
    Boolean done, lineDone;
    SWError err;

    if (gTextBoxMeasureOnly) {
        renderToSprite = false;
    } else if (!renderToSprite) {
        style->style = 0;
    } else {
        SW_ASSERT(spriteWorldP != NULL);
        err = SWCreateSpriteLayer(&tempLayerP);
        if (err == kNoError)
            SWAddSpriteLayer(spriteWorldP, tempLayerP);
        else
            renderToSprite = false;
    }

    SWCopyCString(text, buffer);
    SWCopyPascalStringToCString(style->fontName, fontName);
    rgb[0] = (unsigned char)(color->red >> 8);
    rgb[1] = (unsigned char)(color->green >> 8);
    rgb[2] = (unsigned char)(color->blue >> 8);
    r = *rectP;
    SWOffsetRect(&r, 0, style->size);
    boxWidth = r.right - r.left;

        // note which line breaks are followed by an empty line
    numLines = 0;
    p = buffer;
    while ((p = strchr(p, '\r')) != NULL && numLines < 100) {
        p++;
        if (*p == '\r') {
            paragraphBreak[numLines++] = true;
            p++;
        } else {
            paragraphBreak[numLines++] = false;
        }
    }

    numLines = 0;
    p = strtok(buffer, "\r");
    while (p != NULL && numLines < 100) {
        lines[numLines++] = p;
        p = strtok(NULL, "\r");
    }
    if (numLines == 0) {
        lines[0] = buffer;
        numLines = 1;
    }

    lineIndex = 0;
    word = strtok(lines[0], " ");
    done = false;
    while (!done) {
        lineSpacing = style->size + 2;
        lineText[0] = '\0';
        if (word != NULL)
            snprintf(lineText, sizeof(lineText), "%s", word);
        lineDone = false;
        while (!lineDone) {
            word = strtok(NULL, " ");
            while (word == NULL) {
                lineIndex++;
                if (lineIndex >= numLines) {
                    done = true;
                    goto drawLine;
                }
                word = strtok(lines[lineIndex], " ");
                if (paragraphBreak[lineIndex - 1])
                    lineSpacing += style->size + 2;
                lineDone = true;
                if (word != NULL)
                    goto drawLine;
            }
            snprintf(testText, sizeof(testText), "%s %s", lineText, word);
            textWidth = (short)GetTextSpriteRenderWidth(fontName, style->size, testText);
            if (textWidth < boxWidth)
                strcpy(lineText, testText);
            else
                lineDone = true;
        }
drawLine:
        if (lineText[0] == '\0')
            break;
        if (gTextBoxMeasureOnly) {
            gTextBoxBottom = (short)(r.top + style->size / 3);
            SWOffsetRect(&r, 0, lineSpacing);
            continue;
        }
        err = SWCreateTextSprite(&lineSpriteP, lineText, fontName, style->size, style->style, rgb,
                                 style->dropShadow);
        if (err == kNoError) {
            SetTextSpriteRect(lineSpriteP, &r, style->justification, true);
            SWSetSpriteLocation(lineSpriteP, r.left, r.top);
            SWOffsetRect(&r, 0, lineSpacing);
            SWAddSprite(renderToSprite ? tempLayerP : layerP, lineSpriteP);
        }
    }

    if (renderToSprite) {
        MMRenderSceneForWorkAreaSprite(spriteWorldP);
            // The original scaled the rect to window pixels here (SWAutoScaleRect);
            // this engine captures work areas in design coordinates.
        captureRect = *rectP;
        err = SWCreateSpriteFromWorkArea(spriteWorldP, &resultSpriteP, NULL, &captureRect);
        if (err == kNoError) {
            SWDisposeAllSpritesInLayer(tempLayerP);
            SWAddSprite(layerP, resultSpriteP);
        }
        SWRemoveSpriteLayer(spriteWorldP, tempLayerP);
        SWDisposeSpriteLayer(&tempLayerP);
    }
    return resultSpriteP;
}

/// How tall MyDrawTextBox's text would be in rectP's width (rectP's top to the last line's
/// bottom, in the same units).
short MeasureTextBox(const char *text, const MMTextStyleRec *style, const SWRect *rectP)
{
    MMTextStyleRec copy = *style;
    SWRect rect = *rectP;
    SWRGBColor color = { 0, 0, 0 };

    gTextBoxMeasureOnly = true;
    gTextBoxBottom = rect.top;
    MyDrawTextBox(NULL, text, &copy, &rect, &color, NULL, false);
    gTextBoxMeasureOnly = false;
    return (short)(gTextBoxBottom - rectP->top);
}

/// Draws the world without the cursor, fade and top layers, at zoom 1, so that it
/// can be captured with SWCreateSpriteFromWorkArea.
void MMRenderSceneForWorkAreaSprite(SpriteWorldPtr spriteWorldP)
{
    float zoomH = spriteWorldP->zoomH;
    float zoomV = spriteWorldP->zoomV;

    SWSetSpriteWorldZoom(gSpriteWorldP, 1.0f, 1.0f);
    SWSetSpriteLayerVisible(gCustomCursorSpriteLayerP, false);
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, false);
    SWSetSpriteLayerVisible(gTopSpriteLayerP, false);
    SWUpdateSpriteWorld(spriteWorldP, false);
    SWSetSpriteLayerVisible(gFadeToBlackSpriteLayerP, true);
    SWSetSpriteLayerVisible(gCustomCursorSpriteLayerP, true);
    SWSetSpriteLayerVisible(gTopSpriteLayerP, true);
    SWSetSpriteWorldZoom(gSpriteWorldP, zoomH, zoomV);
}

// ---------------------------------------------------------------------------
// Sprite loading

/// Shows a full-screen background image. cacheIndex 0..9 keeps the image loaded
/// (a clone is shown); -1 loads it uncached.
SWError LoadAndInstallBackgroundSprite(SpritePtr *spritePP, const char *folderPath, const char *fileName,
                                       short cacheIndex, Boolean hasAlpha)
{
    SpritePtr spriteP;
    SWError err = kNoError;
    char path[1024];
    short i;

    snprintf(path, sizeof(path), "%s", AddFilenameToPath(folderPath, fileName));
    if (!gBackgroundSpriteCacheWasInited) {
        for (i = 0; i < kNumCachedBackgrounds; i++)
            gBackgroundSpriteCache[i] = NULL;
        gBackgroundSpriteCacheWasInited = true;
    }

    if (cacheIndex == -1 || cacheIndex >= kNumCachedBackgrounds || gBackgroundSpriteCache[cacheIndex] == NULL) {
        err = SWCreateSpriteFromFile(&gCurBackgroundSprite, NULL, path, 1, hasAlpha);
        if (err == kNoError && cacheIndex != -1 && cacheIndex < kNumCachedBackgrounds)
            gBackgroundSpriteCache[cacheIndex] = gCurBackgroundSprite;
    } else {
        gCurBackgroundSprite = gBackgroundSpriteCache[cacheIndex];
    }

    if (err == kNoError) {
        RemoveAllSpritesInLayer(gBackgroundScreenSpriteLayerP);
        if (cacheIndex == -1)
            spriteP = gCurBackgroundSprite;
        else
            SWCloneSprite(gCurBackgroundSprite, &spriteP, NULL, 0);
        SWAddSprite(gBackgroundScreenSpriteLayerP, spriteP);
        SWSetSpriteLocation(spriteP, 0, 0);
        if (cacheIndex != -1)
            SWSetSpriteScaledSize(spriteP, 640, 432);   // room backgrounds
        if (spritePP != NULL)
            *spritePP = spriteP;
    }
    return err;
}

SWError LoadSpriteFromScreensFolder(SpritePtr *spritePP, const char *fileName)
{
    char path[1024];
    snprintf(path, sizeof(path), "%s", AddFilenameToPath(GetScreensFolderPath(), fileName));
    return SWCreateSpriteFromFile(spritePP, NULL, path, 1, true);
}

/* For the PICT helpers: the PICT's own mask (alpha) is used only when the sprite
   comes from a single PICT (firstResID == maskResID) and useMask is set. */

SWError CreateSpriteFromSinglePict(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                   short pictResID, short maskResID, short frameWidth, short borderWidth,
                                   MaskType maskType)
{
    char path[1024];
    (void)spriteWorldP;
    snprintf(path, sizeof(path), "%s", GetFileNameForResource(GetSpriteImagePath(NULL), pictResID));
    return SWCreateSpriteFromSingleFile(spritePP, storageP, path, frameWidth, borderWidth,
                                        pictResID == maskResID && maskType != kNoMask);
}

SWError CreateSpriteFromPictResource(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                     short pictResID, short maskResID, short maxFrames, MaskType maskType)
{
    char path[1024];
    Boolean selfMask = pictResID == maskResID && maskType != kNoMask;
    (void)spriteWorldP;

    if (maxFrames == 1) {
        snprintf(path, sizeof(path), "%s", GetFileNameForResource(GetSpriteImagePath(NULL), pictResID));
        return SWCreateSpriteFromFile(spritePP, storageP, path, 1, selfMask);
    }
    snprintf(path, sizeof(path), "%s%s.png", GetSpriteImagePath(NULL), "%05d");
    return SWCreateSpriteFromFileSequence(spritePP, storageP, path, pictResID, maxFrames, selfMask);
}

SWError CreateSpriteFromSinglePictXY(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                     short pictResID, short maskResID, short frameWidth, short frameHeight,
                                     short borderWidth, short borderHeight, Boolean hasOutsideBorder,
                                     short maxFrames, MaskType maskType)
{
    char path[1024];
    (void)spriteWorldP;
    snprintf(path, sizeof(path), "%s", GetFileNameForResource(GetSpriteImagePath(NULL), pictResID));
    return SWCreateSpriteFromSingleFileXY(spritePP, storageP, path, frameWidth, frameHeight, borderWidth,
                                          borderHeight, hasOutsideBorder, maxFrames,
                                          pictResID == maskResID && maskType != kNoMask);
}

SWError MMCreateSpriteFromSingleFileXY(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                       const char *fileName, short frameWidth, short frameHeight,
                                       short maxFrames, MaskType maskType)
{
    char path[1024];
    (void)spriteWorldP;
    snprintf(path, sizeof(path), "%s", AddFilenameToPath(GetScreensFolderPath(), fileName));
    return SWCreateSpriteFromSingleFileXY(spritePP, storageP, path, frameWidth, frameHeight, 0, 0, false,
                                          maxFrames, maskType != kNoMask);
}

SWError CreateSpriteFromVerticalPict(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                     short pictResID, short maskResID, short maxFrames, MaskType maskType)
{
    char path[1024];
    (void)spriteWorldP;
    snprintf(path, sizeof(path), "%s", GetFileNameForResource(GetSpriteImagePath(NULL), pictResID));
    return SWCreateSpriteFromVerticalFile(spritePP, storageP, path, 0, maxFrames,
                                          pictResID == maskResID && maskType != kNoMask);
}

SWError MMCreateSpriteFromVerticalFile(SpriteWorldPtr spriteWorldP, SpritePtr *spritePP, void *storageP,
                                       const char *fileName, short maxFrames, MaskType maskType)
{
    char path[1024];
    (void)spriteWorldP;
    snprintf(path, sizeof(path), "%s", AddFilenameToPath(GetScreensFolderPath(), fileName));
    return SWCreateSpriteFromVerticalFile(spritePP, storageP, path, 0, maxFrames, maskType != kNoMask);
}

// ---------------------------------------------------------------------------
// Paths. Like the original, these return a shared buffer: copy the result
// before calling another path function.

const char *GetFileNameForResource(const char *folderPath, int resID)
{
    snprintf(gReturnedFilename, sizeof(gReturnedFilename), "%s%05d.png", folderPath, resID);
    return gReturnedFilename;
}

const char *AddFilenameToPath(const char *folderPath, const char *fileName)
{
    char temp[1024];
    snprintf(temp, sizeof(temp), "%s%s", folderPath, fileName);
    snprintf(gReturnedFilename, sizeof(gReturnedFilename), "%s", temp);
    return gReturnedFilename;
}

static const char *GameDataSubfolder(const char *format, const char *name)
{
    char dataPath[1024];
    snprintf(dataPath, sizeof(dataPath), "%s", GetGameDataPath());
    if (name == NULL || name[0] == '\0')
        snprintf(gImageFilePath, sizeof(gImageFilePath), format, dataPath, "");
    else
        snprintf(gImageFilePath, sizeof(gImageFilePath), format, dataPath, name);
    return gImageFilePath;
}

const char *GetSpriteImagePath(const char *fileName)      { return GameDataSubfolder("%s/Sprites/%s", fileName); }
const char *GetScreensFolderPath(void)                    { return GameDataSubfolder("%s/Screens/%s", NULL); }
const char *GetTitleScreenFolderPath(const char *fileName){ return GameDataSubfolder("%s/Screens/titleScreen/%s", fileName); }
const char *GetMapScreenFolderPath(const char *fileName)  { return GameDataSubfolder("%s/Screens/mapScreen/%s", fileName); }
const char *GetOrganScreenFolderPath(const char *fileName){ return GameDataSubfolder("%s/Screens/organScreen/%s", fileName); }
const char *GetSoundsFolderPath(void)                     { return GameDataSubfolder("%s/Sounds/%s", NULL); }
const char *GetMusicFolderPath(void)                      { return GameDataSubfolder("%s/Music/%s", NULL); }
const char *GetFontsFolderPath(const char *fileName)      { return GameDataSubfolder("%s/Fonts/%s", fileName); }

const char *GetMansionFolderPath(const char *mansionName)
{
    char dataPath[1024];
    snprintf(dataPath, sizeof(dataPath), "%s", GetGameDataPath());
    if (mansionName == NULL || mansionName[0] == '\0')
        snprintf(gMansionPath, sizeof(gMansionPath), "%s/Mansions/", dataPath);
    else
        snprintf(gMansionPath, sizeof(gMansionPath), "%s/Mansions/%s/", dataPath, mansionName);
    return gMansionPath;
}

const char *GetMansionDataFolderPath(const char *fileName)
{
    char dataPath[1024];
    snprintf(dataPath, sizeof(dataPath), "%s", GetGameDataPath());
    if (fileName == NULL || fileName[0] == '\0')
        snprintf(gMansionDataPath, sizeof(gMansionDataPath), "%s/Mansion Data/", dataPath);
    else
        snprintf(gMansionDataPath, sizeof(gMansionDataPath), "%s/Mansion Data/%s", dataPath, fileName);
    return gMansionDataPath;
}

const char *GetCustomMansionsFolderPath(const char *mansionName)
{
    char gameDir[1024];
    snprintf(gameDir, sizeof(gameDir), "%s", GetGameDirectory());
    snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/%s/", gameDir, "Midnight Mansion HD Custom Mansions",
             mansionName != NULL ? mansionName : "");
    printf("Looking for custom mansions in: %s \n", gImageFilePath);
    return gImageFilePath;
}

const char *GetPrefsFilePath(const char *fileName)
{
    return GetAppSupportPath(fileName);
}

const char *GetRealPrefsFilePath(const char *fileName)
{
    const char *home = GetHomeDirectory();
    if (home == NULL) {
        char dataPath[1024];
        snprintf(dataPath, sizeof(dataPath), "%s", GetGameDataPath());
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s", dataPath, fileName);
    } else {
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/Library/Preferences/%s", home, fileName);
    }
    return gImageFilePath;
}

/// Folder for prefs, high scores and saved games.
const char *GetAppSupportPath(const char *fileName)
{
    const char *separator = (fileName != NULL && fileName[0] != '\0') ? "/" : "";
    if (fileName == NULL)
        fileName = "";
#ifdef __APPLE__
    {
        const char *home = GetHomeDirectory();
        if (home != NULL) {
            snprintf(gImageFilePath, sizeof(gImageFilePath),
                     "%s/Library/Application Support/ActionSoft/Midnight Mansion%s%s", home, separator, fileName);
            return gImageFilePath;
        }
    }
#else
    {
        char *prefPath = SDL_GetPrefPath("ActionSoft", "Midnight Mansion");
        if (prefPath != NULL) {
            size_t len = strlen(prefPath);
            if (len > 0 && (prefPath[len - 1] == '/' || prefPath[len - 1] == '\\'))
                prefPath[len - 1] = '\0';
            snprintf(gImageFilePath, sizeof(gImageFilePath), "%s%s%s", prefPath, separator, fileName);
            SDL_free(prefPath);
            return gImageFilePath;
        }
    }
#endif
    {
        char dataPath[1024];
        snprintf(dataPath, sizeof(dataPath), "%s", GetGameDataPath());
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s", dataPath, fileName);
    }
    return gImageFilePath;
}

/// The folder that holds the "ActionSoft" app support folder.
const char *GetPlainAppSupportPath(void)
{
#ifdef __APPLE__
    const char *home = GetHomeDirectory();
    if (home != NULL) {
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/Library/Application Support", home);
        return gImageFilePath;
    }
    return GetGameDataPath();
#else
    char path[1024];
    snprintf(path, sizeof(path), "%s", GetAppSupportPath(""));
    StripNDirectoriesFromEndOfPath(path, 2);    // .../ActionSoft/Midnight Mansion
    snprintf(gImageFilePath, sizeof(gImageFilePath), "%s", path);
    return gImageFilePath;
#endif
}

/// Folder (or file, with fileName) under app support/<subfolder>/<player name, if any>/.
static const char *PlayerSubfolderPath(const char *subfolder, const char *fileName)
{
    char playerName[260], appSupport[1024];
    snprintf(appSupport, sizeof(appSupport), "%s", GetAppSupportPath(""));
    if (gCurrentPlayerNameExists)
        SWCopyPascalStringToCString(gCurrentPlayerName, playerName);

    if (fileName == NULL) {
        if (!gCurrentPlayerNameExists)
            snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/", appSupport, subfolder);
        else
            snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/%s/", appSupport, subfolder, playerName);
    } else if (!gCurrentPlayerNameExists) {
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/%s", appSupport, subfolder, fileName);
    } else {
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/%s/%s", appSupport, subfolder, playerName, fileName);
    }
    return gImageFilePath;
}

/// Saved games folder; with addMansionName, the per-mansion file inside it.
const char *GetSavedGamesFilePath(const char *fileName, Boolean addMansionName)
{
    char mansionName[255];
    PlayerSubfolderPath("Saved Games", fileName);
    if (!addMansionName) {
        printf("Saved game file path returned: %s \n", gImageFilePath);
        return gImageFilePath;
    }
    snprintf(mansionName, sizeof(mansionName), "%s", gMansionFilename);
    StripStringFromEndOfCString(".zip", mansionName, NULL);
    StripStringFromEndOfCString(".ZIP", mansionName, NULL);
    snprintf(gSavedGameFilePath, sizeof(gSavedGameFilePath), "%s/%s", gImageFilePath, mansionName);
    printf("Saved game file path returned: %s \n", gSavedGameFilePath);
    return gSavedGameFilePath;
}

const char *GetMoviesFilePath(const char *fileName)
{
    return PlayerSubfolderPath("User Movies", fileName);
}

static const char *AppSupportSubfolderPath(const char *subfolder, const char *fileName)
{
    char appSupport[1024];
    snprintf(appSupport, sizeof(appSupport), "%s", GetAppSupportPath(""));
    if (fileName == NULL)
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/", appSupport, subfolder);
    else
        snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s/%s", appSupport, subfolder, fileName);
    return gImageFilePath;
}

const char *GetPlainSavedGamesFilePath(const char *fileName)
{
    AppSupportSubfolderPath("Saved Games", fileName);
    printf("GetPlainSavedGamesFilePath returned: %s\n", gImageFilePath);
    return gImageFilePath;
}

const char *GetPlayerProfilePath(const char *fileName)
{
    return AppSupportSubfolderPath("Player Profiles", fileName);
}

const char *GetUserMoviesPath(const char *fileName)
{
    return AppSupportSubfolderPath("User Movies", fileName);
}

void GetSuspendedGameFilePath(char *outPath, const char *fileName)
{
    snprintf(outPath, 1024, "%s", PlayerSubfolderPath("Suspended Game Data", fileName));
}

void GetPlainSuspendedGameFilePath(char *outPath, const char *fileName)
{
    snprintf(outPath, 1024, "%s", AppSupportSubfolderPath("Suspended Game Data", fileName));
}

const char *GetHomeDirectory(void)
{
    const char *home = getenv("HOME");
#ifndef _WIN32
    if (home == NULL) {
        struct passwd *pw = getpwuid(getuid());
        if (pw != NULL)
            home = pw->pw_dir;
    }
#endif
    return home;
}

/// The folder holding "Game Data": the app bundle's Resources on macOS, the
/// executable's folder elsewhere. MM_RESOURCES_DIR overrides it.
const char *GetResourcesDirectory(void)
{
    static char resourcesDir[1024];
    if (resourcesDir[0] == '\0') {
        const char *env = getenv("MM_RESOURCES_DIR");
        if (env != NULL && env[0] != '\0') {
            snprintf(resourcesDir, sizeof(resourcesDir), "%s", env);
        } else {
            char *base = SDL_GetBasePath();
            snprintf(resourcesDir, sizeof(resourcesDir), "%s", base != NULL ? base : "./");
            SDL_free(base);
        }
            // no trailing slash
        {
            size_t len = strlen(resourcesDir);
            while (len > 1 && resourcesDir[len - 1] == '/')
                resourcesDir[--len] = '\0';
        }
    }
    return resourcesDir;
}

const char *GetGameDataPath(void)
{
    char resources[1024];
    snprintf(resources, sizeof(resources), "%s", GetResourcesDirectory());
    snprintf(gImageFilePath, sizeof(gImageFilePath), "%s/%s", resources, "Game Data");
    return gImageFilePath;
}

/// The folder the game (the .app bundle on macOS) is in.
const char *GetGameDirectory(void)
{
    snprintf(gDirPath, sizeof(gDirPath), "%s", GetResourcesDirectory());
#ifdef __APPLE__
    if (strstr(gDirPath, ".app/Contents/Resources") != NULL)
        StripNDirectoriesFromEndOfPath(gDirPath, 3);
#endif
    return gDirPath;
}

const char *GetCurrentDirectory(void)
{
    return GetGameDirectory();
}

// ---------------------------------------------------------------------------
// Resources

/// Opens a resource file. (The original's SDL_RWFromZZIP could also read from
/// inside zipped mansions; the shipped mansions are plain folders.)
SDL_RWops *MMOpenResourceFile(const char *path, const char *mode)
{
    return SDL_RWFromFile(path, mode);
}

/// Remembers which IDs of a resource type exist, to speed up Resource1Exists.
int ScanDirectoryForResIDs(const char *mansionPath, const char *resType)
{
    char path[500];
    DIR *dir;
    struct dirent *entry;

    gLastValidLevelResIDIndex = 0;
    snprintf(gScannedResType, sizeof(gScannedResType), "%s", resType);
    snprintf(path, sizeof(path), "%s/%s", mansionPath, resType);
    dir = opendir(path);
    if (dir == NULL)
        return -1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        gValidLevelResIDs[gLastValidLevelResIDIndex++] = (short)atoi(entry->d_name);
        if (gLastValidLevelResIDIndex >= kMaxValidResIDs) {
            printf("ERROR! ScanDirectoryForResIDs exceeded array bounds!");
            gLastValidLevelResIDIndex = 0;
            break;
        }
    }
    closedir(dir);
    return 0;
}

Boolean ResourceExists(const char *resType, short resID)
{
    return Resource1Exists(resType, resID);
}

Boolean Resource1Exists(const char *resType, short resID)
{
    char quotedType[10], path[1024];
    SDL_RWops *file;
    short i;

    snprintf(quotedType, sizeof(quotedType), "'%s'", resType);
    if (strcmp(quotedType, gScannedResType) == 0 && gLastValidLevelResIDIndex > 0) {
        for (i = 0; i < gLastValidLevelResIDIndex; i++) {
            if (gValidLevelResIDs[i] == resID)
                return true;
        }
        return false;
    }
    snprintf(path, sizeof(path), "%s'%s'/%05d", gCurOpenMansionFilePath, resType, resID);
    file = MMOpenResourceFile(path, "rb");
    if (file == NULL)
        return false;
    SDL_RWclose(file);
    return true;
}

Boolean DoesFileExist(const char *path)
{
    SDL_RWops *file = SDL_RWFromFile(path, "rb");
    if (file != NULL)
        SDL_RWclose(file);
    return file != NULL;
}

/// Counts files named "<folder><baseName> 1<ext>", "... 2<ext>", ...
short CountNumSuccessiveFiles(const char *folderPath, const char *baseName, const char *extension)
{
    char path[500];
    short i;
    for (i = 1; i > 0; i++) {
        snprintf(path, sizeof(path), "%s%s %d%s", folderPath, baseName, i, extension);
        if (!DoesFileExist(path))
            break;
    }
    return i - 1;
}

/// Number of resources of a type with IDs 128 and up (the original counted into a char).
short MMCountResources(const char *resType)
{
    signed char count = 0;
    short resID;
    for (resID = 128; resID != 0x7fff; resID++) {
        if (Resource1Exists(resType, resID))
            count++;
    }
    return count;
}

int PrintFilesInFolder(const char *folderPath)
{
    DIR *dir = opendir(folderPath);
    struct dirent *entry;
    int count = 0;
    if (dir == NULL)
        return -1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        count++;
        printf("\n%s", entry->d_name);
    }
    printf("\n\nreaddir() found a total of %i files\n", count);
    closedir(dir);
    return 0;
}

short CountNumFilesInFolder(const char *folderPath)
{
    DIR *dir = opendir(folderPath);
    struct dirent *entry;
    short count = 0;
    if (dir == NULL)
        return 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.')
            count++;
    }
    closedir(dir);
    return count;
}

void *MMGetResource(const char *resType, short resID, size_t size)
{
    return MMGetResourceUsingPath(resType, resID, size, gCurOpenMansionFilePath);
}

/// Reads up to size bytes of a resource into a zeroed buffer (free() it), or NULL.
void *MMGetResourceUsingPath(const char *resType, short resID, size_t size, const char *mansionPath)
{
    char path[1024];
    SDL_RWops *file;
    void *data = NULL;

    snprintf(path, sizeof(path), "%s'%s'/%05d", mansionPath, resType, resID);
    file = MMOpenResourceFile(path, "rb");
    if (file != NULL) {
        data = calloc(1, size);
        if (data != NULL)
            SDL_RWread(file, data, 1, size);
        SDL_RWclose(file);
    }
    return data;
}

long MMGetMansionVersion(const char *resType, short resID, size_t size, const char *mansionPath)
{
    char path[1024];
    SDL_RWops *file;
    Uint32 version = 0xffffffff;

    (void)size;
    snprintf(path, sizeof(path), "%s'%s'/%05d", mansionPath, resType, resID);
    file = MMOpenResourceFile(path, "rb");
    if (file != NULL) {
        SDL_RWread(file, &version, 1, 4);
        SDL_RWclose(file);
    }
    return (long)(Sint32)SDL_SwapBE32(version);
}

/// Returns the TEXT resource as a malloc'ed C string (free() it), or NULL.
char *MMGetCStringFromTEXTResource(short resID)
{
    char path[1024];
    SDL_RWops *file;
    char *buffer, *result = NULL;
    size_t length = 0;

    snprintf(path, sizeof(path), "%s'%s'/%05d.txt", gCurOpenMansionFilePath, "TEXT", resID);
    file = MMOpenResourceFile(path, "rb");
    if (file == NULL) {
        printf("Couldn't load TEXT resource at path: %s \n", path);
        return NULL;
    }
    buffer = (char *)calloc(1, 4000);
    if (buffer != NULL)
        length = SDL_RWread(file, buffer, 1, 4000);
    SDL_RWclose(file);
    if (buffer != NULL) {
        result = (char *)calloc(1, length + 2);
        if (result != NULL)
            memcpy(result, buffer, length);
        free(buffer);
    }
    return result;
}

/// Size of a SPRT resource: a big-endian count of 22-byte sprite records.
long GetSPRTResourceSize(short resID)
{
    char path[1024];
    SDL_RWops *file;
    Uint32 count = 0;

    snprintf(path, sizeof(path), "%s'%s'/%05d", gCurOpenMansionFilePath, "SPRT", resID);
    file = MMOpenResourceFile(path, "rb");
    if (file != NULL) {
        SDL_RWread(file, &count, 4, 1);
        count = SDL_SwapBE32(count);
        SDL_RWclose(file);
    }
    return (long)count * 0x16;
}

// ---------------------------------------------------------------------------
// Menu and editor entry points (empty in the SDL build)

void EnableRecordMovieMenuItem(void) {}

/// The original's Cocoa menu bar (menubar.m) enabled menu items for the current screen.
/// The SDL build has no custom menus; everything is reachable in game or by keyboard.
void UpdateMenuBar(void) {}
void SetUpMenuBar(void) {}
void OpenGameSupportFiles(void) {}
void StartUpEditor(void) {}
void ExitToEditor(void) {}
