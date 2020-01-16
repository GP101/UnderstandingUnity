using System;
using System.Text;
using System.Reflection;

class Program1
{
    public delegate void HelloDelegate();

    class Sample
    {
        public HelloDelegate hello;
        //public event HelloDelegate hello;

        public void DoAction()
        {
            hello();
        }
    }

    static void Main(string[] args)
    {
        Sample s = new Sample();
        s.hello += SayHello;
        s.hello += SayHello2;
        s.hello += SayHello3;
        s.hello = SayHello;
        s.DoAction();

    }
    static void SayHello()
    {
        Console.WriteLine("Hello World");
    }
    static void SayHello2()
    {
        Console.WriteLine("Hello World2");
    }
    static void SayHello3()
    {
        Console.WriteLine("Hello World3");
    }
}
