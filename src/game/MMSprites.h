/*
 * MMSprites.h — the game's sprite records.
 *
 * Every game sprite starts with a SpriteRec and adds its own fields. In the
 * original 32-bit build SpriteRec was 0xf8 bytes; the "0x..." comments give
 * each field's offset there, to match it with the disassembly. All game
 * sprites carry the level-object ID at 0xf8 (-1 for sprites not placed by the
 * level), so every record starts with GameSpriteRec.
 */
#ifndef MMSPRITES_H
#define MMSPRITES_H

/// Sprite type numbers (SpriteRec.userData). Level objects use 0..0xb8.
enum {
    kHeroSprite = 0,
    kSkullSpiderSprite = 1,
    kScorpionSprite = 2,
    kEyeballSprite = 3,
    kBlueRavenSprite = 4, kRedRavenSprite = 5,
    kSnakeSprite1 = 6, kSnakeSprite2 = 7,
    kMedusaHeadSprite1 = 8, kMedusaHeadSprite2 = 9,
    kSmallSpiderSprite = 10, kBigSpiderSprite = 11,
    kSkeletonFishSprite = 12,
    kWalkingSkeletonSprite = 13,
    kFirstLeverSprite = 14, kLastLeverSprite = 17,
    kFirstBrickSwitchSprite = 18, kLastBrickSwitchSprite = 25,
    kFirstZapperNodeSprite = 26, kLastZapperNodeSprite = 29,
    kFirstObjectSprite = 30, kLastObjectSprite = 44,
    kLanternObjectSprite = 36,
    kSignObjectSprite = 38,
    kFirstDoorSprite = 45, kLastDoorSprite = 68,
    kFirstMovingPlatformSprite = 69, kLastMovingPlatformSprite = 79,
    kPlatformBlockerSprite = 79,
    kFirstFallingPlatformSprite = 80, kLastFallingPlatformSprite = 94,
    kFirstTrapDoorSprite = 95, kLastTrapDoorSprite = 96,
    kFirstPedestalSprite = 97, kSavePedestalSprite = 101, kLastPedestalSprite = 111,
    kFirstBackgroundSprite = 112, kLastBackgroundSprite = 157,
    kFirstTorchSprite = 158, kLastTorchSprite = 160,
    kFirstCandleSprite = 161, kLastCandleSprite = 166,
    kFirstCandleHolderSprite = 167, kLastCandleHolderSprite = 176,
    kFirstTreasureSprite = 177, kLastTreasureSprite = 182,
    kExitSprite = 183,
    kSecretIndicatorSprite = 184,
    kLastLevelSprite = 184,

        // sprites created by the game, not placed by levels
    kPuffOfSmokeSprite = 187,
    kBloodDripSprite = 188,
    kGooDripSprite = 190,
    kSlimeSprite = 192,
    kFlameSprite = 193,
    kStatueSprite = 194,
    kWebTrappedHeroSprite = 195,
    kWebTrappedEnemySprite = 196,
    kBottomCircleOfLightSprite = 197,
    kGoldSparkleSprite = 199,
    kZzzzSprite = 210,
    kHintQuestionMarkSprite = 225
};

/// Fields shared by every game sprite.
typedef struct GameSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8 level-object ID, -1 if none
} GameSpriteRec, *GameSpritePtr;

#define GAME_SPRITE(s)  ((GameSpritePtr)(s))

