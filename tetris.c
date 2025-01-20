#include <gb/gb.h>
#include <gbdk/bcd.h>
#include <rand.h>
#include "BlankScreen.h"
#include "TitleTiles.h"
#include "GameTiles.h"
#include "SpriteTiles.h"
#include "hUGEDriver.h"
#include "huge_music.h"
#include "soundeffects.h"

extern const hUGESong_t huge_music;

uint16_t i,j;
char jp = 0; // joypad
char last_jp;
uint8_t frame = 0; // Multiuse timer
fixed fallingSpeed = { 0x0000 };
fixed downMovement = { 0x0000 };
const uint16_t fallingSpeeds[] = {
	0x0008, 0x000B, 0x000E, 0x0012, 0x0018,
	0x0021, 0x002D, 0x003F, 0x005B, 0x0085,
	0x00C6, 0x0131, 0x01DA, 0x0308, 0x04C3 
};
uint8_t gameMode = 1;
uint8_t musicID = 'A';
struct Tetrimino {
	char x;
	fixed y;
	uint8_t rot;
	enum {
		X,T,O,S,Z,J,L,I,W
	} color;
};
struct Tetrimino currTetr = { 0, {0}, 0, X };
struct Tetrimino movTetr  = { 0, {0}, 0, X };
struct Tetrimino ghstTetr = { 0, {0}, 0, X };

uint8_t tetrBag[14] = {T,O,S,Z,J,L,I,T,O,S,Z,J,L,I};
uint8_t *bagPtr = tetrBag;
uint8_t holdTetr = X;
uint8_t holded = 0;
uint8_t holded2 = 0;

enum {
	FALLING,
	LOCK,
	ANIMATION
} currentPhase;

struct { // based on BCD
	uint8_t f;	// frame
	uint8_t s;	// seconds
	uint8_t m;	// minutes
} gameTimer;

uint8_t matrix[24*12];
uint8_t finalMatrix[24*12];
uint8_t lines[23];
const uint8_t tetriminos[8][4][16] = {
	{
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x00,0x26,0x00,0x00, 0x26,0x26,0x26,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x26,0x00,0x00, 0x00,0x26,0x26,0x00, 0x00,0x26,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x26,0x26,0x26,0x00, 0x00,0x26,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x26,0x00,0x00, 0x26,0x26,0x00,0x00, 0x00,0x26,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x00,0x21,0x21,0x00, 0x00,0x21,0x21,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x21,0x21,0x00, 0x00,0x21,0x21,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x21,0x21,0x00, 0x00,0x21,0x21,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x21,0x21,0x00, 0x00,0x21,0x21,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
	}, {
		{0x00,0x24,0x24,0x00, 0x24,0x24,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x24,0x00,0x00, 0x00,0x24,0x24,0x00, 0x00,0x00,0x24,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x00,0x24,0x24,0x00, 0x24,0x24,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x24,0x00,0x00,0x00, 0x24,0x24,0x00,0x00, 0x00,0x24,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x22,0x22,0x00,0x00, 0x00,0x22,0x22,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x22,0x00, 0x00,0x22,0x22,0x00, 0x00,0x22,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x22,0x22,0x00,0x00, 0x00,0x22,0x22,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x22,0x00,0x00, 0x22,0x22,0x00,0x00, 0x22,0x00,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x23,0x00,0x00,0x00, 0x23,0x23,0x23,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x23,0x23,0x00, 0x00,0x23,0x00,0x00, 0x00,0x23,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x23,0x23,0x23,0x00, 0x00,0x00,0x23,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x23,0x00,0x00, 0x00,0x23,0x00,0x00, 0x23,0x23,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x00,0x00,0x25,0x00, 0x25,0x25,0x25,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x25,0x00,0x00, 0x00,0x25,0x00,0x00, 0x00,0x25,0x25,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x00,0x00, 0x25,0x25,0x25,0x00, 0x25,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x25,0x25,0x00,0x00, 0x00,0x25,0x00,0x00, 0x00,0x25,0x00,0x00, 0x00,0x00,0x00,0x00}
	}, {
		{0x00,0x00,0x00,0x00, 0x27,0x28,0x28,0x29, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00},
		{0x00,0x00,0x2A,0x00, 0x00,0x00,0x2B,0x00, 0x00,0x00,0x2B,0x00, 0x00,0x00,0x2C,0x00},
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x27,0x28,0x28,0x29, 0x00,0x00,0x00,0x00},
		{0x00,0x2A,0x00,0x00, 0x00,0x2B,0x00,0x00, 0x00,0x2B,0x00,0x00, 0x00,0x2C,0x00,0x00}
	}
};

