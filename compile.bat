..\..\bin\lcc -c -o tetris.o tetris.c
:: ..\..\bin\lcc -c -o TitleTiles.o TitleTiles.c
:: ..\..\bin\lcc -c -o GameTiles.o GameTiles.c
:: ..\..\bin\lcc -c -o BlankScreen.o BlankScreen.c
:: ..\..\bin\lcc -c -o SpriteTiles.o SpriteTiles.c

..\..\bin\lcc -Wl-yt0x1A -Wl-yo4 -Wl-ya4 -o tetris.gb tetris.o TitleTiles.o GameTiles.o BlankScreen.o SpriteTiles.o hUGEDriver.obj.o huge_music.c
