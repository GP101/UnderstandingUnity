using System;
using System.Collections.Generic;
using System.Text;

namespace ConsoleApp1
{
    class Program
    {
        public class KPoint
        {
            private int _x, _y;

            public KPoint( int x, int y )
            {
                _x = x;
                _y = y;
            }

            //public class EqualityComparer : IEqualityComparer<KPoint>
            //{
            //    public bool Equals( KPoint lhs, KPoint rhs )
            //    {
            //        return lhs._x == rhs._x && lhs._y == rhs._y;
            //    }

            //    public int GetHashCode( KPoint p )
            //    {
            //        return p._x ^ p._y;
            //    }
            //}
            public int GetX()
            {
                return _x;
            }
        }

        static void Main( string[] args )
        {
            Dictionary<KPoint,string>     parkingInfo = new Dictionary<KPoint, string>();
            parkingInfo.Add( new KPoint( 1, 1 ), "first point" );
            parkingInfo.Add( new KPoint( 3, 2 ), "second point" );
            foreach( KeyValuePair<KPoint, string> pair in parkingInfo )
            {
                Console.WriteLine( "{0} {1}", pair.Key.GetX(), pair.Value );
            }
        }
    }
}
