/* #pragma bank 1 */
#include <gb/gb.h>
#include "hUGEDriver.h"

unsigned char samples[] = { 
	0x01,0x23,0x44,0x46,0x8A,0xAA,0xAC,0xEE,0xFC,0x79,0xB8,0x6B,0xFC,0x98,0x76,0x42
};
unsigned char saved[16];
void nop() {}
void stop() {
	NR10_REG = 0x00; 
	NR11_REG = 0x00;
	NR12_REG = 0x00;
	NR13_REG = 0x00;
	NR14_REG = 0x00;
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH1, HT_CH_PLAY);
	/*NR30_REG = 0x00; 
	NR31_REG = 0x00;
	NR32_REG = 0x00;
	NR33_REG = 0x00;
	NR34_REG = 0x00;*/
	hUGE_mute_channel(HT_CH3, HT_CH_PLAY);
	SWITCH_ROM(1);
}
extern void savewave();
void sfx1() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
	SWITCH_ROM(1);
	
	NR10_REG = 0x00; 
	NR11_REG = 0x80;
	NR12_REG = 0x80;
	NR13_REG = 0x80;
	NR14_REG = 0x87;
	
	/*unsigned char *dst = (unsigned char *)(0xFF30);
	__critical {
	
		for(unsigned char n=8;n;n--) {
			*dst = *dst;
			dst++;
		}
	}*/
/* 	*(unsigned char *)(0xFF30) = *(unsigned char *)(0xFF30); */
/* 	*(unsigned char *)(0xFF31) = *(unsigned char *)(0xFF31); */
/* 	*(unsigned char *)(0xFF32) = *(unsigned char *)(0xFF32); */
/* 	*(unsigned char *)(0xFF33) = *(unsigned char *)(0xFF33); */
/* 	*(unsigned char *)(0xFF34) = *(unsigned char *)(0xFF34); */
/* 	*(unsigned char *)(0xFF35) = *(unsigned char *)(0xFF35); */
/* 	*(unsigned char *)(0xFF36) = *(unsigned char *)(0xFF36); */
/* 	*(unsigned char *)(0xFF37) = *(unsigned char *)(0xFF37); */
	// savewave();
}

void sfx2() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
	SWITCH_ROM(1);
	
	NR10_REG = 0x00; 
	NR11_REG = 0x80;
	NR12_REG = 0x40;
	NR13_REG = 0x66;
	NR14_REG = 0x87;
}

void sfx3() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
	SWITCH_ROM(1);
	
	NR10_REG = 0x00; 
	NR11_REG = 0x80;
	NR12_REG = 0xD0;
	NR13_REG = 0x88;
	NR14_REG = 0x87;
}
void sfx3_1() {
	NR12_REG = 0xC0;
	NR13_REG = 0x9B;
	NR14_REG = 0x86;
}
void sfx3_2() {
	NR12_REG = 0xB0;
	NR13_REG = 0x82;
	NR14_REG = 0x87;
}
void sfx3_3() {
	NR12_REG = 0xA0;
	NR13_REG = 0x8C;
	NR14_REG = 0x86;
}
void sfx3_4() {
	NR12_REG = 0x80;
	NR13_REG = 0x40;
	NR14_REG = 0x87;
}
void sfx3_5() {
	NR12_REG = 0x70;
	NR13_REG = 0x56;
	NR14_REG = 0x86;
}
void sfx3_6() {
	NR12_REG = 0x60;
	NR13_REG = 0x82;
	NR14_REG = 0x87;
}
void sfx3_7() {
	NR12_REG = 0x50;
	NR13_REG = 0x8F;
	NR14_REG = 0x86;
}
void sfx3_8() {
	NR12_REG = 0x40;
	NR13_REG = 0x40;
	NR14_REG = 0x87;
}
void sfx3_9() {
	NR12_REG = 0x30;
	NR13_REG = 0x5F;
	NR14_REG = 0x86;
}
void sfx3_10() {
	NR12_REG = 0x20;
	NR13_REG = 0x7F;
	NR14_REG = 0x87;
}
void sfx3_11() {
	NR12_REG = 0x10;
	NR13_REG = 0x95;
	NR14_REG = 0x86;
}

fixed hiPitch;
fixed loPitch;
unsigned char volume;
void sfx4_1() {
	NR10_REG = 0x1F; 
	NR11_REG = 0x80;
	NR12_REG = volume;
	NR13_REG = hiPitch.l;
	NR14_REG = hiPitch.h;
	hiPitch.w += 0x001E;
}
void sfx4_2() {
	NR10_REG = 0x1F; 
	NR11_REG = 0x80;
	NR12_REG = volume;
	NR13_REG = loPitch.l;
	NR14_REG = loPitch.h;
	loPitch.w += 0x001E;
	volume -= 11;
}
void sfx4() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
	hUGE_mute_channel(HT_CH3, HT_CH_PLAY);
	SWITCH_ROM(1);
	
	hiPitch.w = 0x85D8;
	loPitch.w = 0x85A9;
	volume = 0xB7;
	sfx4_1();
}

