/*
 * MMExtras.c — settings, lifetime statistics and Fun-tab gameplay changes added by this
 * version. None of this exists in the original game; with every option off the game plays
 * exactly as before.
 */

#include <sys/stat.h>
#include <dirent.h>
#include "MMGame.h"
#include "MMProtos.h"
#include "MMExtras.h"
#include "BKGL.h"

ExtrasSettings gExtras;
LifetimeStats gStats[kMaxPlayers];
short gActivePlayer = 0;

// ---------------------------------------------------------------------------
// Options. Descriptions are shown in the box below the checkboxes while one is hovered.

static const ExtrasOption kFunOptions[] = {
    { "download_mansion", "You Wouldn't Download a Mansion",
      "The anti-piracy Medusas are back: super fast, all over every room. Unplayable, on purpose. Cheater!",
      &gExtras.piracyMedusas },
    { "medusa_late", "Medusa's Late for Work?",
      "Medusa heads fly two and a half times faster.", &gExtras.medusaLate },
    { "moon_gravity", "Jackie, Watch the Stars",
      "Moon gravity: Jack falls at 40% speed and floats farther. Long falls don't hurt.", &gExtras.moonGravity },
    { "pinata", "Jack's Pinata Party",
      "Confetti bursts out whenever Jack dies.", &gExtras.pinata },
    { "all_spiders", "Oops! All Spiders",
      "Every enemy looks like a spider, but still acts like what it really is.", &gExtras.allSpiders },
    { "too_scared", "I'm Too S-S-Scared",
      "No Medusas, eyeballs, spiders or skeletons.", &gExtras.tooScared },
    { "nine_lives", "Nine-Lived Malone",
      "Infinite lives. Only in mansions you've already beaten.", &gExtras.nineLives },
    { "espresso", "Jack on Espresso",
      "Jack runs half again as fast. Only in mansions you've already beaten.", &gExtras.espresso },
    { "pitch_black", "Pitch Black",
      "The room is dark except for a small circle of light around Jack.", &gExtras.pitchBlack },
    { "my_tuba", "My Tuba!",
      "Starting a new game plays the Out of This World guard instead of the evil laugh.", &gExtras.myTuba },
};

static const ExtrasOption kExtrasOptions[] = {
    { "arcade_menu", "Centered main menu",
      "Centers the main menu text, like an arcade cabinet.", &gExtras.arcadeMenu },
    { "smooth_rooms", "Smooth room transitions",
      "Slides from one room to the next instead of cutting.", &gExtras.smoothRooms },
    { "rumble", "Controller rumble",
      "A tap for keys and gems, a heavy rumble for doors and boulders, a jolt when Jack dies.", &gExtras.rumble },
    { "multiplayer_smoke", "Smoke in multiplayer",
      "In multiplayer games a cloud of smoke bursts out where a player dies.", &gExtras.multiplayerSmoke },
};

int GetExtrasOptions(int tab, const ExtrasOption **optionsP)
{
    if (tab == kExtrasTabFun) {
        *optionsP = kFunOptions;
        return (int)SDL_arraysize(kFunOptions);
    }
    *optionsP = kExtrasOptions;
    return (int)SDL_arraysize(kExtrasOptions);
}

static const char *ExtrasSettingsPath(void)
{
    return GetAppSupportPath("Midnight Mansion HD Extras.txt");
}

void LoadExtrasSettings(void)
{
    char line[512], key[128];
    int value, tab, i, count;
    const ExtrasOption *options;
    FILE *file;

    memset(&gExtras, 0, sizeof(gExtras));
    gExtras.rumble = true;
    gExtras.multiplayerSmoke = true;
    file = fopen(ExtrasSettingsPath(), "r");
    if (file == NULL)
        return;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, " %127[a-z_0-9] = %d", key, &value) != 2)
            continue;
        for (tab = 0; tab < 2; tab++) {
            count = GetExtrasOptions(tab, &options);
            for (i = 0; i < count; i++)
                if (strcmp(options[i].key, key) == 0)
                    *options[i].valueP = value != 0;
        }
    }
    fclose(file);
}

/// After the Options dialog's OK: settings that show outside of play take effect now.
void ExtrasSettingsChanged(void)
{
    if (gCurrentGameMode == kTitleScreenMode)
        LayOutTitleScreenButtons();
}