/// Jack, the player's sprite (0x1d8 bytes in the original).
typedef struct HeroSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       frameTimer;             ///< 0xfc counts down to the next frame (HeroMiscFrameProc)
    short       frameDirection;         ///< 0xfe +1/-1, see MyAdvanceSpriteFrame
    short       frameDelay;             ///< 0x100 frameTimer reload value
    SWRect      collisionInset;         ///< 0x102 inset used for floors and platforms
    SWRect      doorInset;              ///< 0x10a inset used for doors
    SWRect      ladderUpInset;          ///< 0x112 ladder search area when pressing up
    SWRect      ladderDownInset;        ///< 0x11a ladder search area when pressing down
    SWRect      vineInset;              ///< 0x122 vine/pole search area
    SWRect      vineFromGroundInset;    ///< 0x12a vine search area when standing
    SWRect      poleInset;              ///< 0x132 floor check while sliding down a pole
    SWRect      unusedInset;            ///< 0x13a
    SWRect      wallInset;              ///< 0x142 inset for wall checks
    short       normalDoorInsetTop;     ///< 0x14a doorInset.top when not jumping
    short       normalInsetTop;         ///< 0x14c collisionInset.top when not jumping
    float       platformHorizDelta;     ///< 0x150 horizontal speed of the platform last stood on
    float       unused154;              ///< 0x154
    float       conveyorDelta;          ///< 0x158 conveyor/wind push when last on the ground
    float       jumpHorizDelta;         ///< 0x15c carried horizontal speed while jumping
    float       jumpVertDelta;          ///< 0x160 vertical speed inherited from a platform
    SpritePtr   jumpPlatformP;          ///< 0x164 platform jumped from
    short       lastStepFrame;          ///< 0x168 frame that last played a footstep
    short       ladderDirection;        ///< 0x16a -1 up, +1 down, 0 none
    short       deathType;              ///< 0x16c kDeath...
    Boolean     canJump;                ///< 0x16e false until the jump key is released
    Boolean     canDoPushups;           ///< 0x16f
    Boolean     isJumping;              ///< 0x170
    Boolean     isDoingPushups;         ///< 0x171
    Boolean     isLanding;              ///< 0x172 landing animation is playing
    short       jumpFrameCount;         ///< 0x174
    Boolean     unused176;              ///< 0x176
    Boolean     facingRight;            ///< 0x177
    Boolean     wasFacingRight;         ///< 0x178
    Boolean     hasClimbed;             ///< 0x179 has moved on the current ladder
    Boolean     isDead;                 ///< 0x17a
    Boolean     didSkeletonCollapse;    ///< 0x17b
    Boolean     didStoneCollapse;       ///< 0x17c
    Boolean     deathAnimIsDone;        ///< 0x17d
    Boolean     onGround;               ///< 0x17e standing on something solid
    Boolean     playedFallingSound;     ///< 0x17f
    Boolean     wasOnGround;            ///< 0x180
    short       jumpStartBottom;        ///< 0x182 collision bottom when the jump started
    short       fallStartTop;           ///< 0x184 top when the hero last left the ground
    short       lastGroundVertLoc;      ///< 0x186 vertical location when last on the ground
    short       pushupDelay;            ///< 0x188
    short       pushupsLeft;            ///< 0x18a
    short       pushupSound;            ///< 0x18c grunts get higher as Jack tires
    short       ladderStepTimer;        ///< 0x18e
    short       leverTimer;             ///< 0x190
    short       collapseTimer;          ///< 0x192
    short       reappearTimer;          ///< 0x194
    short       animTimer;              ///< 0x196 landing/death animation counter
    short       electrocutionTime;      ///< 0x198
    short       jumpBufferFrames;       ///< 0x19a a jump pressed this recently still happens
    short       jumpBufferDirection;    ///< 0x19c 0 left, 1 straight, 2 right
    short       upKeyDelay;             ///< 0x19e frames before up can grab a ladder again
    short       downKeyDelay;           ///< 0x1a0 frames before down can grab a ladder again
    short       idleState;              ///< 0x1a2 IdleHeroFrameProc state
    short       idleDelay;              ///< 0x1a4 frames standing still before idling
    short       blinkTimer;             ///< 0x1a6 idle animation timers
    short       lookTimer;              ///< 0x1a8
    short       lookAroundTimer;        ///< 0x1aa
    short       sleepTimer;             ///< 0x1ac
    short       scratchCount;           ///< 0x1ae
    short       vineTileLayer;          ///< 0x1b0 tile layer of the vine being climbed
    short       leverStartHorizLoc;     ///< 0x1b2 where the hero stood before pulling a lever
    short       lavaSurfaceRow;         ///< 0x1b4 pixel row of the lava surface
    float       lavaSinkFactor;         ///< 0x1b8
    short       actionKeyFrames;        ///< 0x1bc frames the action key has been up
    short       flickerCount;           ///< 0x1be electrocution: frames before switching poses
    SpritePtr   pedestalP;              ///< 0x1c0 save pedestal stood on this frame
    SpritePtr   platformP;              ///< 0x1c4 moving platform stood on
    SpritePtr   leverP;                 ///< 0x1c8 lever being pulled
    SpritePtr   deadBodyP;              ///< 0x1cc dead body this sprite follows
    SpritePtr   attachedSpriteP;        ///< 0x1d0 sprite removed along with the hero
    Boolean     showQuestionMark;       ///< 0x1d4
} HeroSpriteRec, *HeroSpritePtr;

