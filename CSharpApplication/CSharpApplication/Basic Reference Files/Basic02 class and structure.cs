using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    class Rectangle
    {
        //member variables
        public double length;
        public double width;

        public Rectangle() // constructor
        {
        }
        ~Rectangle() // finalizer
        {
        }
        public double GetArea()
        {
            return length * width;
        }
        public void Display()
        {
            Console.WriteLine( "Length: {0}", length );
            Console.WriteLine( "Width: {0}", width );
            Console.WriteLine( "Area: {0}", GetArea() );
        }
    }

    struct Rectangle2
    {
        //member variables
        public double length;
        public double width;

        public double GetArea()
        {
            return length * width;
        }
        public void Display()
        {
            Console.WriteLine( "Length: {0}", length );
            Console.WriteLine( "Width: {0}", width );
            Console.WriteLine( "Area: {0}", GetArea() );
        }
    }

    static void Main( string[] args )
    {
        Rectangle r = new Rectangle();
        Rectangle2 r2;
        r.length = 2;
        r.width = 3;
        r.Display();
        r2.length = 4;
        r2.width = 5;
        r2.Display();
    }
}
