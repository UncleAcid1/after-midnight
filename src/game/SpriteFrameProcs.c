/*
 * SpriteFrameProcs.c — frame procs: called when a sprite's frame changes, they
 * drive animations (idling, deaths, save statues, fades, lightning...).
 *
 * Reconstructed from Midnight Mansion HD 1.0.1b ("Sources & Headers/SpriteFrameProcs.c").
 */
#include "MMGame.h"

/// Steps the hero's animation every frameDelay frames.
void HeroMiscFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (heroP->frameTimer-- < 1) {
        heroP->frameTimer = heroP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &heroP->frameDirection);
    }
}

/// Jack standing still: blinks, looks around, scratches, yawns and falls asleep.
/// Frames: 68 looking ahead, 69-71 blink/look, 72-74 yawn, 75-80 scratch.
void IdleHeroFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);
    short next = -1;

    (void)curFrameP;
    switch (heroP->idleState) {
        case 0:
            *frameIndex = 68;
            SWSetSpriteFrameTime(spriteP, 142);
            SWSetSpriteFrameAdvance(spriteP, 0);
            heroP->blinkTimer = GetRandom(4, 12);
            heroP->lookTimer = GetRandom(6, 20);
            heroP->sleepTimer = GetRandom(30, 200);
            heroP->idleState = 1;
            heroP->facingRight = true;
            break;

        case 1:
            *frameIndex = 69;
            SWSetSpriteFrameTime(spriteP, 200);
            if (heroP->blinkTimer-- < 1) {
                *frameIndex = 70;
                heroP->blinkTimer = GetRandom(4, 12);
            }
            if (heroP->lookTimer-- < 1)
                next = heroP->sleepTimer < 1 ? 4 : 2;
            break;

        case 2:
            SWSetSpriteFrameTime(spriteP, 250);
            *frameIndex = *frameIndex == 71 ? 69 : 71;
            if (heroP->lookAroundTimer-- < 1)
                next = 1;
            if (heroP->blinkTimer-- < 1 && *frameIndex == 69) {
                *frameIndex = 70;
                heroP->blinkTimer = GetRandom(4, 12);
            }
            if (GetRandom(1, 15) == 1)
                next = 3;
            break;

        case 3:
            if (*frameIndex == 75 && heroP->scratchCount-- < 2) {
                SWSetSpriteFrameAdvance(spriteP, 0);
                next = 6;
            }
            break;

        case 6:
            next = 1;
            break;

        case 4:
            if (*frameIndex == 74) {
                    // asleep
                SWSetSpriteFrameAdvance(spriteP, 0);
                SWSetSpriteFrameTime(spriteP, 909);
                heroP->idleState = 5;
            }
            break;

        case 5:
            MakeZzzzFromJacksHead(spriteP);
            break;
    }
    heroP->sleepTimer--;

    switch (next) {
        case 1:
            heroP->idleState = 1;
            heroP->lookTimer = GetRandom(6, 20);
            heroP->blinkTimer = GetRandom(4, 12);
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameRange(spriteP, 69, 71);
            SWSetCurrentFrameIndex(spriteP, 69);
            break;
        case 2:
            heroP->idleState = 2;
            heroP->lookAroundTimer = GetRandom(12, 24);
            heroP->blinkTimer = GetRandom(4, 12);
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameRange(spriteP, 69, 71);
            SWSetCurrentFrameIndex(spriteP, 71);
            break;
        case 3:
            heroP->idleState = 3;
            SWSetSpriteFrameAdvance(spriteP, 1);
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
            SWSetSpriteFrameTime(spriteP, 125);
            SWSetSpriteFrameRange(spriteP, 75, 80);
            SWSetCurrentFrameIndex(spriteP, 75);
            heroP->scratchCount = GetRandom(4, 10);
            break;
        case 6:
            heroP->idleState = 6;
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameTime(spriteP, 500);
            break;
        case 4:
            heroP->idleState = 4;
            SWSetSpriteFrameAdvance(spriteP, 1);
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
            SWSetSpriteFrameTime(spriteP, 333);
            SWSetSpriteFrameRange(spriteP, 72, 74);
            SWSetCurrentFrameIndex(spriteP, 72);
            break;
    }
}

// ---------------------------------------------------------------------------
// Deaths

void SkeletonCollapseFrameProc1(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    if (HERO(spriteP)->collapseTimer-- < 1) {
        HERO(spriteP)->collapseTimer = 10;
        SWSetSpriteFrameProc(spriteP, SkeletonCollapseFrameProc2);
    }
}

/// The skeleton falls apart, each frame a bit faster (frames 42-49).
void SkeletonCollapseFrameProc2(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP;
    if (heroP->collapseTimer-- >= 1)
        return;
    (*frameIndex)++;
    heroP->collapseTimer = (short)((*frameIndex - 42) / -1.7 + 9.0);
    if (*frameIndex == 44)
        MMPlaySound(8, 9, 1);
    if (*frameIndex == 49) {
        MySetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameProc(spriteP, NULL);
        if (!heroP->deathAnimIsDone) {
            gRestartLevel = true;
            heroP->deathAnimIsDone = true;
        }
    }
}

