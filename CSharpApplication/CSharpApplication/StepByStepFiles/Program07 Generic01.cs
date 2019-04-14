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
            Console.WriteLine( "Success = {0}, Data = {1}", intResult.Success, intResult.Data );
        }
    }
    public class ResultInt
    {
        public bool Success { get; set; }
        public int Data { get; set; }
    }
}
