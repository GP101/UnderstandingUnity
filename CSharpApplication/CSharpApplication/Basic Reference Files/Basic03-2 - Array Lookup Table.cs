using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    struct KeyValueInfo
    {
        public ConsoleKey key;
        public string dow;

        public KeyValueInfo( ConsoleKey key_, string dow_ )
        {
            key = key_;
            dow = dow_;
        }
    }

    static void Main( string[] args )
    {
        int[,] a = new int[2, 3] { { 1, 2, 3 }, { 4, 5, 6 } };
        string[] names = new string[] { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        //KeyValueInfo[] keyValue = new KeyValueInfo[2] { new KeyValueInfo(), new KeyValueInfo() };
        //keyValue[0].key = ConsoleKey.D1;
        //keyValue[0].dow = "Sun";
        //keyValue[1].key = ConsoleKey.D2;
        //keyValue[1].dow = "Mon";

        KeyValueInfo[] keyValue = new KeyValueInfo[2] 
        {
            new KeyValueInfo(){ key=ConsoleKey.D1, dow="Sun"},
            new KeyValueInfo(){ key=ConsoleKey.D2, dow="Mon"}
        };

        //KeyValueInfo[] keyValue = new KeyValueInfo[2]
        //{
        //    new KeyValueInfo(ConsoleKey.D1, "Sun"),
        //    new KeyValueInfo(ConsoleKey.D2, "Mon")
        //};

        ConsoleKeyInfo keyInfo = Console.ReadKey( true );
        //if( keyInfo.Key == ConsoleKey.D1 )
        //    Console.WriteLine( keyValue[0].dow );
        //else if( keyInfo.Key == ConsoleKey.D2 )
        //    Console.WriteLine( keyValue[1].dow );
        for( int i = 0; i < keyValue.Length; ++i )
        {
            if( keyInfo.Key == keyValue[i].key )
                Console.WriteLine( keyValue[i].dow );
        }
    }
}