#define HERO(s) ((HeroSpritePtr)(s))

/// Death types (HeroSpriteRec.deathType).
enum {
    kDeathNone = 0,
    kDeathSkeletonCollapse = 1,
    kDeathStoneCollapse = 2,
    kDeathElectrocution = 3,
    kDeathFellTooFar = 4,
    kDeathSmallSpider = 5,
    kDeathCollapsing = 6                ///< collapse animation has started
};

/// Doors (0x128 bytes). They open by sliding into the wall above or below.
typedef struct DoorSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       bottomInset;            ///< 0xfc drawn height lost at the bottom
    short       topInset;               ///< 0xfe drawn height lost at the top
    Boolean     unused100;              ///< 0x100
    short       openDistance;           ///< 0x102 distance left to slide
    Boolean     isOpen;                 ///< 0x104
    Boolean     opensUpward;            ///< 0x105 even door types slide up (shrink from the bottom)
    double      moveFraction;           ///< 0x108 sub-pixel movement carried to the next frame
    double      moveSpeed;              ///< 0x110
    double      moveAccel;              ///< 0x118
    Boolean     startsOpen;             ///< 0x120
    short       height;                 ///< 0x122
    SpritePtr   partnerDoorP;           ///< 0x124 the other half of a double door
} DoorSpriteRec, *DoorSpritePtr;

/// Levers and brick switches (0x118 bytes).
enum {
        // lever actions
    kLeverToggleTarget = 0, kLeverReverseOnePlatform = 1, kLeverReverseAllPlatforms = 2,
    kLeverReverseConveyors = 3, kLeverStartStopPlatforms = 4, kLeverStartStopConveyors = 5,
    kLeverStartStopZappers = 6, kLeverToggleGoldSilver = 7, kLeverOpenTrapDoors = 8,
    kLeverOpenGrayDoors = 9, kLeverToggleGrayDoors = 10, kLeverToggleTorches = 11
};
typedef struct LeverSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       action;                 ///< 0xfc kLever... (brick switches have their own list)
    Sint32      targetObjectID;         ///< 0x100 object acted on
    Boolean     isActive;               ///< 0x104 can still be used
    Boolean     isOneShot;              ///< 0x105 only works once
    Boolean     dontRememberState;      ///< 0x106 resets when the room is reloaded
    Boolean     isInverted;             ///< 0x107 the action applies while NOT pulled
    SpritePtr   markerSpriteP;          ///< 0x114 shows that the lever stays put
} LeverSpriteRec, *LeverSpritePtr;

/// Zapper nodes (0x11c bytes): pairs of nodes facing each other shoot a beam
/// between them, on and off.
typedef struct ZapperNodeSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       onTime;                 ///< 0xfc frames the beam is on
    short       offTime;                ///< 0xfe frames it is off
    short       timer;                  ///< 0x100
    short       frameCounter;           ///< 0x102
    Boolean     isOn;                   ///< 0x104
    Boolean     isDisabled;             ///< 0x105 switched off by a lever
    short       beamLength;             ///< 0x106 tiles the beam currently covers
    short       distance;               ///< 0x108 tiles to the opposite node
    SpritePtr   partnerP;               ///< 0x10c the opposite node
    SpritePtr   endGlowP;               ///< 0x110 glow at this end
    SpritePtr   beamGlowP;              ///< 0x114 glow along the beam
    SpritePtr   farGlowP;               ///< 0x118 glow at the other end
} ZapperNodeSpriteRec, *ZapperNodeSpritePtr;

/// One tile of a zapper beam (0x104 bytes).
typedef struct ZapperBeamSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SpritePtr   nodeP;                  ///< 0xfc the node that controls it
    short       baseFrame;              ///< 0x100
    Boolean     isHorizontal;           ///< 0x102
} ZapperBeamSpriteRec, *ZapperBeamSpritePtr;