void SaveExtrasSettings(void)
{
    const ExtrasOption *options;
    int tab, i, count;
    FILE *file = fopen(ExtrasSettingsPath(), "w");

    if (file == NULL)
        return;
    fputs("# Midnight Mansion HD: settings added by this version (1 = on)\n", file);
    for (tab = 0; tab < 2; tab++) {
        count = GetExtrasOptions(tab, &options);
        for (i = 0; i < count; i++)
            fprintf(file, "%s = %d\n", options[i].key, *options[i].valueP ? 1 : 0);
    }
    fclose(file);
}

// ---------------------------------------------------------------------------
// Files

static Boolean FileExists(const char *path)
{
    struct stat info;
    return stat(path, &info) == 0;
}

/// Looks in MM_EXTRAS_DIR, then "Game Data Extras" next to "Game Data", then the source tree.
const char *GetExtrasFilePath(const char *relativePath)
{
    static char path[1200];
    const char *env = getenv("MM_EXTRAS_DIR");

    if (env != NULL && env[0] != '\0') {
        snprintf(path, sizeof(path), "%s/%s", env, relativePath);
        if (FileExists(path))
            return path;
    }
    snprintf(path, sizeof(path), "%s/Game Data Extras/%s", GetResourcesDirectory(), relativePath);
    if (FileExists(path))
        return path;
#ifdef MM_EXTRAS_SOURCE_DIR
    snprintf(path, sizeof(path), "%s/%s", MM_EXTRAS_SOURCE_DIR, relativePath);
    if (FileExists(path))
        return path;
#endif
    return NULL;
}

const char *GetBundledCustomMansionsPath(void)
{
    const char *path = GetExtrasFilePath("Custom Mansions");
    return path != NULL ? path : "";
}

// ---------------------------------------------------------------------------
// Lifetime statistics

static const char *StatsFilePath(const char *playerName)
{
    static char path[1200];
    char folder[1024];

    snprintf(folder, sizeof(folder), "%s", GetAppSupportPath(""));
    MMCreateDirectory(folder, "Player Stats");
    snprintf(path, sizeof(path), "%s/Player Stats/%s.txt", folder, playerName);
    return path;
}

void LoadLifetimeStats(short player, const char *playerName)
{
    LifetimeStats *s = &gStats[player];
    char line[2300], key[64], rest[2200];
    long value;
    int d, m;
    FILE *file;

    memset(s, 0, sizeof(*s));
    snprintf(s->playerName, sizeof(s->playerName), "%s", playerName != NULL ? playerName : "");
    if (playerName == NULL || playerName[0] == '\0')
        return;
    file = fopen(StatsFilePath(playerName), "r");
    if (file == NULL)
        return;
    while (fgets(line, sizeof(line), file) != NULL) {
        rest[0] = 0;
        if (sscanf(line, " %63[a-z_0-9] = %2199[^\n]", key, rest) < 1)
            continue;
        value = atol(rest);
        if (strcmp(key, "pixels_fallen") == 0) s->pixelsFallen = atof(rest);
        else if (strcmp(key, "keys_turned") == 0) s->keysTurned = value;
        else if (strcmp(key, "doors_opened") == 0) s->doorsOpened = value;
        else if (strcmp(key, "coins") == 0) s->coinsCollected = value;
        else if (strcmp(key, "games_started") == 0) s->gamesStarted = value;
        else if (strcmp(key, "mansions_beaten") == 0) s->mansionsBeaten = value;
        else if (strcmp(key, "seconds_played") == 0) s->secondsPlayed = value;
        else if (sscanf(key, "deaths_%d", &d) == 1 && d >= 0 && d < kNumDeathCauses) s->deaths[d] = value;
        else if (sscanf(key, "looted_%d_%d", &d, &m) == 2 && d >= 0 && d < 3 && m >= 0 && m < 16)
            s->looted[d][m] = (unsigned char)(value != 0);
        else if (sscanf(key, "beaten_%d_%d", &d, &m) == 2 && d >= 0 && d < 3 && m >= 0 && m < 16)
            s->beaten[d][m] = (unsigned char)(value != 0);
        else if (strcmp(key, "beaten_custom") == 0 && strlen(s->beatenCustom) + strlen(rest) + 2 < sizeof(s->beatenCustom)) {
            strcat(s->beatenCustom, rest);
            strcat(s->beatenCustom, "\n");
        }
    }
    fclose(file);
}