const char SRSR[2][4][10] = {
	{
		{0,0,  0,-1,  1,-1, -2, 0, -2,-1},
		{0,0,  0,-1, -1,-1,  2, 0,  2,-1},
		{0,0,  0, 1,  1, 1, -2, 0, -2, 1},
		{0,0,  0, 1, -1, 1,  2, 0,  2, 1}
	}, {
		{0,0,  0, 1,  0,-2,  2, 1, -1,-2},
		{0,0,  0,-2,  0, 1,  1,-2, -2, 1},
		{0,0,  0, 1,  0,-2, -2,-1,  1, 2},
		{0,0,  0, 2,  0,-1, -1, 2,  2,-1}
	}
};
const char SRSL[2][4][10] = {
	{
		{0,0,  0,-1, -1,-1,  2, 0,  2,-1},
		{0,0,  0, 1,  1, 1, -2, 0, -2, 1},
		{0,0,  0, 1, -1, 1,  2, 0,  2, 1},
		{0,0,  0,-1,  1,-1, -2, 0, -2,-1}
	}, {
		{0,0,  0,-2,  0, 1,  1,-2, -2, 1},
		{0,0,  0, 1,  0,-2, -2,-1,  1, 2},
		{0,0,  0, 2,  0,-1, -1, 2,  2,-1},
		{0,0,  0, 1,  0,-2,  2, 1, -1,-2},
	}
};

const uint8_t songs[] = {8,14,20,36,40,48,50,59,7,67};
const uint8_t songBanks[] = {2,2,2,3,3,3,3,3,2,3};
uint8_t currSongBank;

void endFrame();
void drawMatrix();
void drawScore();
void drawTime();
uint8_t checkCollision(struct Tetrimino*);
void gameInput();
void pauseScreen();

uint32_t score = 0;
uint8_t level = 1;
uint8_t nLines = 0;

uint8_t lowestLine;
uint8_t lockdownTimer;
uint8_t actionsBeforeLockdown;
uint8_t lastMovWasSpin = 0;

void switchRom1() {
	SWITCH_ROM(1);
}
void switchRom2() {
	SWITCH_ROM(currSongBank);
}
void soundEffectsFunc() {
	if(*currSoundEff) {
		(**currSoundEff)();
		currSoundEff++;
	}
	if(*currNoiseEff) {
		(**currNoiseEff)();
		currNoiseEff++;
	}
}

