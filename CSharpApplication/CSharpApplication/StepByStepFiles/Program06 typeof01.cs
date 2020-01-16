using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpApplication
{
    public interface IShape
    {
        double getArea();
    }

    public class Rectangle : IShape
    {
        public int Height;
        public int Width;

        public double getArea()
        {
            return Height * Width;
        }
    }

    public class Circle : IShape
    {
        public int Radius;

        public double getArea()
        {
            return Math.PI * Radius * Radius;
        }
    }

    class Program
    {
        static void Main( string[] args )
        {
            IShape shape1 = new Rectangle()
            {
                Height = 1,
                Width = 3
            };
            IShape shape2 = new Circle()
            {
                Radius = 2
            };

            Console.WriteLine( "{0}", typeof( IShape ) );
            if( shape2 is Circle )
            {
                Console.WriteLine( "shape2 is {0}", typeof( Circle ) );
            }
            //System.Type t0 = shape1.GetType();

            bool b = (shape2 is Rectangle);
            Console.WriteLine( "shape2 is Rectangle = {0}", b );
            //if( shape2.GetType() == typeof( Circle ) ) {
            //    Console.WriteLine( "shape2.GetType() == typeof( Circle )" );
            //}

            System.Type type = typeof(int);

            int radius = 1;
            Console.WriteLine( "Area = {0}", radius * radius * Math.PI );
            Console.WriteLine( "The type is {0}",
                              ( radius * radius * Math.PI ).GetType() );
            Console.WriteLine();
        }
    }
}
