extern void (*soundEffects[])(void);
void (**currSoundEff)(void) = soundEffects;

extern void (*noiseEffects[])(void);
void (**currNoiseEff)(void) = noiseEffects;

#define SFX1 currSoundEff=soundEffects+1	// Menu
#define SFX2 currSoundEff=soundEffects+6	// Move
#define SFX3 currSoundEff=soundEffects+10	// Rotate
#define SFX4 currNoiseEff=noiseEffects+1	// Place
#define SFX5 currSoundEff=soundEffects+34	//;currNoiseEff=noiseEffects+1 // Line
#define SFX6 currNoiseEff=noiseEffects+13	// After line

#define SFX7 currSoundEff=soundEffects+99	// Tetris
#define SFX8 currSoundEff=soundEffects+157	// Level Up
#define SFX9 currSoundEff=soundEffects+191	// Pause

#define SFX10 currSoundEff=soundEffects+217	// Game Over

#define SFX0 currSoundEff=soundEffects+4