void main() {
// Init
	DISPLAY_ON;	
	NR52_REG = 0x80;
    NR50_REG = 0x77;
    NR51_REG = 0xFF;
    set_interrupts(VBL_IFLAG);
    currSongBank = songBanks[0];
    
	__critical {
		SWITCH_ROM(2);
        hUGE_init(&huge_music);
        add_VBL(switchRom2);
        add_VBL(hUGE_dosound);
        add_VBL(switchRom1);
        add_VBL(soundEffectsFunc);
    }
	
	gameTimer.f = 0;
	gameTimer.s = 0;
	gameTimer.m = 0;
	
// Title
title:
	__critical {
		SWITCH_ROM(2);
		hUGE_init(&huge_music);
	}
	SWITCH_ROM(1);
	set_bkg_data(0, 256, titleTiles);
	set_bkg_tiles(0,0,20,18,titleMap);
    
titleLoop:
	SWITCH_ROM(1);
	if(frame==24)
		set_bkg_tiles(8,14,5,1,blankScreen);
	else if(frame==40) {
		set_bkg_tiles(8,14,5,1,titleMap+288); // "Start" text
		frame = 0;
	}
	if(!last_jp && (jp & (J_A|J_START)))
		goto menu1;
	gameTimer.f++; // For the seed
	endFrame();
	goto titleLoop;
	
// Menu 1
menu1:
	initrand(gameTimer.f);
	
	set_sprite_data(0, 16, spriteTiles);
	set_sprite_tile(0,0);
	set_sprite_tile(1,0);
	set_sprite_tile(2,0);
	set_sprite_tile(3,0);
	
	move_sprite(0, 144, 116);
	move_sprite(1, 152, 116);
	move_sprite(2, 144, 140);
	move_sprite(3, 152, 140);
    
	SWITCH_ROM(1);
	set_bkg_tiles(0,0,20,18,blankScreen);
	
	hUGE_set_position(songs[musicID-'A']);
	currSongBank = songBanks[musicID-'A'];
	
	set_bkg_data(0, 256, gameTiles);
	endFrame();
	set_bkg_tiles(0,0,20,18,menu1Map);
	
	set_tile_xy(5,5,LEFTARR);
	set_tile_xy(14,5,RIGHTARR);
	set_bkg_tiles(6,5,8,1,menuText+8*gameMode);
	
	if(musicID=='I')
		set_bkg_tiles(7,11,6,1,menuText+40);
	else {
		set_tile_xy(7,11,LETTER(musicID));
		set_bkg_tiles(8,11,5,1,menuText+32);
	}

menuModeLoop:
	SWITCH_ROM(1);
	if(frame==24)
		set_bkg_tiles(6,5,8,1,menuText);
	else if(frame==40) {
		set_bkg_tiles(6,5,8,1,menuText+8*gameMode);
		frame = 0;
	}
	if(!last_jp) {
		if(jp & J_B) {
			set_bkg_tiles(0,0,20,18,blankScreen);
			endFrame();
			frame = 0;
			goto title;
		} else if(jp & (J_A|J_START)) {
				set_bkg_tiles(0,0,20,18,blankScreen);
			endFrame();
			goto game;
		} else if(jp & J_RIGHT) {
			gameMode++;
			if(gameMode==3) gameMode=1;
				set_bkg_tiles(6,5,8,1,menuText+8*gameMode);
			frame=0;
			SFX1;
			
		} else if(jp & J_LEFT) {
			gameMode--;
			if(gameMode==0) gameMode=2;
				set_bkg_tiles(6,5,8,1,menuText+8*gameMode);
			frame=0;
			SFX1;
		} else if(jp & (J_UP|J_DOWN)) {
						set_tile_xy(5,5,0x00);
			set_tile_xy(14,5,0x00);
			set_tile_xy(6,11,LEFTARR);
			set_tile_xy(13,11,RIGHTARR);
			set_bkg_tiles(6,5,8,1,menuText+8*gameMode);
			frame=0;
			SFX1;
			endFrame();
			goto menuMusicLoop;
		}
	}
	endFrame();
	goto menuModeLoop;
	
menuMusicLoop:
	if(frame==24)
		set_bkg_tiles(7,11,6,1,menuText);
	else if(frame==40) {
		if(musicID=='I')
			set_bkg_tiles(7,11,6,1,menuText+40);
		else {
			set_tile_xy(7,11,LETTER(musicID));
			set_bkg_tiles(8,11,5,1,menuText+32);
		}
		frame = 0;
	}
	if(!last_jp) { 
		if(jp & J_B) {
			set_bkg_tiles(0,0,20,18,blankScreen);
			endFrame();
			goto title;
		} else if(jp & (J_A|J_START)) {
			set_bkg_tiles(0,0,20,18,blankScreen);
			endFrame();
			goto game;
		} else if(jp & J_RIGHT) {
			musicID++;
			if(musicID=='J') musicID = 'A';
			if(musicID=='I') {
				set_bkg_tiles(7,11,6,1,menuText+40);
			}
			else {
				set_tile_xy(7,11,LETTER(musicID));
				set_bkg_tiles(8,11,5,1,menuText+32);
			}
			currSongBank = songBanks[musicID-'A'];
			hUGE_set_position(songs[musicID-'A']);
			frame=0;
		} else if(jp & J_LEFT) {
			musicID--;
			if(musicID=='@') {
				musicID = 'I';
			set_bkg_tiles(7,11,6,1,menuText+40);
			} else {
				set_tile_xy(7,11,LETTER(musicID));
				set_bkg_tiles(8,11,5,1,menuText+32);
			}
			currSongBank = songBanks[musicID-'A'];
			currSongBank = songBanks[musicID-'A'];
			hUGE_set_position(songs[musicID-'A']);
			frame=0;
		} else if(jp & (J_UP|J_DOWN)) {
			set_tile_xy(5,5,LEFTARR);
			set_tile_xy(14,5,RIGHTARR);
			set_tile_xy(6,11,0x00);
			set_tile_xy(13,11,0x00);
			if(musicID=='I')
				set_bkg_tiles(7,11,6,1,menuText+40);
			else {
				set_tile_xy(7,11,LETTER(musicID));
				set_bkg_tiles(8,11,5,1,menuText+32);
			}
			frame=0;
			SFX1;
			endFrame();
			goto menuModeLoop;
		}
	}
	endFrame();
	goto menuMusicLoop;

game:
	set_bkg_tiles(0,0,20,18,gameMap);
	if(gameMode==1)
		set_bkg_tiles(14,6,5,1,menuText+48);
	else {
		set_bkg_tiles(14,6,5,1,menuText+56);
		set_tile_xy(15,7,NUMBER(0));
		set_tile_xy(16,7,0x7C);
		set_tile_xy(17,7,NUMBER(0));
		set_tile_xy(18,7,NUMBER(0));
	}
	
	for(i=0;i<12*24;i++)
		matrix[i] = gameMatrix[i];
	fallingSpeed.w = fallingSpeeds[0];
	
	nLines = 0;
	level = 1;
	drawScore();

	bagPtr = tetrBag;
	uint8_t *b;
	uint8_t *B = tetrBag+7;
	uint8_t *B2= tetrBag+14;
	initrand(rand()+gameTimer.f);
	for(b=tetrBag; b<B; b++) {
		uint8_t temp = *b;
		uint8_t *b2 = tetrBag+rand()%7;
		temp = *b;
		*b = *b2;
		*b2 = temp;
	}
	for(b=B; b<B2; b++) {
		uint8_t temp = *b;
		uint8_t *b2 = B+rand()%7;
		temp = *b;
		*b = *b2;
		*b2 = temp;
	}
	holdTetr = X;
	
	gameTimer.f = 0;
	gameTimer.s = 0;
	gameTimer.m = 0;
	
// Generation Phase
generation:
	currTetr.x = 4;
	currTetr.y.h = 3;
	currTetr.rot = 0;
	currTetr.color = *bagPtr++;
	if(checkCollision(&currTetr)) {
		frame = 0;
		goto gameover;
	}
	currTetr.y.h = 4;
	if(checkCollision(&currTetr))
		currTetr.y.h = 3;
	lockdownTimer = 15;
	actionsBeforeLockdown = 14;
	lowestLine = currTetr.y.h;
	// Shuffle bags
	if(bagPtr >= B2) {
		bagPtr = tetrBag;
		for(b=B; b<B2; b++) {
			uint8_t temp = *b;
			uint8_t *b2 = B+rand()%7;
			temp = *b;
			*b = *b2;
			*b2 = temp;
		}
	} else if (bagPtr == B2)
		for(b=tetrBag; b<B; b++) {
			uint8_t temp = *b;
			uint8_t *b2 = tetrBag+rand()%7;
			temp = *b;
			*b = *b2;
			*b2 = temp;
		}
	set_sprite_tile(0,*bagPtr);
	set_sprite_tile(1,*bagPtr+8);
	frame=0;
	lastMovWasSpin = 0;
	currentPhase = FALLING;
	goto falling;
hold:
	if(!*currSoundEff || currSoundEff<soundEffects+33)
		SFX3;
	drawMatrix();
	endFrame();
	if(holdTetr==X)
		goto firstHold;
	currTetr.x = 4;
	currTetr.y.h = 4;
	currTetr.rot = 0;
	if(checkCollision(&currTetr))
		currTetr.y.h = 3;
	lockdownTimer = 15;
	actionsBeforeLockdown = 14;
	holded = currTetr.color;
	currTetr.color = holdTetr;
	holdTetr = holded;
	set_sprite_tile(2,holded);
	set_sprite_tile(3,holded+8);
	frame=0;
	lastMovWasSpin = 0;
	currentPhase = FALLING;
	goto falling;
firstHold:
	holdTetr = currTetr.color;
	holded = holdTetr;
	set_sprite_tile(2,holded);
	set_sprite_tile(3,holded+8);
	goto generation;
// Falling Phase
falling:
	if((jp&J_SELECT) && !holded && !(last_jp&J_SELECT))
		goto hold;
	gameInput();
	if((jp&J_UP) && !(last_jp&J_UP)) {
		currTetr = ghstTetr;
		lastMovWasSpin = 0;
		goto lockdown;
	}
		
	if(jp&J_DOWN) {
		if(!(last_jp&J_DOWN)) {
			if(!*currSoundEff)
				SFX2;
			lastMovWasSpin = 0;
		}
		downMovement.w = level==1 ? 170 : fallingSpeed.w*20;
	} else {
		downMovement.w = fallingSpeed.w;
		if(level==1 && gameTimer.f&1) // At level one falling speed should be 8 and half
			downMovement.w++;
	}
	currTetr.y.w += downMovement.l;
	do {
		if(checkCollision(&currTetr)) {
			currTetr.y.h--;
			drawMatrix();
			endFrame();
			currentPhase = LOCK;
			goto lock;
		}
		if(downMovement.h==0)
			break;
		downMovement.h--;
		currTetr.y.h++;
	} while(1);
	
	if(currTetr.y.h > lowestLine) {
		lowestLine = currTetr.y.h;
		actionsBeforeLockdown = 14;
		lockdownTimer = 15;
	}
	drawMatrix();
	endFrame();
	goto falling;
	
// Lock Phase
lock:
	if((jp&J_SELECT) && !holded && !(last_jp&J_SELECT))
		goto hold;
	gameInput();
	currTetr.y.h++;
	if(!checkCollision(&currTetr)) {
		currTetr.y.h--;
		frame = 0;
		drawMatrix();
		endFrame();
		currentPhase = FALLING;
		goto falling;
	}
	currTetr.y.h--;
	if(!lockdownTimer)
		goto lockdown;
	if((jp&J_UP) && !(last_jp&J_UP))
		goto lockdown;
	lockdownTimer--;
	drawMatrix();
	endFrame();
	goto lock;
lockdown:
	SFX4;
	// Place Tetrimino
	const uint8_t *t = tetriminos[currTetr.color][currTetr.rot];
	uint8_t *m = matrix + 12*currTetr.y.h + currTetr.x; // Matrix pointer
	for(i=4;i;i--) {
		if(*t) *m = *t;
		t++; m++;
		if(*t) *m = *t;
		t++; m++;
		if(*t) *m = *t;
		t++; m++;
		if(*t) *m = *t;
		t++; m+=9;
	}
	drawMatrix();
	endFrame();
	holded = 0;

// Pattern Phase
	uint8_t *lp; // Line pointer
	for(lp=lines+2,m=matrix+25; lp<lines+23;lp++) {
		*lp = 1;
		uint8_t *M = m+10;
		while(m<M) {
			*lp = (*lp)&&(*m);
			m++;
		}
		m += 2;
	}
	uint8_t clearedLines = 0;
	for(lp=lines+2;lp<lines+23;lp++)
		clearedLines += *lp;
	
	m = matrix + 12*currTetr.y.h + currTetr.x;
	uint8_t tcorners = (*m>0) + (*(m+2)>0) + (*(m+24)>0) + (*(m+26)>0);
	
	if(clearedLines>0) {
		frame = 0;
		if(clearedLines>3)
			SFX7;
		else if (currTetr.color==T && lastMovWasSpin && tcorners>=3)
			SFX7;
		else
			SFX5;
		if(gameMode==2)
			goto elimination;
		goto animate;
	}
	goto generation;

// Animation Phase
animate:
	currentPhase = ANIMATION;
	uint8_t *fm = finalMatrix;
	uint8_t *M = matrix + 12*24; // End of matrix
	m = matrix;
	while(m<M)
		*fm++ = *m++;
	if(frame&4) { // Paint gray
		uint8_t *lp = lines+2;
		fm = finalMatrix+25; 
		while(lp<lines+23) {
			if(*lp) { // If line is cleared
				for(i=10;i;i--)
					*fm++ = 45; // Gray block
				fm += 2;
			} else // Skip
				fm += 12;
			lp++;
		}
	}
	set_bkg_tiles(1,0,12,18,finalMatrix+60);
	// Check pause
	if((jp&J_START) && !(last_jp&J_START))
		pauseScreen();
	endFrame();
	if(frame <= 24)
		goto animate;
// Elimination Phase
elimination:
	;
	uint8_t levelUp = 0;
	for(lp=lines+2,m=matrix+25; lp<lines+23;lp++) {
		if(*lp) {
			uint8_t cont = 1; // continue
			uint8_t *M1 = m;
			uint8_t *M2 = m+10;
			uint8_t *m2;
			uint8_t *m3 = m;
			while(cont) {
				cont = 0;
				for(m=M1,m2=M1-12;m<M2;m++,m2++) {
					cont = cont||*m2; // Check if upper line is empty
					*m = *m2;
				}
				M1 -= 12;
				M2 -= 12;
			}
			m = m3+12;
			if(nLines<255)
				nLines++;
			if(nLines%10==0)
				levelUp = 1;
		} else
			m += 12;
	}

// Completion Phase
	if(gameMode==2)
		SFX6;
	drawScore();
	for(i=8;i;i--) {
		// Check pause
		if((jp&J_START) && !(last_jp&J_START))
			pauseScreen();
		endFrame();
	}
	if(gameMode==2)
		goto generation;
	SFX6;
	if(/*gameMode==1 && */levelUp && level<100) {
		SFX8;
		if(level<15)
			fallingSpeed.w = fallingSpeeds[level];
		else if(fallingSpeed.w<0x1000)
			fallingSpeed.w = (fallingSpeed.w/8)*11;
		level++;
		if(level==100)
			set_tile_xy(15,10,NUMBER(1));
	}
	drawScore();
	goto generation;
	// Game Over
gameover:
	currSongBank = 2;
	hUGE_set_position(7);
	SFX10;
	for(i=18;i;) {
		i--;
		set_bkg_tiles(2,i,10,1,gameOverRow);
		endFrame();
	}
	while(1) {
		if(jp & (J_A|J_B|J_START))
			break;
		endFrame();
	}
	SFX0;
	goto menu1;
}
void gameInput() {
	if(jp&J_RIGHT) {
		if(!(last_jp&J_RIGHT)) {
			frame = 0;
			currTetr.x++;
			if(checkCollision(&currTetr))
				currTetr.x--;
			else {
				if(!*currSoundEff)
					SFX2;
				lastMovWasSpin = 0;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
			}
		} else if(frame>7) {
			currTetr.x++;
			if(checkCollision(&currTetr))
				currTetr.x--;
			else {
				lastMovWasSpin = 0;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
			}
				
		}
		frame++;
	}
	if(jp&J_LEFT) {
		if(!(last_jp&J_LEFT)) {
			frame = 0;
			currTetr.x--;
			if(checkCollision(&currTetr))
				currTetr.x++;
			else {
				if(!*currSoundEff)
					SFX2;
				lastMovWasSpin = 0;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
			}
		} else if(frame>7) {
			currTetr.x--;
			if(checkCollision(&currTetr))
				currTetr.x++;
			else {
				lastMovWasSpin = 0;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
			}
				
		}
		frame++;
	}
	if((jp&J_A) && !(last_jp&J_A)) {
		movTetr = currTetr;
		movTetr.rot++;
		movTetr.rot &= 3;
		const char *srs = SRSR[movTetr.color==I][movTetr.rot];
		const char *end = srs + 10;
		while(srs<end) {
			movTetr.y.h = currTetr.y.h + *srs++;
			movTetr.x = currTetr.x + *srs++;
			if(!checkCollision(&movTetr)) {
				currTetr = movTetr;
				if(!*currSoundEff || currSoundEff<soundEffects+33)
					SFX3;
				lastMovWasSpin = 1;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
				break;
			}
		}
	}
	if((jp&J_B) && !(last_jp&J_B)) {
		movTetr = currTetr;
		movTetr.rot--;
		movTetr.rot &= 3;
		const char *srs = SRSL[movTetr.color==I][movTetr.rot];
		const char *end = srs + 10;
		while(srs<end) {
			movTetr.y.h = currTetr.y.h - *srs++;
			movTetr.x = currTetr.x - *srs++;
			if(!checkCollision(&movTetr)) {
				currTetr = movTetr;
				if(!*currSoundEff || currSoundEff<soundEffects+33)
					SFX3;
				lastMovWasSpin = 1;
				if(currentPhase==LOCK && actionsBeforeLockdown) {
					lockdownTimer = 15;
					actionsBeforeLockdown--;
				}
				break;
			}
		}
	}
	if((jp&J_START) && !(last_jp&J_START))
		pauseScreen();
	ghstTetr = currTetr;
	while(!checkCollision(&ghstTetr))
		ghstTetr.y.h++;
	ghstTetr.y.h--;
}

