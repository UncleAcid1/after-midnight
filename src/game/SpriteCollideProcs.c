/*
 * SpriteCollideProcs.c — what happens when sprites touch: the hero dying (and
 * how), picking up objects, reading signs and tips, lava, Medusa heads burning,
 * big spiders catching prey.
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/SpriteCollideProcs.c").
 */
#include "MMGame.h"

#define kFirstLavaTile      0x309
#define kLastLavaSurfaceTile 0x30c

static short gOrigSignHeight, gOrigSignWidth;
static long gSaveGameModeAtSign;
static short gSignScaledSize;

// ---------------------------------------------------------------------------
// Lava

void CheckHeroWithLavaTiles(void)
{
    SpritePtr spriteP = GetHeroSprite();
    HeroSpritePtr heroP = HERO(spriteP);

    if (spriteP == NULL || SWGetSpriteMoveProc(spriteP) == HeroSinksIntoLavaMoveProc)
        return;
    if (!SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckAll, &heroP->collisionInset, 6, 6,
                                kFirstLavaTile, kLastLavaSurfaceTile, false))
        return;

    WriteCStringToFile("Died by lava.\r");
    SetNextDeathCause(kDeathByLava);
    SWSetSpriteDrawProc(spriteP, ClipMainSpriteDrawProc);
    SWSetSpriteMoveProc(spriteP, HeroSinksIntoLavaMoveProc);
    SWSetSpriteFrameRange(spriteP, 51, 54);
    SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, -1);
    CreateSplashForSprite(spriteP, true);
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    KillHeroCommon(spriteP);
    heroP->lavaSurfaceRow = GetLavaPixelRowFromSprite(spriteP) - 3;
    SWSetSpriteMoveDelta(spriteP, 0, 2.3f);
    heroP->lavaSinkFactor = 1.12f;
    MMPlaySound(0x3c, 9, 1);
    MMPlayStereoSound2(NULL, 0x15, 10, 1, (short)(gSoundVolume / 1.5), 1.0, 0);
}

/// Sprites that touch lava sink into it with a splash. With rememberIt, the
/// sprite stays gone when the room is reloaded.
void CheckSpriteWithLavaTiles(SpritePtr spriteP, Boolean rememberIt)
{
    SWRect platformInset, *insetP = NULL;
    Boolean isHero = false;
    short halfWidth;

    SW_ASSERT(spriteP != NULL);
    if (spriteP->userData == kHeroSprite) {
        rememberIt = false;
        isHero = true;
        insetP = &HERO(spriteP)->collisionInset;
        if (spriteP->curFrameIndex == 50)        // lying skeleton
            return;
    }
    if (SWGetSpriteMoveProc(spriteP) == SpriteSinksIntoLavaMoveProc)
        return;

    if (spriteP->userData >= kFirstFallingPlatformSprite && spriteP->userData <= 0x5e) {
            // platforms only sink when their middle is over lava
        halfWidth = (short)(SWGetSpriteWidth(spriteP) / 2.0f);
        SWSetRect(&platformInset, halfWidth - 4, 0, halfWidth + 2, 0);
        insetP = &platformInset;
    }
    if (!SWCheckSpriteWithTiles(gSpriteWorldP, spriteP, kSW_CheckAll, insetP, 6, 6, kFirstLavaTile,
                                kLastLavaSurfaceTile, false))
        return;

    SWSetSpriteDrawProc(spriteP, ClipSpriteWithLavaDrawProc);
    SWSetSpriteMoveProc(spriteP, SpriteSinksIntoLavaMoveProc);
    spriteP->drawData->vertMoveDelta /= 65.0f;
    spriteP->drawData->horizMoveDelta = 0;
    if (spriteP->drawData->vertMoveDelta < 0)
        spriteP->drawData->vertMoveDelta = 0;

    if (isHero && HERO(spriteP)->deathType == kDeathFellTooFar && HERO(spriteP)->attachedSpriteP != NULL)
        SWSetSpriteDrawProc(HERO(spriteP)->attachedSpriteP, ClipSpriteWithLavaDrawProc);

    CreateSplashForSprite(spriteP, isHero ? true : rememberIt);
    if (!rememberIt && !isHero)
        MMPlayStereoSoundAdjustedVol(spriteP, 0x3d, 11, 1, 1400, 1.0);
    else
        MMPlayStereoSoundAdjustedVol(spriteP, 0x3c, 11, 1, 1400, 1.0);
    if (rememberIt)
        AddSpriteToObjectArray(spriteP);

    if (IsSpriteAMedusaHead(spriteP)) {
        SWSetSpriteFrameRange(spriteP, 3, 5);
        if (CountSounds(0x16) < 2)
            MMPlaySound(0x16, 11, 1);
    }
}