void SaveLifetimeStats(short player)
{
    LifetimeStats *s = &gStats[player];
    char custom[sizeof(s->beatenCustom)], *name;
    int d, m;
    FILE *file;

    if (s->playerName[0] == '\0')
        return;
    file = fopen(StatsFilePath(s->playerName), "w");
    if (file == NULL)
        return;
    fprintf(file, "# Midnight Mansion HD lifetime stats for %s\n", s->playerName);
    fprintf(file, "pixels_fallen = %.1f\n", s->pixelsFallen);
    fprintf(file, "keys_turned = %ld\n", s->keysTurned);
    fprintf(file, "doors_opened = %ld\n", s->doorsOpened);
    fprintf(file, "coins = %ld\n", s->coinsCollected);
    fprintf(file, "games_started = %ld\n", s->gamesStarted);
    fprintf(file, "mansions_beaten = %ld\n", s->mansionsBeaten);
    fprintf(file, "seconds_played = %ld\n", s->secondsPlayed);
    for (d = 0; d < kNumDeathCauses; d++)
        fprintf(file, "deaths_%d = %ld\n", d, s->deaths[d]);
    for (d = 0; d < 3; d++)
        for (m = 0; m < 16; m++) {
            if (s->looted[d][m])
                fprintf(file, "looted_%d_%d = 1\n", d, m);
            if (s->beaten[d][m])
                fprintf(file, "beaten_%d_%d = 1\n", d, m);
        }
    snprintf(custom, sizeof(custom), "%s", s->beatenCustom);
    for (name = strtok(custom, "\n"); name != NULL; name = strtok(NULL, "\n"))
        fprintf(file, "beaten_custom = %s\n", name);
    fclose(file);
}

LifetimeStats *ActiveStats(void)
{
    return &gStats[gActivePlayer >= 0 && gActivePlayer < GetNumPlayers() ? gActivePlayer : 0];
}

const char *GetDeathCauseName(int cause)
{
    static const char *names[kNumDeathCauses] = {
        "Lava", "Zappers", "Medusas", "Long falls", "Spiders", "Skeletons", "Eyeballs", "Other creatures",
        "Other"
    };
    return cause >= 0 && cause < kNumDeathCauses ? names[cause] : "Other";
}

/// Jack is about three tiles (48 pixels) tall; call him five and a half feet.
double PixelsToMiles(double pixels)
{
    return pixels * (5.5 / 48.0) / 5280.0;
}

static Boolean StatsAreTracked(void)
{
    return !IsMoviePlaying() && ActiveStats()->playerName[0] != '\0';
}

void StatsNoteDeath(int cause)
{
    if (StatsAreTracked() && cause >= 0 && cause < kNumDeathCauses)
        ActiveStats()->deaths[cause]++;
}

void StatsNoteFall(float pixels)
{
    if (StatsAreTracked() && pixels > 0)
        ActiveStats()->pixelsFallen += pixels;
}

void StatsNoteKeyTurned(void)
{
    if (StatsAreTracked()) {
        ActiveStats()->keysTurned++;
        ActiveStats()->doorsOpened++;
    }
    Rumble(kRumbleHeavy);
}

void StatsNoteCoin(void)
{
    if (StatsAreTracked())
        ActiveStats()->coinsCollected++;
}

void StatsNoteGameStarted(void)
{
    int p;

    if (IsMoviePlaying())
        return;
    for (p = 0; p < GetNumPlayers(); p++) {
        if (gStats[p].playerName[0] != '\0') {
            gStats[p].gamesStarted++;
            SaveLifetimeStats((short)p);
        }
    }
}

void StatsNoteSecondsPlayed(long seconds)
{
    if (StatsAreTracked() && seconds > 0)
        ActiveStats()->secondsPlayed += seconds;
}

