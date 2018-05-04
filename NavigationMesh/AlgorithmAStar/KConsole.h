// jintaeks on 2017-11-15_21-36
#pragma once

#include <windows.h>
#include <conio.h>
#include <stdio.h>


class KConsole
{
private:
    HANDLE              m_hConsole;

public:
    KConsole( SHORT width = 80, SHORT height = 40, const char* title = "console" )
    {
        m_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        SetScreenSize( width, height );
        SetTitle( title );
    }

    ~KConsole()
    {
        SetColor( 7, 0 );
        ShowCursor( true );
    }

    void ShowCursor( bool bShow )
    {
        CONSOLE_CURSOR_INFO cci;

        GetConsoleCursorInfo( m_hConsole, &cci );
        cci.bVisible = bShow;
        SetConsoleCursorInfo( m_hConsole, &cci );
    }

    void SetTitle( const char* title )
    {
        SetConsoleTitleA( title );
    }

    void SetScreenSize( SHORT width, SHORT height )
    {
        COORD size;

        size = GetLargestConsoleWindowSize( m_hConsole );
        size.X = width;
        size.Y = height;
        SetConsoleScreenBufferSize( m_hConsole, size );
    }

    void GotoXy( SHORT x, SHORT y ) const
    {
        COORD coord;

        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition( m_hConsole, coord );
    }

    void SetColor( WORD fore, WORD back = 0 )
    {
        WORD attribute;
        attribute = ( back & 0x0f ) << 4 | ( fore & 0x0f );
        SetConsoleTextAttribute( m_hConsole, attribute );

        /** predefined flags
            FOREGROUND_BLUE
            FOREGROUND_GREEN
            FOREGROUND_RED
            FOREGROUND_INTENSITY
            BACKGROUND_BLUE
            BACKGROUND_GREEN
            BACKGROUND_RED
            BACKGROUND_INTENSITY
            */
    }
};


/** @example

    #include <stdio.h>
    #include <stdio.h>
    #include "Console.h"

    KConsole g_console;

    void main()
    {
        g_console.ShowCursor(false);    

        g_console.GotoXy(10,10);

        g_console.SetColor(2);
        printf("hello world\n");

        g_console.SetColor(15);
        printf("another world\n");

        getch();
    }//main()

*/