inline void endFrame() {	
// Update Switches
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
// Wait until next frame
	wait_vbl_done();
	last_jp = jp;
	jp = joypad();
	frame ++;
	gameTimer.f++;
}

uint8_t checkCollision(struct Tetrimino *tetrimino) {
	const uint8_t *t = tetriminos[tetrimino->color][tetrimino->rot];
	const uint8_t *T = t + 16;
	uint8_t *m = matrix + 12*tetrimino->y.h + tetrimino->x;
	
	while(t<T) {
		if(*t && *m) return 1;
		t++; m++;
		if(*t && *m) return 1;
		t++; m++;
		if(*t && *m) return 1;
		t++; m++;
		if(*t && *m) return 1;
		t++; m+=9;
	}
	return 0;
}

void drawTime() {
	if(gameTimer.f<30)
		return;
	if(gameTimer.m==0x99 && gameTimer.s==0x59)
		return;
	gameTimer.f = 0;
	gameTimer.s++;
	if((gameTimer.s&0x0F)==0x0A)
		gameTimer.s += 6;
	if(gameTimer.s==0x60) {
		gameTimer.s = 0;
		gameTimer.m++;
		if((gameTimer.m&0x0F)==0x0A)
			gameTimer.m += 6;
		set_tile_xy(15,7,NUMBER(gameTimer.m&0x0F));
		if(gameTimer.m >= 0x10)
			set_tile_xy(14,7,NUMBER(gameTimer.m>>4));
	}
	set_tile_xy(18,7,NUMBER(gameTimer.s&0x0F));
	set_tile_xy(17,7,NUMBER(gameTimer.s>>4));
}