/// Every collectible in the mansion (money, keys, shield, backpack, lantern, map) has been taken.
static Boolean IsCurrentMansionLooted(void)
{
    char folder[1100], path[1200];
    DIR *dir;
    struct dirent *entry;
    unsigned char *data;
    long count, i, size;
    short type;
    Sint32 objectID;
    Boolean looted = true;
    FILE *file;

    snprintf(folder, sizeof(folder), "%s'SPRT'", gCurOpenMansionFilePath);
    dir = opendir(folder);
    if (dir == NULL)
        return false;
    while (looted && (entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        snprintf(path, sizeof(path), "%s/%s", folder, entry->d_name);
        file = fopen(path, "rb");
        if (file == NULL)
            continue;
        fseek(file, 0, SEEK_END);
        size = ftell(file);
        fseek(file, 0, SEEK_SET);
        data = (unsigned char *)malloc((size_t)(size > 4 ? size : 4));
        if (data != NULL && fread(data, 1, (size_t)size, file) == (size_t)size && size >= 4) {
            count = (long)SDL_SwapBE32(*(Uint32 *)data);
            for (i = 0; i < count && 4 + i * 0x16 + 0x16 <= size; i++) {
                type = (short)SDL_SwapBE16(*(Uint16 *)(data + 4 + i * 0x16));
                objectID = (Sint32)SDL_SwapBE32(*(Uint32 *)(data + 4 + i * 0x16 + 12));
                if (type >= 0x1e && type <= 0x2c && type != kSignObjectSprite && !IsObjectInArray(objectID)) {
                    looted = false;
                    break;
                }
            }
        }
        free(data);
        fclose(file);
    }
    closedir(dir);
    return looted;
}

/// Everyone in a co-op game beat the mansion.
void StatsNoteMansionBeaten(void)
{
    char name[300];
    int d = gDifficultySetting, p;
    Boolean looted = false;
    LifetimeStats *s;

    if (IsMoviePlaying())
        return;
    if (!gMansionIsCustomMansion && gCurrentMansionNumber >= 0 && gCurrentMansionNumber < 16)
        looted = IsCurrentMansionLooted();
    for (p = 0; p < GetNumPlayers(); p++) {
        s = &gStats[p];
        if (s->playerName[0] == '\0')
            continue;
        s->mansionsBeaten++;
        if (gMansionIsCustomMansion) {
            snprintf(name, sizeof(name), "%s", gMansionFilename);
            if (!HasPlayerBeatenCustomMansionStats(s, name) &&
                strlen(s->beatenCustom) + strlen(name) + 2 < sizeof(s->beatenCustom)) {
                strcat(s->beatenCustom, name);
                strcat(s->beatenCustom, "\n");
            }
        }
        else if (gCurrentMansionNumber >= 0 && gCurrentMansionNumber < 16 && d >= 0 && d < 3) {
            s->beaten[d][gCurrentMansionNumber] = 1;
            if (looted)
                s->looted[d][gCurrentMansionNumber] = 1;
        }
        SaveLifetimeStats((short)p);
    }
}

/// Saves everyone's stats (at the end of a game).
void SaveAllLifetimeStats(void)
{
    int p;

    for (p = 0; p < GetNumPlayers(); p++)
        SaveLifetimeStats((short)p);
}

int DeathCauseForEnemy(SpritePtr enemyP)
{
    switch (enemyP != NULL ? (int)enemyP->userData : -1) {
        case kMedusaHeadSprite1: case kMedusaHeadSprite2:          return kDeathByMedusa;
        case kSkullSpiderSprite: case kSmallSpiderSprite: case kBigSpiderSprite: return kDeathBySpider;
        case kSkeletonFishSprite: case kWalkingSkeletonSprite:      return kDeathBySkeleton;
        case kEyeballSprite:                                        return kDeathByEyeball;
        case -1:                                                    return kDeathByOther;
        default:                                                    return kDeathByCreature;
    }
}

Boolean HasPlayerBeatenCustomMansion(const char *name)
{
    return HasPlayerBeatenCustomMansionStats(ActiveStats(), name);
}

/// True if a line of the beaten-custom-mansions list is name (or name.zip).
Boolean HasPlayerBeatenCustomMansionStats(LifetimeStats *s, const char *name)
{
    const char *list = s->beatenCustom, *line, *end;
    size_t len = strlen(name), lineLen;

    for (line = list; *line != '\0'; line = *end ? end + 1 : end) {
        end = strchr(line, '\n');
        if (end == NULL)
            end = line + strlen(line);
        lineLen = (size_t)(end - line);
        if ((lineLen == len || (lineLen == len + 4 && SDL_strncasecmp(line + len, ".zip", 4) == 0)) &&
            SDL_strncasecmp(line, name, len) == 0)
            return true;
    }
    return false;
}

Boolean HasBeatenCurrentMansion(void)
{
    if (NetGameActive())        // two computers: each told the other at the start
        return NetPlayerHasBeatenMansion(gActivePlayer);
    LifetimeStats *s = ActiveStats();
    int d;

    if (gMansionIsCustomMansion)
        return gMansionFilename[0] != '\0' && HasPlayerBeatenCustomMansion(gMansionFilename);
    if (gCurrentMansionNumber < 0)
        return false;
    for (d = 0; d < 3; d++) {
        if (IsMansionInArray((short)d, gCurrentMansionNumber))
            return true;
        if (gCurrentMansionNumber < 16 && s->beaten[d][gCurrentMansionNumber])
            return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Fun tab: room contents

Boolean ExtrasFilterRoomSprite(SPRTRecord *recP)
{
    short type = recP->type;

    if (gExtras.tooScared) {
        if (type == kMedusaHeadSprite1 || type == kMedusaHeadSprite2 || type == kEyeballSprite ||
            type == kSkullSpiderSprite || type == kSmallSpiderSprite || type == kBigSpiderSprite ||
            type == kSkeletonFishSprite || type == kWalkingSkeletonSprite)
            return false;
    }
    return true;
}

// "Oops! All Spiders": every enemy is drawn as a spider but keeps its own behaviour, size and
// collisions. Crawlers become skull spiders standing where they stand; flyers become the purple
// spiders, centred where they fly.

static Boolean IsSpiderSkinnedType(long type)
{
    return type == kScorpionSprite || type == kEyeballSprite || type == kBlueRavenSprite ||
           type == kRedRavenSprite || type == kSnakeSprite1 || type == kSnakeSprite2 ||
           type == kMedusaHeadSprite1 || type == kMedusaHeadSprite2 || type == kSkeletonFishSprite ||
           type == kWalkingSkeletonSprite;
}

static Boolean IsFlyingType(long type)
{
    return type == kEyeballSprite || type == kBlueRavenSprite || type == kRedRavenSprite ||
           type == kMedusaHeadSprite1 || type == kMedusaHeadSprite2;
}

static void SpiderSkinDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    static SpritePtr skins[2] = { NULL, NULL };
    Boolean flying = IsFlyingType(srcSpriteP->userData);
    SpritePtr masterP = gSpriteArray[flying ? kSmallSpiderSprite : kSkullSpiderSprite], skinP;
    FramePtr frameP;
    float width = SWGetSpriteWidth(srcSpriteP), height = SWGetSpriteHeight(srcSpriteP);
    float frameW, frameH, skinW, skinH;

    (void)curFrameP;
    if (masterP == NULL || masterP->numFrames < 1)
        return;
    skinP = skins[flying];
    if (skinP == NULL || skinP->frameArray != masterP->frameArray) {
        if (SWCloneSprite(masterP, &skins[flying], NULL, 0) != kNoError)
            return;
        skinP = skins[flying];
    }
        // a walking animation of its own, so every enemy looks alive
    frameP = masterP->frameArray[(SDL_GetTicks() / (flying ? 120 : 160) + (Uint32)(uintptr_t)srcSpriteP / 64) %
                                 (Uint32)masterP->numFrames];
    frameW = (float)(frameP->frameRect.right - frameP->frameRect.left);
    frameH = (float)(frameP->frameRect.bottom - frameP->frameRect.top);
    if (frameW < 1 || frameH < 1)
        return;
    skinW = width < 40.0f ? 40.0f : width;
    skinH = skinW * frameH / frameW;
    SWSetSpriteScaledFactor(skinP, skinW / frameW, skinH / frameH);
    skinP->drawData->flippedHorizontal = srcSpriteP->drawData->flippedHorizontal;
    skinP->drawData->translucencyLevel = srcSpriteP->drawData->translucencyLevel;
    skinP->drawData->colorRed = skinP->drawData->colorGreen = skinP->drawData->colorBlue = 0xffff;
    horizLoc += (width - skinW) / 2.0f;
    vertLoc += flying ? (height - skinH) / 2.0f : height - skinH;
    SWStdSpriteDrawProc(skinP, frameP, horizLoc, vertLoc);
}

static void ApplySpiderSkins(void)
{
    SpriteLayerPtr layers[] = { gEnemySpriteLayerP, gMedusaSpriteLayerP, gEyeballSpriteLayerP, gBirdSpriteLayerP,
                                gSnakeSpriteLayerP };
    SpritePtr spriteP;
    size_t i;

    if (!gExtras.allSpiders || gCurrentGameMode != kPlayingGameMode)
        return;
    for (i = 0; i < SDL_arraysize(layers); i++) {
        if (layers[i] == NULL)
            continue;
        for (spriteP = SWGetNextSprite(layers[i], NULL); spriteP != NULL; spriteP = SWGetNextSprite(layers[i], spriteP))
            if (IsSpiderSkinnedType(spriteP->userData) && spriteP->frameDrawProc != SpiderSkinDrawProc)
                SWSetSpriteDrawProc(spriteP, SpiderSkinDrawProc);
    }
}

static SpritePtr gDarknessSpriteP = NULL;
static Uint32 gNextCheaterSound = 0;

/// "Pitch Black": everything but a soft circle around Jack is covered in black.
static void DarknessDrawProc(SpritePtr srcSpriteP, FramePtr curFrameP, float horizLoc, float vertLoc)
{
    SpriteWorldPtr w = gSWCurrentSpriteWorld;
    SpritePtr heroP = GetHeroSprite();
    float cx, cy, inner = 58.0f, outer = 112.0f, far = 4000.0f, a;
    int i;
    const int kSteps = 48;

    (void)srcSpriteP; (void)curFrameP; (void)horizLoc; (void)vertLoc;
    if (w == NULL)
        return;
    if (heroP == NULL)
        heroP = SWGetNextSprite(gDeadHeroSpriteLayerP, NULL);
    if (heroP != NULL) {
        cx = (heroP->destFrameRect.left + heroP->destFrameRect.right) / 2.0f;
        cy = (heroP->destFrameRect.top + heroP->destFrameRect.bottom) / 2.0f;
    } else {
        cx = (w->visScrollRect.left + w->visScrollRect.right) / 2.0f;
        cy = (w->visScrollRect.top + w->visScrollRect.bottom) / 2.0f;
        inner = outer = 0.0f;
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glScalef(w->designScaleH * w->zoomH, w->designScaleV * w->zoomV, 1.0f);
    glTranslatef(-w->spriteOffsetH, -w->spriteOffsetV, 0);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLE_STRIP);         // soft edge
    for (i = 0; i <= kSteps; i++) {
        a = (float)i / kSteps * 6.2831853f;
        glColor4f(0, 0, 0, 0.0f);
        glVertex2f(cx + cosf(a) * inner, cy + sinf(a) * inner);
        glColor4f(0, 0, 0, 1.0f);
        glVertex2f(cx + cosf(a) * outer, cy + sinf(a) * outer);
    }
    glEnd();
    glColor4f(0, 0, 0, 1.0f);
    glBegin(GL_TRIANGLE_STRIP);         // everything else
    for (i = 0; i <= kSteps; i++) {
        a = (float)i / kSteps * 6.2831853f;
        glVertex2f(cx + cosf(a) * outer, cy + sinf(a) * outer);
        glVertex2f(cx + cosf(a) * far, cy + sinf(a) * far);
    }
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);
    glPopMatrix();
}

static void UpdateDarkness(void)
{
    Boolean want = gExtras.pitchBlack && gCurrentGameMode == kPlayingGameMode;

    if (gDarknessSpriteP != NULL && gDarknessSpriteP->parentSpriteLayerP != gExtrasOverlaySpriteLayerP)
        gDarknessSpriteP = NULL;        // its layer was cleared
    if (want && gDarknessSpriteP == NULL && SWCreateSprite(&gDarknessSpriteP, NULL, 0) == kNoError) {
        SWSetSpriteDrawProc(gDarknessSpriteP, DarknessDrawProc);
        gDarknessSpriteP->destFrameRect = gSpriteWorldP->workAreaRect;
        SWAddSprite(gExtrasOverlaySpriteLayerP, gDarknessSpriteP);
    }
    else if (!want && gDarknessSpriteP != NULL) {
        SWRemoveSpriteFromAnimation(gDarknessSpriteP, true);
        gDarknessSpriteP = NULL;
    }
}

/// "You Wouldn't Download a Mansion": several fast Medusa heads at random places.
static void AddPiracyMedusas(void)
{
    SWRect *r = &gSpriteWorldP->visScrollRect;
    SpritePtr medusaP, heroP = GetHeroSprite();
    short i, count = (short)GetRandom(4, 7), tries;
    float h, v, dx, dy;

    for (i = 0; i < count; i++) {
        medusaP = NewMedusasHeadSprite(i & 1 ? kMedusaHeadSprite2 : kMedusaHeadSprite1);
        if (medusaP == NULL)
            break;
        GAME_SPRITE(medusaP)->objectID = -1;
        tries = 0;
        do {        // not right on top of Jack
            h = (float)GetRandom(r->left + 20, r->right - 60);
            v = (float)GetRandom(r->top + 20, r->bottom - 60);
            dx = heroP != NULL ? h - SWGetSpriteHorizLoc(heroP) : 999.0f;
            dy = heroP != NULL ? v - SWGetSpriteVertLoc(heroP) : 999.0f;
        } while (dx * dx + dy * dy < 160.0f * 160.0f && ++tries < 20);
        SWSetSpriteLocation(medusaP, h, v);
        MEDUSA(medusaP)->speed = GetRandomFloat(1.2f, 2.0f);
        SetUpSpriteFrameRange(medusaP);
    }
}

void ExtrasRoomLoaded(void)
{
    ApplySpiderSkins();
    if (gExtras.piracyMedusas && gCurrentGameMode == kPlayingGameMode)
        AddPiracyMedusas();
    gDarknessSpriteP = NULL;
    UpdateDarkness();
}

void ExtrasGameFrame(void)
{
    static Uint32 playedMs = 0, lastTick = 0;
    Uint32 now = SDL_GetTicks();

        // time played, for the Trophy Room
    if (gCurrentGameMode == kPlayingGameMode && !gGameIsPaused && lastTick != 0 && now - lastTick < 500)
        playedMs += now - lastTick;
    lastTick = now;
    if (playedMs >= 1000) {
        StatsNoteSecondsPlayed((long)(playedMs / 1000));
        playedMs %= 1000;
    }

    UpdateDarkness();
    ApplySpiderSkins();             // also enemies that appear later
    if (gExtras.piracyMedusas && gCurrentGameMode == kPlayingGameMode && !gGameIsPaused) {
            // "Cheater!" pulses at random moments, about once a second
        if (gNextCheaterSound == 0 || now >= gNextCheaterSound) {
            if (gNextCheaterSound != 0)
                MMPlaySound(72, 10, 1);
            gNextCheaterSound = now + 700 + (Uint32)(rand() % 900);     // not the game's random numbers
        }
    }
    else {
        gNextCheaterSound = 0;
    }
}

// ---------------------------------------------------------------------------
// Fun tab: Jack

/// Confetti: gold sparkles tinted bright colours, flung up and falling with a little drag.
static void ConfettiMoveProc(SpritePtr spriteP)
{
    DrawDataPtr d = spriteP->drawData;
    MiscSpritePtr miscP = MISC(spriteP);

    d->vertMoveDelta += 0.18f;
    d->horizMoveDelta *= 0.985f;
    SWOffsetSprite(spriteP, d->horizMoveDelta, d->vertMoveDelta);
    d->rotation += spriteP->miscFloat;
    if (--miscP->frameTimer <= 0 || spriteP->destFrameRect.top > gSpriteWorldP->visScrollRect.bottom + 20)
        SWRemoveSpriteFromAnimation(spriteP, true);
    else if (miscP->frameTimer < 30)
        SWSetSpriteTranslucencyLevel(spriteP, (long)(0xffff * miscP->frameTimer / 30));
}

/// A plain white paper rectangle, tinted per piece.
static SpritePtr ConfettiMaster(void)
{
    static SpritePtr masterP = NULL;
    SDL_Surface *surface;

    if (masterP != NULL)
        return masterP;
    surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 5, 32, SDL_PIXELFORMAT_ABGR8888);
    if (surface == NULL)
        return NULL;
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 255, 255, 255));
    if (SWCreateSpriteFromSurfaceXY(&masterP, NULL, surface, 8, 5, 0, 0, false, 1, false) != kNoError)
        masterP = NULL;
    return masterP;
}