/// Turned to stone (by a Medusa head): frames 59-67.
void StoneCollapseFrameProc1(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP;
    if (heroP->collapseTimer-- >= 1)
        return;
    (*frameIndex)++;
    heroP->collapseTimer = 12;
    if (*frameIndex == 61) {
        SWSetSpriteFrameProc(spriteP, StoneCollapseFrameProc2);
        heroP->collapseTimer = 30;
    }
}

void StoneCollapseFrameProc2(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    if (HERO(spriteP)->collapseTimer-- < 1) {
        HERO(spriteP)->collapseTimer = 10;
        SWSetSpriteFrameProc(spriteP, StoneCollapseFrameProc3);
        MMPlaySound(0x35, 9, 1);
    }
}

void StoneCollapseFrameProc3(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP;
    if (heroP->collapseTimer-- >= 1)
        return;
    (*frameIndex)++;
    heroP->collapseTimer = (short)((*frameIndex - 61) / -1.7 + 10.0);
    if (*frameIndex == 67) {
        MySetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameProc(spriteP, NULL);
        heroP->deathAnimIsDone = true;
        gRestartLevel = true;
    }
}

/// Zapped: flickers between two pairs of frames (55-56 and 57-58), then collapses.
void ElectrocutionDeathFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP;
    if (heroP->animTimer-- >= 1)
        return;
    heroP->animTimer = 9;

    if (*frameIndex == 55) {
        *frameIndex = 56;
    }
    else if (*frameIndex == 57) {
        *frameIndex = 58;
    }
    else if (*frameIndex == 56) {
        if (heroP->flickerCount++ < 1) {
            *frameIndex = 55;
        }
        else {
            heroP->flickerCount = 0;
            SWSetSpriteFrameRange(spriteP, 57, 58);
            SWSetCurrentFrameIndex(spriteP, 58);
        }
    }
    else if (*frameIndex == 58) {
        if (heroP->flickerCount++ < 1) {
            *frameIndex = 57;
        }
        else {
            heroP->flickerCount = 0;
            SWSetSpriteFrameRange(spriteP, 55, 56);
            SWSetCurrentFrameIndex(spriteP, 56);
        }
    }

    if (heroP->electrocutionTime++ > 10) {
        if (SWGetSpriteMoveProc(spriteP) == SpriteSinksIntoLavaMoveProc) {
            KillHeroCommon(spriteP);
            SWSetSpriteFrameProc(spriteP, NULL);
            gRestartLevel = true;
        }
        else {
            KillHeroAndDoSkeletonCollapse(spriteP, false);
            SWSetSpriteMoveDelta(spriteP, 0, 0);
            heroP->deathAnimIsDone = true;
            gRestartLevel = true;
        }
    }
}

void BlinkingSpriteFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    SWSetSpriteVisible(spriteP, !spriteP->isVisible);
}

void HeroLandedFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (heroP->animTimer-- >= 1)
        return;
    heroP->isLanding = false;
    SWSetSpriteFrameProc(spriteP, NULL);
    if (SWGetSpriteMoveProc(spriteP) == DeadHeroMoveProc)
        SWSetSpriteMoveProc(spriteP, MainSpriteMoveProc);
    if (heroP->onGround)
        SetHeroGroundFrame(spriteP);
}

/// Fell too far: frames 38-41, then a skeleton is left lying there while the
/// body fades away.
void FellTooFarFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);
    SpritePtr skeletonP, lastP;

    (void)curFrameP;
    if (heroP->animTimer-- >= 1)
        return;
    (*frameIndex)++;
    heroP->animTimer = 5;
    if (*frameIndex != 41)
        return;

    skeletonP = NewMainSprite();
    if (skeletonP != NULL) {
        SWRemoveSprite(skeletonP);
        lastP = SWGetPreviousSprite(gDeadHeroSpriteLayerP, NULL);
        if (lastP == NULL)
            SWAddSprite(gDeadHeroSpriteLayerP, skeletonP);
        else
            SWInsertSpriteBeforeSprite(skeletonP, lastP);
        HERO(skeletonP)->onGround = false;
        HERO(skeletonP)->isDead = true;
        HERO(skeletonP)->deathType = kDeathFellTooFar;
        HERO(skeletonP)->deathAnimIsDone = true;
        SWSetSpriteFrameRange(skeletonP, 50, 50);
        SWSetSpriteFlipX(skeletonP, !heroP->facingRight);
        SWSetSpriteFrameProc(skeletonP, NULL);
        SWSetSpriteFrameTime(skeletonP, -1);
        SWSetSpriteMoveProc(skeletonP, LyingSkeletonMoveProc);
        SWSetSpriteLocation(skeletonP, spriteP->destFrameRect.left, spriteP->destFrameRect.top);
        HERO(skeletonP)->deadBodyP = spriteP;
        heroP->attachedSpriteP = skeletonP;
    }
    SWSetSpriteDrawProc(spriteP, gTranslucentMaskDrawProc);
    SWSetSpriteFrameProc(spriteP, DeadBodyFadeFrameProc);
    SWSetSpriteFrameAdvance(spriteP, 0);
    heroP->animTimer = 10;
    MMPlayStereoSound2(NULL, 0x20, 10, 1, gSoundVolume / 2, 0.9, 0);
}

