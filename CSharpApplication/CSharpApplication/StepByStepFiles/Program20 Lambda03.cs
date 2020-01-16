using System;
using System.Text;
using System.Reflection;

class Program1
{
    delegate void Calculate(int num);
    delegate int Calculate2(int num);

    static void Multiply2(int num)
    {
        Console.WriteLine("Multiply2: {0} * 2 = {1}", num, num * 2);
    }

    static void Main(string[] args)
    {
        Calculate cal = Multiply2;
        cal(3);
        Calculate cal2 = delegate (int num)
        {
            Console.WriteLine("{0} * 2 = {1}", num, num * 2);
        };
        cal2(3);
        Calculate cal3 = (int num) =>
        {
            Console.WriteLine("{0} * 2 = {1}", num, num * 2);
        };
        cal3(3);
        Calculate cal4 = (num) =>
        {
            Console.WriteLine("{0} * 2 = {1}", num, num * 2);
        };
        cal4(3);
        Calculate cal5 = num =>
        {
            Console.WriteLine("{0} * 2 = {1}", num, num * 2);
        };
        cal5(3);
        Calculate2 cal6 = num => { return num * 2; };
        Console.WriteLine("{0}", cal6(4));
        Calculate2 cal7 = num => num * 2; ;
        Console.WriteLine("{0}", cal7(5));
    }
    /** output:
        Multiply2: 3 * 2 = 6
        3 * 2 = 6
        3 * 2 = 6
        3 * 2 = 6
        3 * 2 = 6
        8
        10
    */
}