static void BurstConfetti(SpritePtr heroP)
{
    static const unsigned short colors[][3] = {
        { 0xffff, 0x3000, 0x3000 }, { 0x3000, 0xffff, 0x4000 }, { 0x3000, 0x7000, 0xffff },
        { 0xffff, 0xf000, 0x2000 }, { 0xffff, 0x4000, 0xffff }, { 0x2000, 0xffff, 0xffff }
    };
    SpritePtr masterP = ConfettiMaster(), bitP;
    void *storageP;
    float cx, cy, angle, speed;
    int i;

    if (masterP == NULL)
        return;
    cx = (heroP->destFrameRect.left + heroP->destFrameRect.right) / 2.0f;
    cy = (heroP->destFrameRect.top + heroP->destFrameRect.bottom) / 2.0f;
    for (i = 0; i < 80; i++) {
        storageP = calloc(1, sizeof(MiscSpriteRec));
        if (storageP == NULL || SWCloneSprite(masterP, &bitP, storageP, sizeof(MiscSpriteRec)) != kNoError) {
            free(storageP);
            break;
        }
        bitP->userData = kGoldSparkleSprite;
        GAME_SPRITE(bitP)->objectID = -1;
        SWAddSprite(gTopSpriteLayerP, bitP);
        SWSetSpriteScaledFactor(bitP, GetRandomFloat(0.6f, 1.1f), GetRandomFloat(0.6f, 1.2f));
        SWSetSpriteColor(bitP, colors[i % 6][0], colors[i % 6][1], colors[i % 6][2]);
        SWSetSpriteLocation(bitP, cx + GetRandomFloat(-6.0f, 6.0f), cy + GetRandomFloat(-10.0f, 6.0f));
        angle = GetRandomFloat(-3.0f, -0.14f);
        speed = GetRandomFloat(2.0f, 6.5f);
        SWSetSpriteMoveDelta(bitP, cosf(angle) * speed, sinf(angle) * speed - 1.0f);
        SWSetSpriteMoveProc(bitP, ConfettiMoveProc);
        SWSetSpriteMoveTime(bitP, 0);
        MISC(bitP)->frameTimer = (short)GetRandom(70, 120);
        bitP->miscFloat = GetRandomFloat(-14.0f, 14.0f);
    }
}