Boolean IsSpriteAMedusaHead(SpritePtr spriteP)
{
    return spriteP->userData == kMedusaHeadSprite1 || spriteP->userData == kMedusaHeadSprite2;
}

/// Bodies that are still falling can land in lava too.
void CheckDeadHeroWithLavaTiles(void)
{
    SpritePtr spriteP = NULL;

    while ((spriteP = SWGetNextSprite(gDeadHeroSpriteLayerP, spriteP)) != NULL) {
        SW_ASSERT(spriteP->userData == kHeroSprite);
        if (!HERO(spriteP)->onGround &&
            (HERO(spriteP)->deathType == kDeathCollapsing || HERO(spriteP)->deathType == kDeathElectrocution ||
             HERO(spriteP)->deathType == kDeathFellTooFar))
            CheckSpriteWithLavaTiles(spriteP, false);
    }
}

// ---------------------------------------------------------------------------
// The hero touching things

/// The hero ran into an enemy: a shield saves him (and kills the enemy),
/// otherwise he dies.
void MainSpriteCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    Boolean removeEnemy = true, usedShield;

    (void)sectRect;
    if (SWGetSpriteMoveProc(dstSpriteP) == SpriteSinksIntoLavaMoveProc || HERO(srcSpriteP)->isDead ||
        !SWBestMaskCollision(srcSpriteP, dstSpriteP))
        return;

    if (gObjects.counts[kShieldItem] < 1) {
        SetNextDeathCause(DeathCauseForEnemy(dstSpriteP));
        if (IsSpriteAMedusaHead(dstSpriteP)) {
            KillHeroAndDoStoneCollapse(srcSpriteP);
        }
        else if (dstSpriteP->userData == kSmallSpiderSprite) {
            KillHeroAndDoSmallSpiderDeath(srcSpriteP, dstSpriteP);
            removeEnemy = false;
        }
        else {
            KillHeroAndDoSkeletonCollapse(srcSpriteP, true);
        }
        switch (dstSpriteP->userData) {
            case 1:  WriteCStringToFile("Died by Skull Spider."); break;
            case 2:  WriteCStringToFile("Died by Scorpion."); break;
            case 3:  WriteCStringToFile("Died by Eyeball."); break;
            case 4:  WriteCStringToFile("Died by Blue Raven."); break;
            case 5:  WriteCStringToFile("Died by Red Raven."); break;
            case 6:
            case 7:  WriteCStringToFile("Died by Snake."); break;
            case 8:  WriteCStringToFile("Died by Medusa (non-respawning)."); break;
            case 9:  WriteCStringToFile("Died by Medusa (respawning)."); break;
            case 10: WriteCStringToFile("Died by Small Spider."); break;
            case 12: WriteCStringToFile("Died by Skeleton Fish."); break;
            case 13: WriteCStringToFile("Died by Walking Skeleton."); break;
        }
        WriteCarriageReturn(1);
        usedShield = false;
    }
    else {
        RemoveItemFromStats(kShieldItem);
        gObjects.counts[kShieldItem]--;
        if (gFileIsOpen)
            fprintf(gOutputFileP, "Used shield. %d shields left.\r", gObjects.counts[kShieldItem]);
        if (dstSpriteP->userData == kEyeballSprite)
            MMPlaySound(0x12, 11, 1);
        else if (dstSpriteP->userData == kBlueRavenSprite || dstSpriteP->userData == kRedRavenSprite)
            MMPlaySound(0x31, 11, 1);
        else
            MMPlaySound(0x14, 11, 1);
        if (dstSpriteP->userData == kSmallSpiderSprite)
            DeleteWebForSmallSpider(dstSpriteP);
        usedShield = true;
    }

    if (removeEnemy) {
        CoopPostEnemyKilled(dstSpriteP);
        SWRemoveSpriteFromAnimation(dstSpriteP, true);
            // respawning Medusas come back if killed by a shield
        if (dstSpriteP->userData != kMedusaHeadSprite2 || !usedShield)
            AddSpriteToObjectArray(dstSpriteP);
        SWCenterSpriteOverSprite(NewPuffOfSmokeSprite(), dstSpriteP);
    }
}

void ObjectSpriteCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    SW_ASSERT(srcSpriteP->userData >= kFirstObjectSprite);
    SW_ASSERT(srcSpriteP->userData <= kLastObjectSprite);
    SW_ASSERT(dstSpriteP->userData == kHeroSprite);
    if (!HERO(dstSpriteP)->isDead && SWBestMaskCollision(srcSpriteP, dstSpriteP))
        GetObject(srcSpriteP);
}

void SecretSpriteCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    SW_ASSERT(srcSpriteP->userData == kSecretIndicatorSprite);
    SW_ASSERT(dstSpriteP->userData == kHeroSprite);
    if (!HERO(dstSpriteP)->isDead) {
        gNumSecretsFound++;
        MMPlaySound(0x4e, 8, 1);
        WriteCStringToFile("Found secret!\r");
        SWRemoveSpriteFromAnimation(srcSpriteP, true);
        AddSpriteToObjectArray(srcSpriteP);
    }
}

void HeroExitCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    SW_ASSERT(srcSpriteP->userData == kHeroSprite);
    SW_ASSERT(dstSpriteP->userData <= kExitSprite);
    if (!gUsingEditor)
        gBeatTheMansion = true;
}

/// Reading a sign: its picture zooms in with the text drawn over it, and the
/// game pauses until it is dismissed.
void HeroSignCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    ObjectSpritePtr signP = OBJECT(dstSpriteP);
    SpritePtr signPictP, textP;
    SWRect rect, textRect;
    SWRGBColor textColor = { 0xf000, 0xf000, 0xf000 };
    MMTextStyleRec style;
    char errorText[256];
    char *text;
    SWError err;

    (void)sectRect;
    SW_ASSERT(srcSpriteP->userData == kHeroSprite);
    if (SWGetNextSprite(gSignContentsSpriteLayerP, NULL) != NULL || signP->isTip)
        return;

    err = CreateSpriteFromPictResource(gSpriteWorldP, &signPictP, NULL, signP->signPictResID,
                                       signP->signPictResID, 1, 4);
    if (err != kNoError)
        return;

    gOrigSignHeight = (short)SWGetSpriteHeight(signPictP);
    gOrigSignWidth = (short)SWGetSpriteWidth(signPictP);
    StopAllChannels();
    MMPlaySound(0x40, 9, 1);
    IsMusicPlaying();
    SWLockSprite(signPictP);
    SWAddSprite(gSignContentsSpriteLayerP, signPictP);

    rect = signPictP->destFrameRect;
    SWCenterRect(&rect, &gSpriteWorldP->visScrollRect);
    SWSetSpriteLocation(signPictP, rect.left, rect.top);
    SW_ASSERT(rect.left < rect.right);
    SW_ASSERT(rect.top < rect.bottom);

    textRect = rect;
    SWInsetRect(&textRect, 28, 28);
    memset(&style, 0, sizeof(style));
    if (!FontExists("Chalkboard")) {
        SetPascalString(style.fontName, (ConstStringPtr)"\x12" "Comic Sans MS Bold");
        style.size = 14;
    }
    else {
        SetPascalString(style.fontName, (ConstStringPtr)"\x0a" "Chalkboard");
        style.size = 15;
    }
    style.style = 1;
    style.unused = true;
    style.dropShadow = true;
    style.justification = 0;

    text = MMGetCStringFromTEXTResource(signP->signTextResID);
    if (text == NULL) {
        snprintf(errorText, sizeof(errorText),
                 "Error: The text for this sign could not be found. Please make sure there is a file in the "
                 "mansion's 'TEXT' folder that is named %05d.txt", signP->signTextResID);
    }
    textP = MyDrawTextBox(gSpriteWorldP, text != NULL ? text : errorText, &style, &textRect, &textColor,
                          gCustomDialogSpriteLayerP, true);
    signPictP->misc1.sprite = textP;
    if (textP != NULL) {
        SWRemoveSprite(textP);
        SWAddSprite(gSignContentsSpriteLayerP, textP);
    }
    free(text);

    ResetKeys();
    WriteEventToMovieFile(11, 1);
    gReadingSignWasDismissed = false;
    gSaveGameModeAtSign = gCurrentGameMode;
    gCurrentGameMode = kReadingSignMode;
    HERO(srcSpriteP)->actionKeyFrames = 0;
    gSignScaledSize = 0;
    SWSetSpriteVisible(signPictP, false);
    SWSetSpriteFrameProc(signPictP, SignScaleInFrameProc);
    SWSetSpriteFrameTime(signPictP, 0);
    PauseAllSpriteLayers(gSpriteWorldP);
    SWUnpauseSpriteLayer(gSignContentsSpriteLayerP);
}