/// Statues of Jack (frames 39-41) flinch when the hero lands right next to them.
void FlinchOccasionallyFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SpritePtr heroP;
    short dv;

    (void)curFrameP;
    if (spriteP->frameAdvance == 1) {
        if (*frameIndex == 41) {
            SWSetSpriteFrameAdvance(spriteP, 0);
            SWSetSpriteFrameTime(spriteP, 0);
        }
        return;
    }
    heroP = GetHeroSprite();
    if (gHeroLandedThisFrame && heroP != NULL) {
        dv = (short)(SWGetSpriteVertLoc(spriteP) - SWGetSpriteVertLoc(heroP));
        if (dv > -8 && dv < 8 && SWBestMaskCollision(spriteP, heroP)) {
            SWSetCurrentFrameIndex(spriteP, 39);
            SWSetSpriteFlipX(spriteP, !HERO(spriteP)->facingRight);
            SWSetSpriteFrameAdvance(spriteP, 1);
            MySetSpriteFrameTime(spriteP, 76);
        }
    }
}

void RandomFlinchFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)spriteP; (void)curFrameP;
    *frameIndex = GetRandom(1, 35) == 10 ? 1 : 0;
}

// ---------------------------------------------------------------------------
// Lightning

void LightningFlashFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MiscSpritePtr miscP = MISC(spriteP);

    (void)curFrameP;
    if (!spriteP->isVisible) {
        if (GetRandom(1, 45) == 7) {
            playThunderSound(spriteP);
            SWSetSpriteVisible(spriteP, true);
            SWSetSpriteFrameAdvance(spriteP, 1);
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
        }
        return;
    }
    if (*frameIndex == 2 && !miscP->flashedTwice) {
        miscP->flashedTwice++;
        *frameIndex = 1;
    }
    if (*frameIndex == 0) {
        SWSetSpriteVisible(spriteP, false);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetCurrentFrameIndex(spriteP, 0);
        miscP->flashedTwice = false;
    }
}

/// The title screen's lightning: a few flashes, during which Jack (in the hero
/// layer) is drawn lit up, then a pause.
void TitleScreenFlashFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MiscSpritePtr miscP = MISC(spriteP);
    SpritePtr jackP;

    (void)curFrameP; (void)frameIndex;
    if (miscP->flashesLeft < 1) {
        if (miscP->flashDelay-- < 0) {
            miscP->flashesLeft = GetRandom(1, 3);
            playThunderSound(spriteP);
            miscP->flashDelay = GetRandom(10, 30);
        }
        return;
    }

    jackP = SWGetNextSprite(gHeroSpriteLayerP, NULL);
    if (!spriteP->isVisible) {
        SWSetSpriteVisible(spriteP, true);
        if (jackP != NULL)
            SWSetSpriteFrameRange(jackP, 3, 3);
    }
    else {
        SWSetSpriteVisible(spriteP, false);
        miscP->flashesLeft--;
        if (jackP != NULL) {
            if (miscP->flashesLeft < 1)
                SWSetSpriteFrameRange(jackP, 0, 2);
            else
                SWSetSpriteFrameRange(jackP, 2, 2);
        }
    }
}

/// Thunder at a random distance: farther away is quieter and lower.
void playThunderSound(SpritePtr spriteP)
{
    int distance = GetRandom(1, 30);
    short position = GetStereoPositionOfSprite(spriteP, &gSpriteWorldP->workAreaRect);

    MMPlaySoundDetailed(0x3a, position >= 0, 1, (short)(distance * 5.0 / -3.0 + (gSoundVolume * 3) / 4),
                        position, distance / -70.0 + 0.8, 0);
}

void InGameLightningFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MiscSpritePtr miscP = MISC(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (!spriteP->isVisible) {
        if (GetRandom(1, 120) == 7) {
            playThunderSound(spriteP);
            SWSetSpriteVisible(spriteP, true);
            SWSetSpriteFrameAdvance(spriteP, 1);
            SWSetSpriteFrameAdvanceMode(spriteP, kNoLoopingMode);
            miscP->lightningFirstFrame = GetRandom(0, 3) << 2;
            miscP->lightningFrame = 0;
            SWSetCurrentFrameIndex(spriteP, miscP->lightningFirstFrame);
        }
    }
    else if (miscP->lightningFrame < 3) {
        miscP->lightningFrame++;
        SWSetCurrentFrameIndex(spriteP, miscP->lightningFirstFrame + miscP->lightningFrame);
    }
    else {
        SWSetSpriteVisible(spriteP, false);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetCurrentFrameIndex(spriteP, 0);
        miscP->flashedTwice = false;
    }
}

/// A shaking web slows down until it stops.
void BigWebFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    long frameTime = SWGetSpriteFrameTime(spriteP);
    double newTime = frameTime / 8.0 + frameTime + 1.8;

    (void)curFrameP; (void)frameIndex;
    SWSetSpriteFrameTime(spriteP, (long)newTime);
    if (newTime > 200.0) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameAdvance(spriteP, 0);
    }
}

