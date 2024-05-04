#ifndef GAME4_H_
#define GAME4_H_

#define GAME4_NAME "Dino Run"

void game4_start( void );

void OLED_DrawBMP( unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[] );
void OLED_DrawBMPFast( const unsigned char BMP[] );
void oled_drawbmp_block_clear( int bx, int by, int clear_size );
void OLED_DrawGround();
void OLED_DrawCloud();
void OLED_DrawDino();
void OLED_DrawCactus();
int  OLED_DrawCactusRandom( unsigned char ver, unsigned char reset );
int  OLED_DrawDinoJump( char reset );
void OLED_DrawRestart();
void OLED_DrawCover();

#endif
