CC	= ../../bin/lcc -Wa-l -Wl-m -Wl-j
ROMUSE = ../../tools/romusage
EMU = vba

all:	tetris.gb

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.a

test:
	$(ROMUSE) tetris.gb -a
	$(EMU) tetris.gb -4 --config=vba.cfg

%.o: %.c
	$(CC) -c -o $@ $<

tetris.gb:	tetris.o TitleTiles.o GameTiles.o BlankScreen.o SpriteTiles.o huge_music.o huge_music_2.o huge_music_3.o huge_music_4.o soundeffects.o
	$(CC) -Wl-yt0x1A -Wl-yo4 -Wl-ya4 -Wl-l./hUGEDriver.lib -o tetris.gb tetris.o TitleTiles.o GameTiles.o BlankScreen.o SpriteTiles.o huge_music.o huge_music_2.o huge_music_3.o huge_music_4.o soundeffects.o
	$(ROMUSE) $@