/// The hero caught in a web twitches, more often as time goes on.
void RandomFlinchHeroFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    WebTrappedSpritePtr trappedP = WEBTRAPPED(spriteP);

    (void)curFrameP;
    SW_ASSERT(spriteP->userData == kWebTrappedHeroSprite);
    if (*frameIndex != 0) {
        *frameIndex = 0;
        return;
    }
    if (GetRandom(1, trappedP->flinchTimer) == 1)
        *frameIndex = 1;
    if (trappedP->flinchCounter++ > 4) {
        trappedP->flinchTimer++;
        trappedP->flinchCounter = 0;
    }
}

void RandomFlinchEnemyFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)spriteP; (void)curFrameP;
    if (*frameIndex == 1) {
        if (GetRandom(1, 4) == 1)
            *frameIndex = GetRandom(1, 2) == 1 ? 0 : 2;
    }
    else {
        *frameIndex = 1;
    }
}

// ---------------------------------------------------------------------------
// Push-ups (holding down with nothing to climb)

void PushupDelayFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP; (void)frameIndex;
    SWSetSpriteFrameTime(spriteP, 16);
    if (!gKeys[kKeyDown]) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameAdvance(spriteP, 0);
        return;
    }
    SWSetSpriteFrameRange(spriteP, 15, 15);
    SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    if (heroP->pushupDelay-- < 1) {
        SWSetSpriteFrameProc(spriteP, DoPushupsFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 1);
        SWSetSpriteFrameTime(spriteP, 400);
        SWSetSpriteFrameRange(spriteP, 16, 17);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
        heroP->pushupsLeft = 8;
        MMPlaySound(0x2d, 9, 1);
        heroP->pushupSound = 0x2e;
    }
}

void DoPushupsFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP;
    if (heroP->pushupsLeft-- < 1) {
        SWSetSpriteFrameProc(spriteP, PushupDelayFrameProc);
        MySetSpriteFrameTime(spriteP, 800);
        SWSetSpriteFrameAdvance(spriteP, 0);
        heroP->pushupDelay = GetRandom(720, 1200);
        SWSetCurrentFrameIndex(spriteP, 17);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    }
    else if (*frameIndex == 16) {
            // each push-up is slower, and the grunt more strained
        MMPlaySound(heroP->pushupSound, 9, 1);
        if (heroP->pushupSound++ > 0x2f)
            heroP->pushupsLeft = 0;
        SWSetSpriteFrameTime(spriteP, (long)(SWGetSpriteFrameTime(spriteP) + 170.0));
    }
}

// ---------------------------------------------------------------------------
// Saving at a pedestal

/// The gray statue waits a moment, then asks whether to save. Saving fades it
/// out with circles of light; not saving unfreezes Jack right away.
void PauseThenFadeStatueFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    StatueSpritePtr statueP = STATUE(spriteP);
    char eventType;
    unsigned char eventValue;
    Boolean save = false;

    (void)curFrameP;
    SW_ASSERT(spriteP->userData == kStatueSprite);

    if (*frameIndex == 0) {
        if (!SplitScreenActive()) {     // split screen: the others are still playing
            FadeMusicNow(3);
            StopAllChannels();
        }
        if (SplitScreenActive()) {
                // split screen: no question (the others are playing). The statue saves as
                // usual (lives back, and the player comes back here); Arcade Split Screen also
                // writes the team's game there and then
            RestoreJacksLives();
            SplitSaveTeam();
            save = true;
        }
        else if (NetIsFollower()) {
                // two computers: the player who has the turn decides
            save = NetWaitForSaveChoice();
            if (save) {
                gCurrentSaveIndex++;                                    // as the saving computer did
                snprintf(gLastSaveFilename, sizeof(gLastSaveFilename), "%s", NetLastSaveName());
                MultiSaveWrite(kMultiSaveCoop, NetLastSaveName());     // this computer keeps a copy too
            }
        }
        else if (!IsMoviePlaying()) {
            if (!gEditorIsRunning) {
                save = MMDoSaveGameDialog();
                NetSendSaveChoice(save);
                WriteEventToMovieFile(save ? 8 : 9, 1);
            }
        }
        else {
                // replaying a recorded game: the movie says what the player chose
            Boolean gotEvent = GetEventFromMovieFile(&eventType, &eventValue);

            if (gotEvent && eventType == 8) {
                save = true;
            }
            else {
                if (!gotEvent || eventType != 9)
                    StopGameMovie();
                save = false;
            }
            ResetKeys();
        }
        if (save && !SplitScreenActive())
            RestoreJacksLives();
        SWUpdateSpriteWorld(gStatsSpriteWorldP, true);
        SWUpdateSpriteWorld(gSpriteWorldP, true);
        if (save) {
            WriteRoomInfoToFile();
            WriteCStringToFile("SAVED GAME\r");
            WriteCStringToFile("Time spent so far: ");
            WriteNumberToFile(gTimeSpentInMansion / 60);
            WriteCStringToFile(" minutes and ");
            WriteNumberToFile(gTimeSpentInMansion % 60);
            WriteCStringToFile(" seconds.\r");
            MyDelay();
        }
    }
    else {
        save = true;
    }

    if (!save) {
        RemoveSpriteFromObjectArray(statueP->pedestalP);
        gLastSavePedestalObjectID = 0;
        SWSetCurrentFrameIndex(spriteP, 0);
        SWSetSpriteFrameProc(spriteP, PauseThenUnfreezeFrameProc);
        SWSetSpriteFrameTime(spriteP, 0);
        SWSetSpriteFrameAdvance(spriteP, 0);
        statueP->pauseTimer = 30;
        if (statueP->otherStatueP != NULL)
            SWRemoveSpriteFromAnimation(statueP->otherStatueP, true);
    }
    else {
        MMPlaySound(0x33, 9, 1);
        SWSetSpriteFrameAdvance(spriteP, 0);
        SWSetSpriteFrameTime(spriteP, 0);
        statueP->pauseTimer = 2;
        statueP->unused106 = 1;
        SWSetSpriteFrameProc(spriteP, StatueFadeFrameProc);
        SWSetSpriteMoveProc(spriteP, ColorStatueMoveProc);
        SW_ASSERT(statueP->otherStatueP != NULL);
        SWSetSpriteMoveProc(statueP->otherStatueP, CreateCirclesOfLightMoveProc);
    }
}

