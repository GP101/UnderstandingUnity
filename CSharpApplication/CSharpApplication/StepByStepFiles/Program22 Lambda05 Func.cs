using System;
using System.Text;
using System.Reflection;

class Program1
{
    static void Main(string[] args)
    {
        Action<int> cal6 = num => { Console.WriteLine("Action {0} * 2 = {1}", num, num * 2); };
        cal6(3);
        Func<int, float> cal7 = num => { return num * 2; };
        Console.WriteLine("Func {0} * 2 = {1}", 3, cal7(3));
    }
}
