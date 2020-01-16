using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    public abstract class KBase
    {
        // interface members
        public abstract void Start();
        public abstract void Update();
    }

    public class KDerived : KBase
    {
        public override void Start() { Console.WriteLine( "KDerived::Start" ); }
        public override void Update() { Console.WriteLine( "KDerived::Update" ); }
    }
    class Tester
    {
        static void Main( string[] args )
        {
            KBase b = new KDerived();
            b.Start();
            b.Update();
            Console.ReadKey();
            /*
                KDerived::Start
                KDerived::Update
            */
        }
    }
}