/// Turns the statue back into Jack (frame 1 is the color statue) and resumes play.
void PauseThenUnfreezeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    StatueSpritePtr statueP = STATUE(spriteP);
    SpritePtr heroP;

    (void)curFrameP;
    if (statueP->pauseTimer-- >= 1)
        return;

    if (*frameIndex == 1) {
        heroP = NewMainSprite();
        if (heroP != NULL) {
            SWMoveSprite(heroP, SWGetSpriteHorizLoc(spriteP), SWGetSpriteVertLoc(spriteP));
            UpdateHaloSpritePosition(heroP);
        }
        if (statueP->otherStatueP != NULL)
            SWRemoveSpriteFromAnimation(statueP->otherStatueP, true);
        SWRemoveSpriteFromAnimation(spriteP, true);
        SW_ASSERT(statueP->pedestalP != NULL);
        SWSetCurrentFrameIndex(statueP->pedestalP, 1);
    }
    else {
        SWRemoveSpriteFromAnimation(spriteP, true);
        heroP = GetHeroSprite();
        if (heroP != NULL)
            SWSetSpriteVisible(heroP, true);
    }
    if (gMusicIsOn && !SplitScreenActive())     // split screen: the music kept playing for everyone
        FadeMusicIn(4);
    MMResumeGameTimer();
    UnpauseAllSpriteLayers(gSpriteWorldP);
    gLayersArePausedForSaveAnim = false;
    gStatuesAreDoneFading = false;
}

void StatueFadeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    StatueSpritePtr statueP = STATUE(spriteP);
    unsigned long level;

    (void)curFrameP; (void)frameIndex;
    if (statueP->pauseTimer-- >= 1)
        return;
    statueP->pauseTimer = 2;
    level = SWGetSpriteTranslucencyLevel(spriteP);
    if (level < 0x5dd) {
        gStatuesAreDoneFading = true;
    }
    else {
        SWSetSpriteTranslucencyLevel(spriteP, level - 0x5dc);
        SWInvalidateSprite(spriteP);
    }
}

/// Each "Z" grows and fades as it floats away.
void ZzzzFadeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    float level, scale;

    (void)curFrameP; (void)frameIndex;
    SWSetSpriteVisible(spriteP, true);
    level = (float)SWGetSpriteTranslucencyLevel(spriteP);
    scale = level / -65535.0f + 2.0f;
    if (level <= 300.0f) {
        SWRemoveSpriteFromAnimation(spriteP, true);
        return;
    }
    SWSetSpriteScaledFactor(spriteP, scale / 3.0f, scale / 3.0f);
    level -= 300.0f;
    if (level <= 0)
        level = 0;
    SWSetSpriteTranslucencyLevel(spriteP, (unsigned long)level);
    SWInvalidateSprite(spriteP);
}

void DeadBodyFadeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);
    unsigned long level;

    (void)curFrameP; (void)frameIndex;
    SW_ASSERT(spriteP != NULL);
    if (heroP->animTimer-- >= 1)
        return;
    heroP->animTimer = 1;
    if (!heroP->deathAnimIsDone) {
        heroP->deathAnimIsDone = true;
        gRestartLevel = true;
    }
    level = SWGetSpriteTranslucencyLevel(spriteP);
    if (level < 0x7d1) {
        SWSetSpriteDrawProc(spriteP, DrawNothingDrawProc);
        SWSetSpriteFrameProc(spriteP, NULL);
    }
    else {
        SWSetSpriteTranslucencyLevel(spriteP, level - 2000);
        SWInvalidateSprite(spriteP);
    }
}

/// Fades a reappearing falling platform (or moving platform, or sprite 0xed) back in.
void FadeInFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (level >= 0xf82f) {
        SWSetSpriteFrameProc(spriteP, NULL);
        return;
    }
    if (spriteP->userData >= kFirstFallingPlatformSprite && spriteP->userData <= 0x5e)
        level += 0x514;
    else if (spriteP->userData >= kFirstMovingPlatformSprite && spriteP->userData <= kLastMovingPlatformSprite)
        level += 0x640;
    else if (spriteP->userData == 0xed)
        level += 0x1e0;
    else
        SW_ASSERT(false);
    SWSetSpriteTranslucencyLevel(spriteP, level);
    SWInvalidateSprite(spriteP);
}