void drawMatrix() {
	uint8_t *fm = finalMatrix+60;
	uint8_t *m = matrix+60;
	uint8_t *M = matrix+12*24;
	while(m<M)
		*fm++ = *m++;
	const uint8_t *t, *T;
	// Ghost
	fm = finalMatrix + 12*ghstTetr.y.h + ghstTetr.x;
	t = tetriminos[ghstTetr.color][ghstTetr.rot];
	T = t + 16;
	while(t<T) {
		if(*t) *fm = 0x2D;
		t++; fm++;
		if(*t) *fm = 0x2D;
		t++; fm++;
		if(*t) *fm = 0x2D;
		t++; fm++;
		if(*t) *fm = 0x2D;
		t++; fm+=9;
	}
	// Tetrimino
	fm = finalMatrix + 12*currTetr.y.h + currTetr.x;
	t = tetriminos[currTetr.color][currTetr.rot];
	T = t + 16;
	while(t<T) {
		if(*t) *fm = *t;
		t++; fm++;
		if(*t) *fm = *t;
		t++; fm++;
		if(*t) *fm = *t;
		t++; fm++;
		if(*t) *fm = *t;
		t++; fm+=9;
	}
	set_bkg_tiles(1,0,12,18,finalMatrix+60);
	if(gameMode==2)
		drawTime();
}

