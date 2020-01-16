using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    class Rectangle
    {
        public double width;
        public double height;

        public Rectangle()
        {
        }
        ~Rectangle()
        {
        }
        public double GetArea()
        {
            return width * height;
        }
        public void Display()
        {
            Console.WriteLine( "Width: {0}", width );
            Console.WriteLine( "Height: {0}", height );
            Console.WriteLine( "Area: {0}", GetArea() );
        }
    }

    struct Rectangle2
    {
        public double width;
        public double height;

        public double GetArea()
        {
            return width * height;
        }
        public void Display()
        {
            Console.WriteLine( "Width: {0}", width );
            Console.WriteLine( "Height: {0}", height );
            Console.WriteLine( "Area: {0}", GetArea() );
        }
    }

    static void Main( string[] args )
    {
        Rectangle r;
        Rectangle2 r2;
        r = new Rectangle() { width = 2, height = 3 };
        r.Display();
        r2.width = 4;
        r2.height = 5;
        r2.Display();
    }
}