/// Rain/snow that landed fades away.
void FadeOutFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    unsigned long level = SWGetSpriteTranslucencyLevel(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (level > 9000) {
        SWSetSpriteTranslucencyLevel(spriteP, level - 9000);
        SWInvalidateSprite(spriteP);
    }
    else {
        SWRemoveSpriteFromAnimation(spriteP, true);
        gNumWeatherSpritesOnGround--;
    }
}

/// Paintings animate to the end and back, with a random pause at each end.
void AnimatedPaintingFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    BackgroundSpritePtr paintingP = BACKGROUND(spriteP);

    (void)curFrameP;
    if (paintingP->frameTimer-- >= 1)
        return;
    if (spriteP->frameAdvance == 0) {
        SWSetSpriteFrameAdvance(spriteP, *frameIndex == 0 ? 1 : -1);
    }
    else if (*frameIndex == spriteP->maxFrames - 1 || *frameIndex == 0) {
        SWSetSpriteFrameAdvance(spriteP, 0);
        paintingP->frameTimer = GetRandom(paintingP->minDelay, paintingP->maxDelay);
        if (spriteP->userData == 0x80 && *frameIndex == spriteP->maxFrames - 1)
            paintingP->frameTimer = GetRandom(15, 35);
    }
}

void RandomFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    short frame;

    (void)curFrameP;
    do {
        frame = GetRandom(spriteP->firstFrameIndex, spriteP->lastFrameIndex);
    } while (frame == *frameIndex);
    *frameIndex = frame;
}

/// The halo's brightness drifts up and down (userData holds the level).
void HaloFlickerFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP; (void)frameIndex;
    if (spriteP->misc1.n == 1) {
        spriteP->userData += 0x90;
        if (spriteP->userData > 0xd8ff)
            spriteP->misc1.n = -spriteP->misc1.n;
    }
    else {
        spriteP->userData -= 0x90;
        if (spriteP->userData < 0xb8ff)
            spriteP->misc1.n = -spriteP->misc1.n;
    }
    SWSetSpriteTranslucency(spriteP, spriteP->userData);
}

void ResetSpriteFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)spriteP; (void)curFrameP;
    *frameIndex = 0;
}

/// After dying, the hero reappears (blinking) where it entered the room.
void PauseThenReappearFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    HeroSpritePtr heroP = HERO(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (heroP->reappearTimer-- >= 1)
        return;

    if (gCurrentLevel != gLastLevelPositionWasSaved) {
        SWRemoveSprite(spriteP);
        LoadLevel(gLastLevelPositionWasSaved, false);
        SetUpLevelForPlaying();
        DrawLevel();
        DoGameFadeToNormal(30);
        if (gCurrentMusicIndex != gLastMusicIndex) {
            gCurrentMusicIndex = gLastMusicIndex;
            PlayBackgroundMusic(gCurrentMusicIndex);
        }
        SWAddSprite(gDeadHeroSpriteLayerP, spriteP);
    }

    SWSetSpriteLocation(spriteP, gStartingCol, gStartingRow);
    heroP->facingRight = gStartingFacingRight;
    if (gStartingSpriteState == 1) {
        SWSetSpriteFrameRange(spriteP, 18, 25);
        SWSetSpriteFlipX(spriteP, false);
    }
    else if (gStartingSpriteState == 2) {
        SWSetSpriteFrameRange(spriteP, 30, 33);
        SWSetSpriteFlipX(spriteP, false);
    }
    else if (gStartingSpriteState == 3) {
        SWSetSpriteFrameRange(spriteP, 31, 31);
        SWSetSpriteFlipX(spriteP, false);
    }
    else {
        SWSetCurrentFrameIndex(spriteP, 0);
        SWSetSpriteFlipX(spriteP, !heroP->facingRight);
    }
    SWSetSpriteMoveProc(spriteP, BlinkingSpriteMoveProc);
    SWSetSpriteFrameProc(spriteP, BlinkingSpriteFrameProc);
    SWSetSpriteFrameTime(spriteP, 250);
    SWSetSpriteFrameAdvance(spriteP, 0);
    ResetKeys();
    WriteEventToMovieFile(11, 1);
}

/// One-shot animations remove themselves when they wrap back to frame 0.
void PuffOfSmokeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP;
    if (*frameIndex == 0)
        SWRemoveSpriteFromAnimation(spriteP, true);
}

/// Shields glint every 20 frames.
void ShieldSpriteFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    ObjectSpritePtr objectP = OBJECT(spriteP);

    (void)curFrameP;
    if (objectP->glowDelay++ > 19) {
        (*frameIndex)++;
        if (*frameIndex >= spriteP->numFrames) {
            *frameIndex = 0;
            objectP->glowDelay = 0;
        }
    }
}

// ---------------------------------------------------------------------------
// Enemy animations

void SkullSpiderFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkullSpiderSpritePtr spiderP = SKULLSPIDER(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (spiderP->frameTimer-- < 1) {
        spiderP->frameTimer = spiderP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &spiderP->frameDirection);
        MMSetTempStereoFadeDist(600);
        MMPlayStereoSoundAdjustedVol(spriteP, spiderP->stepSound, 13, 3, 250, 1.0);
    }
}

void ScorpionFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    CrawlerSpritePtr crawlerP = CRAWLER(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (crawlerP->frameTimer-- < 1) {
        crawlerP->frameTimer = 7;
        MyAdvanceSpriteFrame(spriteP, &crawlerP->frameDirection);
    }
}

void SnakeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    CrawlerSpritePtr crawlerP = CRAWLER(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (crawlerP->frameTimer-- < 1) {
        crawlerP->frameTimer = 12;
        MyAdvanceSpriteFrame(spriteP, &crawlerP->frameDirection);
    }
}

/// Ravens flap, and squawk at the end of each flap.
void BirdFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    RavenSpritePtr ravenP = RAVEN(spriteP);

    (void)curFrameP;
    if (ravenP->frameTimer-- < 1) {
        ravenP->frameTimer = 6;
        MyAdvanceSpriteFrame(spriteP, &ravenP->frameDirection);
        if (gCurrentGameMode != 3 && *frameIndex == spriteP->maxFrames - 1) {
            MMSetTempStereoFadeDist(500);
            MMPlayStereoSoundAdjustedVol(spriteP, 7, 11, 3, 400, 1.0);
        }
    }
}

void MedusaFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MedusaSpritePtr medusaP = MEDUSA(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (medusaP->frameTimer-- < 1) {
        medusaP->frameTimer = medusaP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &medusaP->frameDirection);
    }
}

void SpiderFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SpiderSpritePtr spiderP = SPIDER(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (spiderP->frameTimer-- < 1) {
        spiderP->frameTimer = spiderP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &spiderP->frameDirection);
    }
}

void SkeletonFishFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    FishSpritePtr fishP = FISH(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (fishP->frameTimer-- < 1) {
        fishP->frameTimer = fishP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &fishP->frameDirection);
    }
}

void FishTurnAroundFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    FishSpritePtr fishP = FISH(spriteP);

    (void)curFrameP;
    if (fishP->frameTimer-- >= 1)
        return;
    fishP->frameTimer = fishP->frameDelay;
    MyAdvanceSpriteFrame(spriteP, &fishP->frameDirection);
    if ((short)(*frameIndex - fishP->firstFrame) == 7) {
            // turned: now dive
        SWSetSpriteFrameRange(spriteP, fishP->firstFrame + 7, fishP->firstFrame + 8);
        SWSetSpriteFrameProc(spriteP, SkeletonFishFrameProc);
        fishP->frameDelay = 6;
        fishP->frameTimer = 6;
    }
}

/// Eyeballs look around randomly (frames 0-6) and now and then blink (10-12).
void EyeballFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP;
    if (GetRandom(1, 5) != 1)
        return;
    *frameIndex = GetRandom(0, 6);
    if (GetRandom(1, 15) == 1) {
        SWSetSpriteFrameProc(spriteP, EyeballBlinkFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 1);
        *frameIndex = 10;
        SWSetSpriteFrameTime(spriteP, 100);
    }
}

void EyeballBlinkFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP;
    if (*frameIndex >= 13) {
        SWSetSpriteFrameAdvance(spriteP, -1);
    }
    else if (*frameIndex < 10 && spriteP->frameAdvance < 0) {
        SWSetSpriteFrameProc(spriteP, EyeballFrameProc);
        SWSetSpriteFrameAdvance(spriteP, 0);
        *frameIndex = 3;
        SWSetSpriteFrameTime(spriteP, 83);
    }
}

/// A pile of bones pulls itself together (frames 0-7, faster and faster).
void SkeletonUncollapseFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);

    (void)curFrameP;
    if (skeletonP->frameTimer-- >= 1)
        return;
    MyAdvanceSpriteFrame(spriteP, &skeletonP->frameDirection);
    skeletonP->frameTimer = (short)(-*frameIndex / -1.7 + 7.0);
    if (*frameIndex == 1)
        MMPlaySound(0x34, 11, 3);
    if (*frameIndex == 7) {
        SWSetSpriteFrameProc(spriteP, SkeletonPauseThenShakeFrameProc);
        skeletonP->frameTimer = GetRandom(20, 30);
    }
}

/// Turns toward the hero and starts shaking itself off (frames 8-9).
void SkeletonPauseThenShakeFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);
    SpritePtr heroP;
    short mid, heroMid;

    (void)curFrameP; (void)frameIndex;
    if (skeletonP->frameTimer-- >= 1)
        return;

    heroP = GetHeroSprite();
    if (heroP == NULL)
        heroP = SWGetPreviousSprite(gDeadHeroSpriteLayerP, NULL);
    if (heroP == NULL) {
        skeletonP->facingRight = false;
        SWSetSpriteFrameRange(spriteP, 8, 9);
    }
    else {
        mid = (short)(SWGetSpriteWidth(spriteP) / 2.0f + SWGetSpriteHorizLoc(spriteP));
        heroMid = (short)(SWGetSpriteWidth(heroP) / 2.0f + SWGetSpriteHorizLoc(heroP));
        SWSetSpriteFrameRange(spriteP, 8, 9);
        skeletonP->facingRight = !(heroMid + skeletonP->speed < mid);
        SWSetSpriteFlipX(spriteP, skeletonP->facingRight);
    }
    skeletonP->shakeCount = 6;
    SWRemoveSprite(spriteP);
    SWAddSprite(gEnemySpriteLayerP, spriteP);
    SWSetSpriteFrameProc(spriteP, SkeletonShakeOffFrameProc);
    skeletonP->frameTimer = 5;
}

void SkeletonShakeOffFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (skeletonP->frameTimer-- >= 1)
        return;
    skeletonP->frameTimer = 5;
    MyAdvanceSpriteFrame(spriteP, &skeletonP->frameDirection);
    if ((skeletonP->shakeCount / 2) * 2 == skeletonP->shakeCount)
        MMPlayStereoSound(spriteP, 0x17, 11, CountSounds(0x17) < 3 ? 1 : 3);
    if (skeletonP->shakeCount-- < 0) {
        SWSetSpriteFrameProc(spriteP, SkeletonPauseThenWalkFrameProc);
        skeletonP->frameTimer = 30;
    }
}

void SkeletonPauseThenWalkFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);
    short speed;

    (void)curFrameP; (void)frameIndex;
    if (skeletonP->frameTimer-- >= 1)
        return;
    SWSetSpriteMoveProc(spriteP, WalkingSkeletonMoveProc);
    SWSetSpriteFrameProc(spriteP, WalkingSkeletonFrameProc);
    speed = (short)(skeletonP->speed + skeletonP->speed + 4.0f);
    if (speed > 11)
        speed = 12;
    skeletonP->frameDelay = 60 / speed;
    skeletonP->frameTimer = 60 / speed;
}

void WalkingSkeletonFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SkeletonSpritePtr skeletonP = SKELETON(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (skeletonP->frameTimer-- < 1) {
        skeletonP->frameTimer = skeletonP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &skeletonP->frameDirection);
    }
}

void MiscSpriteFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MiscSpritePtr miscP = MISC(spriteP);

    (void)curFrameP; (void)frameIndex;
    if (miscP->frameTimer-- < 1) {
        miscP->frameTimer = miscP->frameDelay;
        MyAdvanceSpriteFrame(spriteP, &miscP->frameDirection);
    }
}

/// A key's glow pulses with the key's spin (the parent it is mounted on).
/// misc1 holds the glow's full brightness.
void KeyGlowFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    SpritePtr keyP = spriteP->mountParentP;
    short frame;
    float scale = 1.0f, level;

    (void)curFrameP; (void)frameIndex;
    if (keyP == NULL)
        return;
    frame = (short)keyP->curFrameIndex;
    if (frame >= keyP->maxFrames / 2)
        frame -= (short)(keyP->maxFrames / 2);
    if (frame == 0)
        scale = 1.2f;
    else if (frame == 1 || frame == 7)
        scale = 1.1f;
    else if (frame == 2 || frame == 6)
        scale = 1.0f;
    else if (frame == 3 || frame == 5)
        scale = 0.9f;
    else if (frame == 4)
        scale = 0.8f;
    level = spriteP->misc1.n * ((scale + 0.95f) / 2.0f);
    if (level <= 0)
        level = 0;
    SWSetSpriteTranslucency(spriteP, (unsigned long)level);
}

/// The hint question mark bobs and wobbles.
void HintQuestionMarkFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    MiscSpritePtr miscP = MISC(spriteP);

    (void)curFrameP; (void)frameIndex;
    SWOffsetSprite(spriteP, 0, (float)(SWCos(miscP->bobAngle) / 11.0));
    miscP->bobAngle += 3.0f;
    if (miscP->bobAngle >= 360.0f)
        miscP->bobAngle -= 360.0f;
    SWSetSpriteRotation(spriteP, (float)(SWCos(miscP->wobbleAngle) / 8.0 + spriteP->drawData->rotation));
    miscP->wobbleAngle += 2.0f;
    if (miscP->wobbleAngle >= 360.0f)
        miscP->wobbleAngle -= 360.0f;
}

/// Flames flicker through their frames at random.
void FlameFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    long oldFrame = *frameIndex;

    (void)curFrameP;
    do {
        *frameIndex = GetRandom(0, spriteP->maxFrames - 1);
    } while (*frameIndex == (short)oldFrame);
}

void CandleFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    long oldFrame = *frameIndex;

    (void)curFrameP;
    do {
        *frameIndex = GetRandom(0, 3);
    } while (*frameIndex == (short)oldFrame);
    if (CANDLE(spriteP)->glowSpriteP != NULL)
        SWSetCurrentFrameIndex(CANDLE(spriteP)->glowSpriteP, *frameIndex);
}

void LanternFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    long oldFrame = *frameIndex;

    (void)spriteP; (void)curFrameP;
    do {
        *frameIndex = GetRandom(0, 3);
    } while (*frameIndex == (short)oldFrame);
}

void TrapDoorFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP;
    if (*frameIndex == spriteP->maxFrames - 1) {
        SWSetSpriteFrameProc(spriteP, NULL);
        SWSetSpriteFrameTime(spriteP, -1);
    }
}

void SkullFallingPlatformFrameProc(SpritePtr spriteP, FramePtr curFrameP, long *frameIndex)
{
    (void)curFrameP;
    if (*frameIndex == 0 || *frameIndex == spriteP->maxFrames - 1) {
        SWSetSpriteFrameTime(spriteP, -1);
        SWSetSpriteFrameProc(spriteP, NULL);
    }
}