void sfx5_1();
void sfx5() {
	hUGE_mute_channel(HT_CH1, HT_CH_MUTE);
	sfx5_1();
}
void sfx5_1() {
	NR10_REG = 0x00;
	NR11_REG = 0x81;
	NR12_REG = 0xC2;
	NR13_REG = 0xC1;
	NR14_REG = 0x87;
}
void sfx5_2() {
	NR10_REG = 0x00;
	NR11_REG = 0x81;
	NR12_REG = 0xC2;
	NR13_REG = 0xAD;
	NR14_REG = 0x87;
}
fixed tetrPitch;
void sfx5_3() { // Loads wave
	unsigned char *dst = (unsigned char *)(0xFF30); // Pointer to the wave RAM.
    unsigned char *src = saved;
	for(unsigned char n=16;n;n--) {
		*src++ = *dst++;
	}
	dst = (unsigned char *)(0xFF30);
    src = samples;
	for(unsigned char n=16;n;n--)
		*dst++ = *src++;
	tetrPitch.w = 0xC798;
	hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
	hUGE_reset_wave();	
}
void sfx5_4() {
	NR30_REG = 0x80;
	NR31_REG = 0x00;
	NR32_REG = 0x20;
	NR33_REG = tetrPitch.l;
	NR34_REG = tetrPitch.h;
	tetrPitch.w += 0x0003;
}
void sfx5_5() {
	tetrPitch.w = 0xC798;
	NR30_REG = 0x80;
	NR31_REG = 0x00;
	NR32_REG = 0x20;
	NR33_REG = tetrPitch.l;
	NR34_REG = tetrPitch.h;
}
void sfx5_6() {
	NR30_REG = 0x80;
	NR31_REG = 0x00;
	NR32_REG = 0x20;
	NR33_REG = tetrPitch.l;
	NR34_REG = tetrPitch.h;
	tetrPitch.w -= 0x0003;
}
void sfx5_7() {
	NR30_REG = 0x00;
	NR31_REG = 0x00;
	NR32_REG = 0x00;
	NR33_REG = 0x00;
	NR34_REG = 0x00;
	
	hUGE_mute_channel(HT_CH1, HT_CH_PLAY);
	hUGE_mute_channel(HT_CH3, HT_CH_PLAY);
}

void sfx6() {
	NR10_REG = 0x00;
	NR11_REG = 0x80;
	NR12_REG = 0xC1;
	NR13_REG = 0xB6;
	NR14_REG = 0x87;
}
void sfx6_1() {
	NR11_REG = 0x80;
	NR12_REG = 0xC1;
	NR13_REG = 0xC4;
	NR14_REG = 0x87;
}
void sfx6_2() {
	NR11_REG = 0x80;
	NR12_REG = 0xC1;
	NR13_REG = 0xCE;
	NR14_REG = 0x87;
}
void sfx6_3() {
	NR11_REG = 0x80;
	NR12_REG = 0xC1;
	NR13_REG = 0xDB;
	NR14_REG = 0x87;
}
void sfx6_4() {
	NR11_REG = 0x80;
	NR12_REG = 0xC1;
	NR13_REG = 0xB6;
	NR14_REG = 0x87;
}

void sfx7_1();
void sfx7() {
	NR10_REG = 0x00;
	
	NR21_REG = 0x00;
	NR22_REG = 0x00;
	NR23_REG = 0x00;
	NR24_REG = 0x00;
	
	NR30_REG = 0x00;
	NR31_REG = 0x00;
	NR32_REG = 0x00;
	NR33_REG = 0x00;
	NR34_REG = 0x00;
	
	NR30_REG = 0x00;
	NR31_REG = 0x00;
	NR32_REG = 0x00;
	NR33_REG = 0x00;
	NR34_REG = 0x00;
	
	NR41_REG = 0x00;
	NR42_REG = 0x00;
	NR43_REG = 0x00;
	NR44_REG = 0x00;
	
	sfx7_1();
}
void sfx7_1() {
	NR11_REG = 0x80;
	NR12_REG = 0xC0;
	NR13_REG = 0x83;
	NR14_REG = 0x87;
}
void sfx7_2() {
	NR11_REG = 0x80;
	NR12_REG = 0xC0;
	NR13_REG = 0xC1;
	NR14_REG = 0x87;
}


