/*

 TITLETILES.H

 Include File.

 Info:
  Form                 : All tiles as one unit.
  Format               : Gameboy 4 color.
  Compression          : None.
  Counter              : None.
  Tile size            : 8 x 8
  Tiles                : 0 to 127

  Palette colors       : None.
  SGB Palette          : None.
  CGB Palette          : None.

  Convert to metatiles : No.

 This file was generated by GBTD v2.2

*/


/* Bank of tiles. */
#define TileLabelBank 1
/* Start of tile array. */
extern const unsigned char titleTiles[];

/* End of TITLETILES.H */
/*

 TITLEMAP.H

 Map Include File.

 Info:
   Section       : 
   Bank          : 0
   Map size      : 20 x 18
   Tile set      : Title.gbr
   Plane count   : 0.5 plane (4 bits)
   Plane order   : Tiles are continues
   Tile offset   : 0
   Split data    : No

 This file was generated by GBMB v1.8

*/

#define titleMapWidth 20
#define titleMapHeight 18
#define titleMapBank 1

extern const unsigned char titleMap[];

/* End of TITLEMAP.H */
