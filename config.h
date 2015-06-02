/***
 * +==========================================================+
 * |                           NOTE                           |
 * +==========================================================+
 *
 * If you modify this file, make sure to make the entire build
 * again. Otherwise, some values will not get updated in linker
 * files.
 *
 **/

// Theoretical maximum fps, set to maybe double expected fps.
#define FPS_HARDCAP 100

// number of frames before enemies move
#define ENEMY_MS 15

// Various maximums for arrays.
#define MAX_ENEMIES_PER_CLASS 5
#define MAX_BULLETS 50

// Maximum frames to render the beam for (usually same as ENEMY_MS)
#define MAX_BEAM_FRAMES 100
#define BEAM_DAMAGE 10 // 10 damage, per frame