/// Tips are signs that show their text (in the bitmap font) while the hero is near.
void HeroTipCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    ObjectSpritePtr tipP = OBJECT(dstSpriteP);
    SpritePtr textP;
    SWRect rect;
    unsigned short width;
    char *text;

    (void)sectRect;
    SW_ASSERT(srcSpriteP->userData == kHeroSprite);
    SW_ASSERT(dstSpriteP->userData == kSignObjectSprite);
    if (!tipP->isTip || !HERO(srcSpriteP)->onGround)
        return;

    textP = SWGetNextSprite(gTipContentsSpriteLayerP, NULL);
    if (textP != NULL) {
        if (textP->userData == kBMTextSpriteMagic && textP->misc2.sprite == dstSpriteP)
            return;                     // already showing this tip
        SWDisposeAllSpritesInLayer(gTipContentsSpriteLayerP);
    }

    switch (tipP->signPictResID) {
        case 0x2008: width = 240; break;
        case 0x2009: width = 320; break;
        case 0x200a: width = 380; break;
        default:     width = 510; break;
    }
    rect.top = 0;
    rect.left = 0;
    rect.bottom = width / 2;
    rect.right = width;
    SWCenterRect(&rect, &gSpriteWorldP->visScrollRect);

    text = MMGetCStringFromTEXTResource(tipP->signTextResID);
    if (text != NULL && (textP = NewBitmapTextSprite(width)) != NULL) {
        SetBMTextSpriteText(textP, text);
        SWSetSpriteLocation(textP, rect.left, rect.top);
        SWAddSprite(gTipContentsSpriteLayerP, textP);
        SWSetSpriteScaledFactor(textP, 0.32f, 0.32f);
        SWSetSpriteMoveProc(textP, TipSpriteMoveProc);
        textP->misc1.n = 0;
        textP->misc2.sprite = dstSpriteP;
        SWSetSpriteTranslucency(textP, 0);
    }
    free(text);
}

void HandleReadingSignCallback(void)
{
    if (NetMustWait())                  // two computers: waiting for the next frame
        return;
    SWProcessSpriteWorld(gSpriteWorldP);
    SWAnimateSpriteWorld(gSpriteWorldP);
        // two computers: the sign closes after a frame on both
    if (gReadingSignWasDismissed && (!NetGameActive() || gSpriteWorldP->frameHasOccurred))
        ExitSignReadingIfPossible();
}

/// Only once the sign has fully zoomed in.
void ExitSignReadingIfPossible(void)
{
    if (gSignScaledSize < 100)
        gReadingSignWasDismissed = false;
    else
        CleanUpAfterSign();
}

void CleanUpAfterSign(void)
{
    SpritePtr signPictP = SWGetNextSprite(gSignContentsSpriteLayerP, NULL);

    if (signPictP == NULL) {
        gCurrentGameMode = kPlayingGameMode;
        return;
    }
    gSignScaledSize = 100;
    SWSetSpriteFrameProc(signPictP, SignScaleOutFrameProc);
    SWSetSpriteFrameTime(signPictP, 0);
    UnpauseAllSpriteLayers(gSpriteWorldP);
    gCurrentGameMode = gSaveGameModeAtSign;
    gKeys[kKeyAction] = false;
}

/// Scales the sign (and its text) to gSignScaledSize percent, centered.
static void ScaleSign(SpritePtr signPictP)
{
    SpritePtr textP = signPictP->misc1.sprite;
    SWRect rect, visRect;
    float scale = gSignScaledSize / 100.0f;

    SWSetSpriteScaledWidthFactor(signPictP, scale);
    SWSetSpriteScaledHeightFactor(signPictP, scale);
    rect = signPictP->destFrameRect;
    SWCenterRect(&rect, &gSpriteWorldP->visScrollRect);
    SWMoveSprite(signPictP, rect.left, rect.top);
    if (textP != NULL) {
        SWSetSpriteScaledWidthFactor(textP, scale);
        SWSetSpriteScaledHeightFactor(textP, scale);
        visRect = gSpriteWorldP->visScrollRect;     // (window pixels in the original; design units here)
        rect = textP->destFrameRect;
        SWCenterRect(&rect, &visRect);
        SWMoveSprite(textP, rect.left, rect.top);
    }
}

void SignScaleInFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    if (!spriteP->isVisible)
        SWSetSpriteVisible(spriteP, true);
    gSignScaledSize += 5;
    if (gSignScaledSize > 100)
        gSignScaledSize = 100;
    ScaleSign(spriteP);
    if (gSignScaledSize >= 100) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
    }
}

void SignScaleOutFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    gSignScaledSize -= 5;
    ScaleSign(spriteP);
    if (gSignScaledSize < 2) {
        if (spriteP->misc1.sprite != NULL)
            SWRemoveSpriteFromAnimation(spriteP->misc1.sprite, true);
        SWRemoveSpriteFromAnimation(spriteP, true);
    }
}

// ---------------------------------------------------------------------------
// Enemies touching things

/// A Medusa head touching a flame (torch) catches fire and falls.
void MedusaCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    MedusaSpritePtr medusaP = MEDUSA(srcSpriteP);
    short channel;

    (void)sectRect;
    if (!SWBestMaskCollision(srcSpriteP, dstSpriteP))
        return;
    CoopPostEnemyKilled(srcSpriteP);
    SWRemoveSprite(srcSpriteP);
    SWAddSprite(gDyingMedusaSpriteLayerP, srcSpriteP);
    SWSetSpriteMoveProc(srcSpriteP, MedusaOnFireMoveProc);
    medusaP->frameDelay = 4;
    medusaP->frameTimer = 4;
    if (srcSpriteP->userData == kMedusaHeadSprite1)
        AddSpriteToObjectArray(srcSpriteP);
    medusaP->fallSpeed = 0.005f;
    medusaP->fallAccel = 0;

    channel = FindSound(11);
    if (channel == -1)
        channel = FindSound(12);
    if (channel != -1)
        StopChannel(channel);
    if (CountSounds(0x16) < 2)
        MMPlaySound(0x16, 11, 1);
}

/// A big spider dropping onto the hero, a raven, a Medusa head or an eyeball.
void BigSpiderCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    SpiderSpritePtr spiderP = SPIDER(srcSpriteP);
    long type = dstSpriteP->userData;

    (void)sectRect;
    if (spiderP->preyP != NULL || !SWBestMaskCollision(dstSpriteP, srcSpriteP))
        return;

    if (type == kHeroSprite) {
        spiderP->state = kSpiderClimbing;
        if (!HERO(dstSpriteP)->isDead) {
            KillHeroOnBigSpider(dstSpriteP, srcSpriteP);
            MMPlaySound(0x15, 11, 1);
        }
        return;
    }
    if (type != kRedRavenSprite && type != kBlueRavenSprite && !IsSpriteAMedusaHead(dstSpriteP) &&
        type != kEyeballSprite)
        return;

    spiderP->state = kSpiderClimbing;
    SWRemoveSpriteFromAnimation(dstSpriteP, true);
    AddSpriteToObjectArray(srcSpriteP);
    AddSpriteToObjectArray(dstSpriteP);
    spiderP->preyP = NewWebTrappedSprite(kWebTrappedEnemySprite);
    MovePreyToSpiderLocation(spiderP->preyP, srcSpriteP);
    if (type == kRedRavenSprite || type == kBlueRavenSprite)
        MMPlaySound(0x31, 11, 1);
    else if (IsSpriteAMedusaHead(dstSpriteP)) {
        if (CountSounds(0x16) < 2)
            MMPlaySound(0x16, 11, 1);
    }
    else if (type == kEyeballSprite)
        MMPlaySound(0x12, 11, 1);
}

void ZapperBeamSpriteCollideProc(SpritePtr srcSpriteP, SpritePtr dstSpriteP, SWRect *sectRect)
{
    (void)sectRect;
    SW_ASSERT(dstSpriteP->userData == kHeroSprite);
    if (srcSpriteP->isVisible && !HERO(dstSpriteP)->isDead && SWBestMaskCollision(srcSpriteP, dstSpriteP)) {
        WriteCStringToFile("Died by zapper beam.\r");
        KillHeroAndDoElectrocutionDeath(dstSpriteP);
    }
}

// ---------------------------------------------------------------------------
// Deaths

void KillHero(SpritePtr spriteP)
{
    KillHeroCommon(spriteP);
    SWRemoveSpriteFromAnimation(spriteP, true);
    gRestartLevel = true;
}

