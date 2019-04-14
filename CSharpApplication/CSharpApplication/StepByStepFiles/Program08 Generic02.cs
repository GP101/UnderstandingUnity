using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CsharpConsole
{
    class Program
    {
        static void Main( string[] args )
        {
            ResultInt intResult = new ResultInt { Success = true, Data = 9 };
            ResultString stringResult = new ResultString { Success = false, Data = "error" };
            Console.WriteLine( "Success = {0}, Data = {1}", intResult.Success, intResult.Data );
            Console.WriteLine( "Success = {0}, Data = {1}", stringResult.Success, stringResult.Data );
        }
    }
    public class ResultInt
    {
        public bool Success { get; set; }
        public int Data { get; set; }
    }
    public class ResultString
    {
        public bool Success { get; set; }
        public string Data { get; set; }
    }
}
