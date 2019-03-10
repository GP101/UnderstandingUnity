using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace CsharpConsole
{
    public class Person
    {
        public string Name { get; set; }
        public int Age { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            //string cSharpType = "Hello World";
            //String dotNetType = "Hello World";
            //Console.WriteLine("cSharpType = " + cSharpType);
            //Console.WriteLine("dotNetType = " + dotNetType);

            //string outputValue = "Have a nice";
            //outputValue += " Day";
            //Console.WriteLine("outputValue = " + outputValue);

            string garbageCollectionTest = "Hello World";
            for (int i = 1; i < 10000; ++i)
            {
                garbageCollectionTest += "Hello World";
            }
            Console.WriteLine(garbageCollectionTest);

            //StringBuilder sb = new StringBuilder("Hello World");
            //for (int i = 1; i < 10000; ++i)
            //{
            //    sb.Append("Hello World");
            //}
            //Console.WriteLine(sb);
        }
    }
}