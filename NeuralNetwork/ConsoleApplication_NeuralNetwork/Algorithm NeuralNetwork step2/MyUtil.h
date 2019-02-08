#ifndef _Util_h_
#define _Util_h_

#include <stdio.h>
#include <conio.h>
#include <windows.h>

enum EConsoleTextColor {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
    WHITE,
};//enum EConsoleTextColor

inline void GotoXy( int x, int y ) {
    COORD p = { x, y };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
}//GotoXy()

inline void SetColor( EConsoleTextColor iForegroundColor_, bool bForegroundLighten_
    , EConsoleTextColor iBackgroundColor_, bool bBackgroundLighten_ ) {
    WORD  wForeground = iForegroundColor_;
    if( bForegroundLighten_ == true )
        wForeground |= 0x8;
    WORD wBackground = iBackgroundColor_ << 4;
    if( bBackgroundLighten_ == true )
        wBackground |= 0x80;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), wForeground | wBackground );
}//SetColor()

//void main() {
//    //system( "CLS" );
//    GotoXy( 5, 5 );
//    SetColor( RED, true, BLUE, false );
//    printf( "Hello\r\n" );
//    GotoXy( 15, 7 );
//    SetColor( RED, true, BLUE, false );
//    printf( "World\r\n" );
//}//main()

#endif // _Util_h_