/// Multiplayer deaths: a cloud of smoke rolls out from where the player was.
static void SmokeMoveProc(SpritePtr spriteP)
{
    DrawDataPtr d = spriteP->drawData;

    d->horizMoveDelta *= 0.94f;
    d->vertMoveDelta = d->vertMoveDelta * 0.94f - 0.025f;
    SWOffsetSprite(spriteP, d->horizMoveDelta, d->vertMoveDelta);
    d->rotation += spriteP->miscFloat;
}

static void BurstSmoke(SpritePtr heroP)
{
    SpritePtr puffP;
    float angle, speed, scale;
    unsigned short gray;
    int i;

    if (gSpriteArray[kPuffOfSmokeSprite] == NULL)
        return;
    for (i = 0; i < 9; i++) {
        puffP = NewPuffOfSmokeSprite();
        if (puffP == NULL)
            break;
        scale = GetRandomFloat(0.55f, 1.1f);
        SWSetSpriteScaledFactor(puffP, scale, scale);
            // plain grey, half see-through: a soft cloud rather than a blast
        gray = (unsigned short)GetRandom(0x8000, 0xb000);
        SWSetSpriteColor(puffP, gray, gray, gray);
        SWSetSpriteTranslucencyLevel(puffP, (unsigned long)GetRandom(0x6000, 0x9000));
        SWCenterSpriteOverSprite(puffP, heroP);
        SWOffsetSprite(puffP, GetRandomFloat(-6.0f, 6.0f), GetRandomFloat(-8.0f, 6.0f));
        angle = GetRandomFloat(0.0f, 6.283f);
        speed = GetRandomFloat(0.4f, 1.5f);
        SWSetSpriteMoveDelta(puffP, cosf(angle) * speed, sinf(angle) * speed * 0.7f - 0.4f);
        SWSetSpriteMoveProc(puffP, SmokeMoveProc);
        SWSetSpriteMoveTime(puffP, 0);
        puffP->miscFloat = GetRandomFloat(-1.5f, 1.5f);
        MySetSpriteFrameTime(puffP, (short)GetRandom(110, 170));
        SWResetSpriteFrameTimer(gSpriteWorldP, puffP);
    }
}