#define LEVER(s)        ((LeverSpritePtr)(s))
#define ZAPPER(s)       ((ZapperNodeSpritePtr)(s))
#define BEAM(s)         ((ZapperBeamSpritePtr)(s))

/// Enemies that walk back and forth with the inset first: skull spiders (0x10c bytes).
typedef struct SkullSpiderSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    short       stepSound;              ///< 0x104 sound played every step
    short       frameTimer;             ///< 0x106
    short       frameDirection;         ///< 0x108
    short       frameDelay;             ///< 0x10a set from the speed
} SkullSpiderSpriteRec, *SkullSpiderSpritePtr;

/// Scorpions and snakes (0x108 bytes).
typedef struct CrawlerSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       frameTimer;             ///< 0xfc
    short       frameDirection;         ///< 0xfe
    SWRect      collisionInset;         ///< 0x100
} CrawlerSpriteRec, *CrawlerSpritePtr;

/// Bouncing eyeballs (0x17c bytes). When one lands it squashes and bounces back up.
#define kMaxEyeballSquishFrames 50
typedef struct EyeballSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    float       bounceSpeed;            ///< 0x104 speed when it hit the floor
    short       normalHeight;           ///< 0x108
    short       normalWidth;            ///< 0x10a
    float       landedVertLoc;          ///< 0x10c
    float       landedHorizLoc;         ///< 0x110
    short       squishHeights[kMaxEyeballSquishFrames]; ///< 0x114 heights while squashing, replayed backward
    short       numSquishFrames;        ///< 0x178
    short       squishTimer;            ///< 0x17a
} EyeballSpriteRec, *EyeballSpritePtr;

/// Ravens (0x128 bytes). Red ones fly in a sine wave.
typedef struct RavenSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    short       frameTimer;             ///< 0x104
    short       frameDirection;         ///< 0x106
    float       baseVertLoc;            ///< 0x108 middle of the wave (-1 until set)
    double      angle;                  ///< 0x10c degrees
    double      amplitude;              ///< 0x114
    double      angleStep;              ///< 0x11c
    Boolean     ignoresWalls;           ///< 0x124
} RavenSpriteRec, *RavenSpritePtr;

/// Flying Medusa heads (0x11c bytes). They home in on the hero.
typedef struct MedusaSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       frameDelay;             ///< 0xfc
    short       frameTimer;             ///< 0xfe
    short       frameDirection;         ///< 0x100
    SWRect      collisionInset;         ///< 0x102
    short       soundTimer;             ///< 0x10a
    float       speed;                  ///< 0x10c
    float       fallSpeed;              ///< 0x110 when burning
    float       fallAccel;              ///< 0x114
    Boolean     ignoresWalls;           ///< 0x118
} MedusaSpriteRec, *MedusaSpritePtr;

/// Spiders (0x140 bytes). Small ones drop on a thread and climb back up; big ones
/// wait at the top and drop onto prey passing below.
enum { kSpiderDropping = 0, kSpiderClimbing = 1, kSpiderWaiting = 2, kSpiderSlowing = 3, kSpiderWatching = 4,
       kSpiderFull = 5 };
typedef struct SpiderSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    short       topVertLoc;             ///< 0x104 where it hangs (-1 until set)
    short       waitTimer;              ///< 0x106
    short       state;                  ///< 0x108 kSpider...
    SpritePtr   preyP;                  ///< 0x10c caught sprite
    short       frameDelay;             ///< 0x110
    short       frameTimer;             ///< 0x112
    short       frameDirection;         ///< 0x114
    short       eatTimer;               ///< 0x116
    Boolean     isFast;                 ///< 0x118 big spider dropping onto a raven
    float       climbSpeed;             ///< 0x11c
    float       dropAccel;              ///< 0x120
    float       minDropSpeed;           ///< 0x124
    float       maxDropSpeed;           ///< 0x128
    short       waitTime;               ///< 0x12c
    float       bounce;                 ///< 0x130 fraction of the drop speed it bounces back up
    float       dropSpeed;              ///< 0x134
    float       peakDropSpeed;          ///< 0x138
    Sint32      eatCounter;             ///< 0x13c
} SpiderSpriteRec, *SpiderSpritePtr;