/// What killed Jack, for the lifetime stats; set just before the death is processed.
static int gNextDeathCause = kDeathByOther;

void SetNextDeathCause(int cause)
{
    gNextDeathCause = cause;
}

/// A lever the hero was pulling springs back.
void KillHeroCommon(SpritePtr spriteP)
{
    SpritePtr leverP = HERO(spriteP)->leverP;

    if (!HERO(spriteP)->isDead) {
        StatsNoteDeath(gNextDeathCause);
        ExtrasHeroDied(spriteP);
    }
    gNextDeathCause = kDeathByOther;
    HERO(spriteP)->isDead = true;
    StopChannel(4);
    if (leverP != NULL) {
        SW_ASSERT(leverP->userData >= kFirstLeverSprite);
        SW_ASSERT(leverP->userData <= kLastLeverSprite);
        SWSetCurrentFrameIndex(leverP, leverP->frameAdvance == -1 ? 3 : 0);
    }
}

/// Bitten: Jack's skeleton falls apart (frames 42-49).
void KillHeroAndDoSkeletonCollapse(SpritePtr spriteP, Boolean playSound)
{
    if (playSound)
        MMPlaySound(9, 10, 1);
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    SWSetSpriteFrameRange(spriteP, 42, 49);
    SWSetSpriteFlipX(spriteP, !HERO(spriteP)->facingRight);
    SWSetCurrentFrameIndex(spriteP, 42);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteMoveProc(spriteP, DeadHeroMoveProc);
    SWSetSpriteFrameProc(spriteP, NULL);
    SWSetSpriteFrameTime(spriteP, 0);
    HERO(spriteP)->collapseTimer = 20;
    HERO(spriteP)->deathType = kDeathSkeletonCollapse;
    gHeroLandedThisFrame = false;
    KillHeroCommon(spriteP);
}

/// Turned to stone by a Medusa head (frames 59-67).
void KillHeroAndDoStoneCollapse(SpritePtr spriteP)
{
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    SWSetSpriteFrameRange(spriteP, 59, 67);
    SWSetSpriteFlipX(spriteP, !HERO(spriteP)->facingRight);
    SWSetCurrentFrameIndex(spriteP, 59);
    SWSetSpriteFrameAdvance(spriteP, 0);
    MySetSpriteFrameTime(spriteP, 0);
    SWSetSpriteMoveProc(spriteP, DeadHeroMoveProc);
    SWSetSpriteFrameProc(spriteP, NULL);
    SWResetSpriteFrameTimer(gSpriteWorldP, spriteP);
    HERO(spriteP)->deathType = kDeathStoneCollapse;
    gHeroLandedThisFrame = false;
    KillHeroCommon(spriteP);
}

void KillHeroAndDoElectrocutionDeath(SpritePtr spriteP)
{
    HeroSpritePtr heroP = HERO(spriteP);

    MMPlaySound(0x43, 10, 1);
    SetNextDeathCause(kDeathByZapper);
    Rumble(kRumbleJolt);
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    SWSetSpriteFrameRange(spriteP, 55, 56);
    SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    SWSetCurrentFrameIndex(spriteP, 56);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameProc(spriteP, ElectrocutionDeathFrameProc);
    SWSetSpriteMoveProc(spriteP, MoveWhenNotTouchingZapperBeam);
    SWSetSpriteFrameTime(spriteP, 0);
    SWSetSpriteMoveTime(spriteP, 0);
    heroP->animTimer = 9;
    heroP->electrocutionTime = 0;
    heroP->flickerCount = 0;
    heroP->deathType = kDeathElectrocution;
    gHeroLandedThisFrame = false;
}

/// A small spider wraps Jack up and carries him off (frames 81-83).
void KillHeroAndDoSmallSpiderDeath(SpritePtr spriteP, SpritePtr spiderSpriteP)
{
    SW_ASSERT(spriteP->userData == kHeroSprite);
    SW_ASSERT(spiderSpriteP->userData == kSmallSpiderSprite);
    MMPlaySound(0x15, 10, 1);
    SetNextDeathCause(kDeathBySpider);
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    SWSetSpriteFrameRange(spriteP, 81, 83);
    SWSetSpriteFlipX(spriteP, false);
    SWSetCurrentFrameIndex(spriteP, 81);
    SWSetSpriteFrameAdvance(spriteP, 1);
    SWSetSpriteFrameAdvanceMode(spriteP, kPatrollingMode);
    MySetSpriteFrameTime(spriteP, 500);
    SWSetSpriteMoveProc(spriteP, NULL);
    SWSetSpriteFrameProc(spriteP, NULL);
    SWSetSpriteMoveProc(spiderSpriteP, SmallSpiderDragsHeroMoveProc);
    SPIDER(spiderSpriteP)->preyP = spriteP;
    AddSpriteToObjectArray(spiderSpriteP);
    HERO(spriteP)->deathType = kDeathSmallSpider;
    LayWebUpToCeiling(spiderSpriteP, 8, 4, 0x5a0);
    KillHeroCommon(spriteP);
}