void drawScore() {
	BCD bcd;
	uint8_t len;
	uint8_t buffer[8];
	set_tile_xy(19,3,NUMBER(0));
	
	uint2bcd(nLines, &bcd);
	bcd2text(&bcd, NUMBER(0), buffer);
	len = nLines<10 ? 1 : (nLines<100 ? 2 : 3);
	
	set_bkg_tiles(18-len, 10, len, 1, buffer+8-len);
	
	if(gameMode!=1)
		return;
		
	uint2bcd(level, &bcd);
	bcd2text(&bcd, NUMBER(0), buffer);
	len = level<10 ? 1 : 2;
	set_bkg_tiles(18-len, 7, len, 1, buffer+8-len);
}

void pauseScreen() {
	__critical {
        remove_VBL(switchRom2);
        remove_VBL(hUGE_dosound);
    }
    SFX9;
	uint8_t *fm = finalMatrix+60;
	uint8_t *M = finalMatrix + 12*23;
	while(fm<M) {
		fm++;
		for(i=10;i;i--)
			*fm++ = 0x00;
		fm++;
	}
	set_bkg_tiles(1,0,12,18,finalMatrix+60);
	endFrame();
pause:
	if((jp&J_START) && !(last_jp&J_START)) {
		__critical {
		    remove_VBL(switchRom1);
		    remove_VBL(soundEffectsFunc);
		    add_VBL(switchRom2);
		    add_VBL(hUGE_dosound);
		    add_VBL(switchRom1);
		    add_VBL(soundEffectsFunc);
		}
		gameTimer.f--;
		endFrame();
		return;
	}
	gameTimer.f--;
	endFrame();
	goto pause;
}