/// Skeleton fish (0x118 bytes): jump out of the lava and fall back.
typedef struct FishSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       restVertLoc;            ///< 0xfc
    short       firstFrame;             ///< 0xfe 0 or 9 (two looks)
    short       frameDelay;             ///< 0x100
    short       frameTimer;             ///< 0x102
    short       frameDirection;         ///< 0x104
    short       waitTime;               ///< 0x106
    short       waitTimer;              ///< 0x108
    float       jumpSpeed;              ///< 0x10c
    float       gravity;                ///< 0x110
    Boolean     hasTurned;              ///< 0x114 turned to dive back down
} FishSpriteRec, *FishSpritePtr;

/// Walking skeletons (0x11c bytes): lie in pieces until something wakes them.
enum { kSkeletonWakeNever = 0, kSkeletonWakeAfterDelay = 1, kSkeletonWakeWhenNear = 2, kSkeletonWakeDelayWhenNear = 3 };
typedef struct SkeletonSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       frameDelay;             ///< 0xfc
    short       frameTimer;             ///< 0xfe
    short       frameDirection;         ///< 0x100
    SWRect      collisionInset;         ///< 0x102
    Boolean     facingRight;            ///< 0x10a
    Boolean     hasUncollapsed;         ///< 0x10b
    short       shakeCount;             ///< 0x10c
    short       wakeMode;               ///< 0x10e kSkeletonWake...
    short       wakeDelay;              ///< 0x110
    short       wakeDistance;           ///< 0x112
    float       speed;                  ///< 0x114
    short       wakeTimer;              ///< 0x118
} SkeletonSpriteRec, *SkeletonSpritePtr;

/// Objects (keys, treasure, signs, lantern...) (0x118 bytes).
typedef struct ObjectSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    Sint32      glowDelay;              ///< 0x104 frames until the next glint
    SpritePtr   glowSpriteP;            ///< 0x108 the lantern's glow
    short       signTextResID;          ///< 0x10c signs and tips: TEXT resource
    short       signPictResID;          ///< 0x10e sign picture (tips: their width)
    short       mapGridH;               ///< 0x110 map items: which part of the map they reveal
    short       mapGridV;               ///< 0x112
    Boolean     isVisible;              ///< 0x114
    Boolean     isTip;                  ///< 0x115 a tip (shown nearby, with a question mark)
} ObjectSpriteRec, *ObjectSpritePtr;

/// Blood/goo drips, splashes and slime (0x108 bytes).
typedef struct DripSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    Boolean     unused104;              ///< 0x104
    Boolean     playsSplashSound;       ///< 0x105
} DripSpriteRec, *DripSpritePtr;

/// Save pedestals and decorative statues (0x108 bytes).
typedef struct PedestalSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    Boolean     isActive;               ///< 0x104 false until the hero steps off it
} PedestalSpriteRec, *PedestalSpritePtr;

/// The statues of Jack shown while saving (0x118 bytes).
typedef struct StatueSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       pauseTimer;             ///< 0x104
    short       unused106;              ///< 0x106
    Sint32      circleTimer;            ///< 0x108 frames until the next circle of light
    short       circleFrame;            ///< 0x10c
    SpritePtr   otherStatueP;           ///< 0x110
    SpritePtr   pedestalP;              ///< 0x114
} StatueSpriteRec, *StatueSpritePtr;

/// Rising circles of light around the statue (0x114 bytes); one in front, one behind.
typedef struct CircleOfLightSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SpritePtr   statueP;                ///< 0x104
    SpritePtr   partnerP;               ///< 0x108
    short       startVertLoc;           ///< 0x10c
    float       riseSpeed;              ///< 0x110
} CircleOfLightSpriteRec, *CircleOfLightSpritePtr;

/// Hero or enemy caught in a big spider's web (0x10c bytes).
typedef struct WebTrappedSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    short       dripChance;             ///< 0x104
    short       dripTimer;              ///< 0x106
    short       flinchTimer;            ///< 0x108
    short       flinchCounter;          ///< 0x10a
} WebTrappedSpriteRec, *WebTrappedSpritePtr;