void KillHeroOnBigSpider(SpritePtr spriteP, SpritePtr spiderSpriteP)
{
    SpritePtr trappedP;

    WriteCStringToFile("Died by big spider.\r");
    SetNextDeathCause(kDeathBySpider);
    KillHeroCommon(spriteP);
    SWRemoveSpriteFromAnimation(spriteP, true);
    trappedP = NewWebTrappedSprite(kWebTrappedHeroSprite);
    MultiplayerNoteWebVictim(spiderSpriteP);
    AddSpriteToObjectArray(spiderSpriteP);
    AddSpriteToDoOnceArray(spiderSpriteP);
    SPIDER(spiderSpriteP)->preyP = trappedP;
    MovePreyToSpiderLocation(trappedP, spiderSpriteP);
}

void KillHeroForFallingTooFar(SpritePtr spriteP)
{
    MMPlaySound(0x20, 10, 1);
    WriteCStringToFile("Died by falling too far.\r");
    SetNextDeathCause(kDeathByFall);
    SWRemoveSprite(spriteP);
    SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    SWSetSpriteFrameRange(spriteP, 38, 41);
    SWSetSpriteFlipX(spriteP, !HERO(spriteP)->facingRight);
    SWSetCurrentFrameIndex(spriteP, 38);
    SWSetSpriteFrameAdvance(spriteP, 0);
    SWSetSpriteFrameTime(spriteP, 0);
    HERO(spriteP)->animTimer = 5;
    SWSetSpriteFrameProc(spriteP, FellTooFarFrameProc);
    SWSetSpriteMoveProc(spriteP, DeadHeroMoveProc);
    HERO(spriteP)->deathType = kDeathFellTooFar;
    KillHeroCommon(spriteP);
}

// ---------------------------------------------------------------------------
// Objects

void GetObject(SpritePtr spriteP)
{
    short type = (short)spriteP->userData;
    Boolean gotIt = false;
    static const char *const kKeyMessages[] = {
        "Got red key.\r", "Got blue key.\r", "Got green key.\r", "Got purple key.\r",
        "Got yellow key.\r", "Got master key.\r"
    };

    switch (type) {
        case 0x1e: gScore += 100; gotIt = true; break;
        case 0x1f: gScore += 200; gotIt = true; break;
        case 0x20: gScore += 50; gotIt = true; break;
        case 0x21: gScore += 25; gotIt = true; break;

        case 0x22:
            gotIt = AddItemToStats(kShieldItem);
            if (gotIt) {
                gObjects.counts[kShieldItem]++;
                WriteCStringToFile("Got shield.\r");
            }
            break;

        case 0x23:
            gotIt = true;
            if (!gObjects.hasBackpack) {
                AddBackpackToStatsArea();
                WriteCStringToFile("Got backpack.\r");
            }
            else {
                gScore += 500;
            }
            break;

        case kLanternObjectSprite:
            if (!gObjects.hasLantern) {
                gotIt = true;
                GetLantern(spriteP);
                WriteCStringToFile("Got lantern.\r");
                if (OBJECT(spriteP)->glowSpriteP != NULL)
                    SWRemoveSpriteFromAnimation(OBJECT(spriteP)->glowSpriteP, true);
            }
            break;

        case 0x25:
            WriteCStringToFile("Got map item.\r");
            MMPlayStereoSound2(NULL, 0x27, 8, 1, gSoundVolume, 2.0, 0);
            RevealGridInMap(OBJECT(spriteP)->mapGridH, OBJECT(spriteP)->mapGridV);
            gotIt = true;
            break;

        case 0x27: case 0x28: case 0x29: case 0x2a: case 0x2b: case 0x2c:
            gotIt = AddItemToStats(type - 0x27);
            if (gotIt) {
                gObjects.counts[type - 0x27]++;
                WriteCStringToFile(kKeyMessages[type - 0x27]);
            }
            break;

        default:
            SW_ASSERT(false);
            break;
    }

    if (!gotIt)
        return;

    if (type >= 0x1e && type <= 0x21)
        StatsNoteCoin();
    Rumble(kRumbleTap);
    if (type >= 0x27 && type <= 0x2c)
        MMPlaySound(0, 8, 1);
    else if (type == 0x22)
        MMPlaySound(0x13, 8, 1);
    else if (type == 0x1e || type == 0x1f)
        MMPlaySound(0x19, 8, 1);
    else if (type == 0x23)
        MMPlaySound(0x27, 8, 1);
    else if (type == 0x21 || type == 0x20)
        MMPlaySound(0x37, 8, 3);
    else if (type == kLanternObjectSprite)
        MMPlaySound(0x3e, 8, 3);

    SWRemoveSpriteFromAnimation(spriteP, true);
    AddSpriteToObjectArray(spriteP);
}

