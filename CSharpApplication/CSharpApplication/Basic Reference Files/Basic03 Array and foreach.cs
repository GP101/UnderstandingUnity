using System;
using System.Collections.Generic;
using System.Text;

class Program
{
    static void Main( string[] args )
    {
        int[] marks = new int[5]  { 99,  98, 92, 97, 95};
        int[] marks2 = { 99,  98, 92, 97, 95};
        int[,] a = new int[2, 3] { { 1, 2, 3 }, { 4, 5, 6 } };

        for( int i = 0; i < marks.Length; ++i )
        {
            Console.Write( "marks[{0}]={1}\r\n", i, marks[i] );
        }
        foreach( int j in marks2 )
            Console.Write( "j={0}\r\n", j );
    }
}
/*  output:
    marks[0]=99
    marks[1]=98
    marks[2]=92
    marks[3]=97
    marks[4]=95
    j=99
    j=98
    j=92
    j=97
    j=95
*/
