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
        public sealed override void Update() { Console.WriteLine( "KDerived::Update" ); }
    }

    public class KFinal : KDerived
    {
        public override void Start() { Console.WriteLine( "KFinal::Start" ); }
        //public override void Update() { Console.WriteLine( "KFinal::Update" ); } // (1) compile time error
    }

    class Tester
    {
        static void Main( string[] args )
        {
            KBase c = new KFinal();
            c.Start();
            c.Update();
            Console.ReadKey();
            /*
                KFinal::Start
                KDerived::Update
            */
        }
    }
}