unsigned char samplesGameOver[] = {
	0xA1,0x82,0x23,0x34,0x45,0x56,0x67,0x78,0x89,0x9A,0xAB,0xBC,0xCD,0x64,0x32,0x10
};
unsigned char sfx8array[] = {
	0xD6,0xD8,0xDA,0xDC,0xEF,0xF1,0xE3,0xE6,0xE8,0xEA,0xEC,0xEF,0xF1,0xE3,0xE6,0xE8,
	0xEB,0xED,0xDF,0xDF,0xD2,0xD4,0xD6,0xD9,0xCB,0xCD,0xC0,0xC2,0xC4,0xC4,0xC4,0xC4
};

char *gameOverIndex;
void sfx8() {
	stop();
	stopNoise();
	unsigned char *dst = (unsigned char *)(0xFF30); // Pointer to the wave RAM.
    unsigned char *src = samplesGameOver;
	for(unsigned char n=16;n;n--)
		*dst++ = *src++;
	gameOverIndex = sfx8array;
	hUGE_mute_channel(HT_CH3, HT_CH_MUTE);
	hUGE_reset_wave();	
	
	NR30_REG = 0x80;
	NR31_REG = 0x00;
	NR32_REG = 0x20;
	NR33_REG = *gameOverIndex;
	NR34_REG = 0xC6;
}
void sfx8_1() {
	++gameOverIndex;
	NR33_REG = *gameOverIndex;
}
void sfx8_2() {
	NR30_REG = 0x00;
	NR31_REG = 0x00;
	NR32_REG = 0x00;
	NR33_REG = 0x00;
	NR34_REG = 0x00;
}


void (*soundEffects[])(void) = {
	NULL,
// 1
	sfx1, nop, nop, stop, NULL, 
// 6	
	sfx2, nop, stop, NULL,
// 10
	sfx3, nop, sfx3_1, nop, sfx3_2, nop, sfx3_3,
	sfx3_4, nop, sfx3_5, nop, sfx3_6, nop, sfx3_7,
	sfx3_8, nop, sfx3_9, nop, sfx3_10, nop, sfx3_11, nop, stop, NULL,
// 34
	sfx4, nop, nop, sfx4_2, nop, nop, sfx4_1, nop, nop, sfx4_2, nop, nop,
	sfx4_1, nop, nop, sfx4_2, nop, nop, sfx4_1, nop, nop, sfx4_2, nop, nop,
	sfx4_1, nop, nop, sfx4_2, nop, nop, sfx4_1, nop, nop, sfx4_2, nop, nop,
	sfx4_1, nop, nop, sfx4_2, nop, nop, sfx4_1, nop, nop, sfx4_2, nop, nop,
	sfx4_1, nop, nop, sfx4_2, nop, nop, sfx4_1, nop, nop, sfx4_2, nop, nop,
	sfx4_1, nop, nop, stop, NULL,
// 99
	sfx5, nop, nop, nop, nop, sfx5_2, nop, nop, nop, nop, nop, nop, 
	sfx5_1, nop, nop, nop, nop, sfx5_2, nop, nop, nop, nop, nop, nop, nop, nop, sfx5_3, 
	sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4, sfx5_4,
	sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5, sfx5_5,
	sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_6, sfx5_7, NULL,
// 157
	sfx6, nop, nop, nop, stop, nop, nop, sfx6_1, nop, nop, nop, stop, nop, nop,
	sfx6_2, nop, nop, nop, stop, nop, nop, sfx6_3, nop, nop, nop, stop, nop, nop,
	sfx6_4, nop, nop, nop, stop, NULL,
// 191
	sfx7, nop, nop, stop, nop, nop, nop, sfx7_2, nop, nop, stop, nop, nop, nop,
	sfx7_1, nop, nop, stop, nop, nop, nop, sfx7_2, nop, nop, stop, NULL, 
// 217
	sfx8, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1,
	sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1, sfx8_1,
	sfx8_2, NULL
// 247
};


void nse1() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
	SWITCH_ROM(1);
	
	NR41_REG = 0x02;
	NR42_REG = 0xD1;
	NR43_REG = 0x45;
	NR44_REG = 0xC0;
	
	NR51_REG = 0xFF;
}
void nse2() {
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH4, HT_CH_MUTE);
	SWITCH_ROM(1);
	
	NR41_REG = 0x02;
	NR42_REG = 0xD1;
	NR43_REG = 0x80;
	NR44_REG = 0x80;
	
	NR51_REG = 0xFF;
}
void stopNoise() {
	NR41_REG = 0x00;
	NR42_REG = 0x00;
	NR43_REG = 0x00;
	NR44_REG = 0x00;
	SWITCH_ROM(2);
	hUGE_mute_channel(HT_CH4, HT_CH_PLAY);
	SWITCH_ROM(1);
}

void (*noiseEffects[])(void) = {
	NULL,
	nse1, nop, nop, nop, nop, nop, nop, nop, nop, nop, stopNoise, NULL,
	nse2, nop, nop, nop, nop, nop, nop, nop, nop, nop, stopNoise, NULL
};