/// With the lantern, dark rooms are lit: redraw the room with everything visible.
void GetLantern(SpritePtr spriteP)
{
    SpriteLayerPtr layerP;
    SpritePtr otherP;
    Boolean roomWasDark;
    short type;

    (void)spriteP;
    if (gObjects.hasLantern)
        return;
    gObjects.hasLantern = true;
    roomWasDark = gRoomIsDark;
    AddLanternToStatsArea();
    DisposeAllMapCachedRooms();
    if (!roomWasDark)
        return;

    MMLoadExtraBackFrameForRoom(gSpriteWorldP, GEGetLevelResID(gCurrentLevel, 1, 200));
    MakeRoomDarkOrLight(false, true);
    for (layerP = SWGetNextSpriteLayer(gSpriteWorldP, NULL); layerP != NULL;
         layerP = SWGetNextSpriteLayer(gSpriteWorldP, layerP)) {
        for (otherP = SWGetNextSprite(layerP, NULL); otherP != NULL; otherP = SWGetNextSprite(layerP, otherP)) {
            type = (short)otherP->userData;
            if (type == kPlatformBlockerSprite || type == kSecretIndicatorSprite || type == 0xb9 || type == 0xba)
                continue;
            if (type >= kFirstDoorSprite && type <= kLastDoorSprite)
                SWSetSpriteTranslucency(otherP, 0xffff);
            SWSetSpriteVisible(otherP, true);
        }
    }
    DrawLevel();
}

/// Two splash halves (back and front) at the lava surface under the sprite.
void CreateSplashForSprite(SpritePtr spriteP, Boolean bigSplash)
{
    short type = bigSplash ? 0xce : 0xd0, lavaRow = GetLavaPixelRowFromSprite(spriteP);
    SpritePtr splashP;

    splashP = NewLavaSplashSprite(type);
    SWCenterSpriteOverSprite(splashP, spriteP);
    SWMoveSprite(splashP, SWGetSpriteHorizLoc(splashP), lavaRow - SWGetSpriteHeight(splashP));
    splashP = NewLavaSplashSprite(type + 1);
    SWCenterSpriteOverSprite(splashP, spriteP);
    SWMoveSprite(splashP, SWGetSpriteHorizLoc(splashP), lavaRow - SWGetSpriteHeight(splashP));
}

/// The top pixel row of the first lava tile under the sprite.
short GetLavaPixelRowFromSprite(SpritePtr spriteP)
{
    short row, col, lastRow, lastCol, tile;

    lastRow = (short)((spriteP->destFrameRect.bottom < 0 ? spriteP->destFrameRect.bottom + 15 : spriteP->destFrameRect.bottom) >> 4);
    lastCol = (short)((spriteP->destFrameRect.right < 0 ? spriteP->destFrameRect.right + 15 : spriteP->destFrameRect.right) >> 4);
    for (row = (short)(((spriteP->destFrameRect.top < 0 ? spriteP->destFrameRect.top + 15 : spriteP->destFrameRect.top) >> 4) - 1);
         row <= lastRow; row++) {
        for (col = (short)(((spriteP->destFrameRect.left < 0 ? spriteP->destFrameRect.left + 15 : spriteP->destFrameRect.left) >> 4) - 1);
             col <= lastCol; col++) {
            tile = MyGetTile(gSpriteWorldP, 6, row, col);
            if (tile > 0x308 && tile < 0x331)
                return (short)(row * 16 - 1);
        }
    }
    return (short)(row * 16 - 1);
}