/// Animated backgrounds (0x108 bytes): big webs and paintings.
typedef struct BackgroundSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    Boolean     isArmed;                ///< 0xfc webs: the hero has left it
    short       frameTimer;             ///< 0xfe paintings: frames until the next change
    short       minDelay;               ///< 0x100
    short       maxDelay;               ///< 0x102
} BackgroundSpriteRec, *BackgroundSpritePtr;

/// Torches (0x104 bytes).
typedef struct TorchSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SpritePtr   flameSpriteP;           ///< 0xfc NULL when unlit
    SpritePtr   glowSpriteP;            ///< 0x100
} TorchSpriteRec, *TorchSpritePtr;

/// Candles (0x100 bytes).
typedef struct CandleSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SpritePtr   glowSpriteP;            ///< 0xfc flickers with the flame
} CandleSpriteRec, *CandleSpritePtr;

/// One-shot animations (smoke, splashes, sparkles) (0x100 bytes).
typedef struct PuffSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    Boolean     isDone;                 ///< 0xfc
} PuffSpriteRec, *PuffSpritePtr;

/// Generic 300-byte records: misc sprites, title/difficulty/organ-screen sprites.
typedef struct MiscSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    Boolean     flashedTwice;           ///< 0xfc lightning
    short       flashesLeft;            ///< 0xfe title-screen lightning
    short       flashDelay;             ///< 0x100
    short       lightningFrame;         ///< 0x102 in-game lightning
    short       lightningFirstFrame;    ///< 0x104
    short       unused106[5];           ///< 0x106
    short       frameTimer;             ///< 0x110
    short       frameDelay;             ///< 0x112
    short       frameDirection;         ///< 0x114
    short       unused116;              ///< 0x116
    float       wobbleAngle;            ///< 0x118 hint question mark
    float       bobAngle;               ///< 0x11c
    char        reserved[0x12c - 0x120];
} MiscSpriteRec, *MiscSpritePtr;

#define SKULLSPIDER(s)  ((SkullSpiderSpritePtr)(s))
#define CRAWLER(s)      ((CrawlerSpritePtr)(s))
#define EYEBALL(s)      ((EyeballSpritePtr)(s))
#define RAVEN(s)        ((RavenSpritePtr)(s))
#define MEDUSA(s)       ((MedusaSpritePtr)(s))
#define SPIDER(s)       ((SpiderSpritePtr)(s))
#define FISH(s)         ((FishSpritePtr)(s))
#define SKELETON(s)     ((SkeletonSpritePtr)(s))
#define OBJECT(s)       ((ObjectSpritePtr)(s))
#define DRIP(s)         ((DripSpritePtr)(s))
#define PEDESTAL(s)     ((PedestalSpritePtr)(s))
#define STATUE(s)       ((StatueSpritePtr)(s))
#define CIRCLE(s)       ((CircleOfLightSpritePtr)(s))
#define WEBTRAPPED(s)   ((WebTrappedSpritePtr)(s))
#define BACKGROUND(s)   ((BackgroundSpritePtr)(s))
#define TORCH(s)        ((TorchSpritePtr)(s))
#define CANDLE(s)       ((CandleSpritePtr)(s))
#define PUFF(s)         ((PuffSpritePtr)(s))
#define MISC(s)         ((MiscSpritePtr)(s))

#define DOOR(s) ((DoorSpritePtr)(s))

/// Moving and falling platforms (0x118 bytes).
typedef struct PlatformSpriteRec {
    SpriteRec   sprite;
    Sint32      objectID;               ///< 0xf8
    SWRect      collisionInset;         ///< 0xfc
    Boolean     isMoving;               ///< 0x104 moving platforms can be stopped by levers
    short       startVertLoc;           ///< 0x106 falling platforms: where they reappear (-1 unset)
    short       reappearTimer;          ///< 0x108
    short       fallDelay;              ///< 0x10a frames between being stepped on and falling
    float       moveDeltaH;             ///< 0x110 movement this frame (also while invisible)
    float       moveDeltaV;             ///< 0x114
} PlatformSpriteRec, *PlatformSpritePtr;

#define PLATFORM(s) ((PlatformSpritePtr)(s))

#endif