void ExtrasHeroDied(SpritePtr heroP)
{
    Rumble(kRumbleJolt);
    SplitNoteDeath();
    if (heroP == NULL || gCurrentGameMode != kPlayingGameMode)
        return;
    if (gExtras.pinata)
        BurstConfetti(heroP);
    if (IsMultiplayerGame() || SplitScreenActive()) {
            // multiplayer: smoke where the player fell, and the bell tolls for them (40% volume)
        if (gExtras.multiplayerSmoke)
            BurstSmoke(heroP);
        if (GetExtraSoundIndex(kExtraSoundBell) >= 0)
            MMPlayStereoSound2(NULL, GetExtraSoundIndex(kExtraSoundBell), 12, 1, (short)(gSoundVolume * 0.4f), 1.0, 0);
    }
}

Boolean ExtrasKeepLife(void)
{
    return gExtras.nineLives && HasBeatenCurrentMansion();
}

float ExtrasWalkSpeedFactor(void)
{
    return gExtras.espresso && HasBeatenCurrentMansion() ? 1.5f : 1.0f;
}

float ExtrasFallGravityFactor(void)
{
    return gExtras.moonGravity ? 0.4f : 1.0f;
}

Boolean ExtrasFallsAreSafe(void)
{
    return gExtras.moonGravity;
}

float ExtrasMedusaSpeedFactor(void)
{
    return gExtras.medusaLate ? 2.5f : 1.0f;
}

void ExtrasPlayNewGameSound(void)
{
    if (gExtras.myTuba && GetExtraSoundIndex(kExtraSoundMyTuba) >= 0)
        MMPlayExtraSound(kExtraSoundMyTuba);
    else
        MMPlaySound(0x4d, 0, 1);
}
