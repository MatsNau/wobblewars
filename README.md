
WobbleWars DS is simple game for the Nintendo DS made with [NightFox's Lib](https://gbatemp.net/threads/nightfoxs-lib-for-libnds-nflib-ds-entry-coding-library.280385/) and libnds. Based on polymars [Snake DS](https://github.com/PolyMarsDev/Snake-DS) 

# WobbleWars-DS

A Nintendo DS homebrew game developed using DevKitPro and NightFoxLib. Based on the snake game by polymars (). This is a 2D action game where the player controls Nina, who must defend herself against enemies while progressing through the game.

<img width="458" height="687" alt="image" src="https://github.com/user-attachments/assets/f0891b52-b0fd-40b8-80a9-c6885b2cb882" />

## Technical Overview

### Development Environment
- **Platform**: Nintendo DS
- **Toolchain**: DevKitPro (devkitARM)
- **Graphics Library**: NightFoxLib (NFLib)
- **Language**: C++
- **Audio**: Raw PCM audio files
- **Build System**: Make

### Architecture

#### Game States
- `INTRO`: Main menu/start screen
- `PLAYING`: Active gameplay
- `GAMEOVER`: Death screen with restart option
- `WINNING`: Victory sequence with dual-screen transition

### Graphics Implementation

#### Sprite System
- **Format**: Custom .img/.pal files generated with GRIT
- **Animation**: Frame-based sprite animation system
- **Dual Screen**: Sprites rendered on both DS screens for seamless transitions
- **Sprite IDs**: Dynamic allocation system for multiple entities

```cpp
// Example sprite initialization
spriteManager.initSprite("sprites/player/idle", 0, 32, 0, 0, 0, false, 0);
spriteManager.createSprite(0, 0, 0, 0, nina.getX(), nina.getY());
```

#### Background System
- Tiled backgrounds using NightFoxLib
- Multiple background layers per screen
- Screen-specific background loading

#### Animation System
- Frame-based animation with configurable timing
- State-dependent animations (idle, walking, dying)
- Sprite flipping for directional movement
- Synchronized animation updates at 60 FPS

### Audio Implementation

#### Sound System
```cpp
// Audio initialization
soundEnable();
NF_InitRawSoundBuffers();
NF_LoadRawSound("sounds/output", 0, 5512, 0);

// Playback
u8 sound_id = NF_PlayRawSound(0, 127, 64, true, 0);
```

#### Audio Compression
- **Format**: Raw PCM audio
- **Compression**: Reduced to 8kHz sample rate, 8-bit, mono
- **Target Size**: <256KB for memory constraints
- **Tools**: SoX for audio processing

```bash
# Audio compression pipeline
sox input.raw -r 8000 -c 1 -b 8 output.raw
```

### Input System

#### Controls
- **D-Pad**: Player movement (4-directional)
- **Touchscreen**: Weapon targeting and throwing
- **A Button**: Menu interactions, restart
- **Start Button**: Exit application

#### Touch Implementation
```cpp
touchPosition touch;
touchRead(&touch);
if (keysDown() & KEY_TOUCH) {
    nina.throwWeapon(touch.px, touch.py);
}
```

### Game Mechanics

#### Enemy Movevment
- **Spawning**: Random spawn with enough distance from player
- **Movement**: Direct pathfinding toward player
- **Collision**: Circle-based collision detection
- **States**: Chasing, Dying with animated transitions

#### Weapon System
- **Physics**: Projectile motion with target-seeking behavior
- **Return Mechanism**: Weapon returns to player's current position
- **Visibility**: Dynamic sprite showing/hiding based on weapon state

#### Collision Detection
```cpp
// Example collision check
int dx = enemy.getX() - weapon.getX();
int dy = enemy.getY() - weapon.getY();
if (dx * dx + dy * dy < 64) {
    // Collision detected
}
```

### Memory Management

#### Resource Loading
- Sprites loaded into VRAM at initialization
- Dynamic sprite creation/destruction for enemies
- Efficient memory usage with sprite pooling

#### Performance Optimizations
- Frame-rate locked to 60 FPS using `swiWaitForVBlank()`
- Minimal dynamic allocations during gameplay
- Optimized collision detection using squared distances

### Build System

#### Dependencies
```makefile
SOURCES := source source/Managers
INCLUDES := include include/Managers
LIBS := -lnflib -lfilesystem -lfat -lnds9
```

#### Asset Pipeline
1. Graphics converted using GRIT tool
2. Audio processed with SoX
3. Assets embedded in NitroFS
4. Automatic resource management through NFLib


## Building

### Prerequisites
- DevKitPro with devkitARM
- NightFoxLib
- GRIT (for graphics conversion)
- SoX (for audio processing)

### Compilation
```bash
make clean
make
```

### Running
Use any DS emulator (DeSmuME, melonDS) or flash to real hardware.

## Technical Challenges Solved

1. **Dual Screen Coordination**: Seamless character transition between screens
2. **Memory Constraints**: Efficient sprite and audio management within DS limitations
3. **Audio Compression**: Maintaining audio quality while meeting size constraints
4. **Real-time Animation**: Smooth 60 FPS animation system
5. **Touch Input Integration**: Precise weapon targeting using touchscreen
