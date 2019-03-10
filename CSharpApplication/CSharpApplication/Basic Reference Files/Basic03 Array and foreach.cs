using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static void Main( string[] args )
    {
        int [] marks = new int[5]  { 99,  98, 92, 97, 95};
        int [] marks2 = { 99,  98, 92, 97, 95};

        for( int i = 0; i < marks.Length; ++i )
        {
            Console.Write( "marks[{0}]i={1}\r\n", i, marks[i] );
        }
        foreach( int j in marks2 )
            Console.Write( "i={0}\r\n", j );
    }
}
