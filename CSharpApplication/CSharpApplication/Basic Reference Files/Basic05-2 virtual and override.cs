using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    public class KBase
    {
        // interface members
        public virtual void Start() { Console.WriteLine( "KBase::Start" ); }
        public virtual void Update() { Console.WriteLine( "KBase::Update" ); }
    }

    public class KDerived : KBase
    {
        public virtual void Start() { Console.WriteLine( "KDerived::Start" ); }
        public override void Update() { base.Update(); Console.WriteLine( "KDerived::Update" ); }
    }
    class Tester
    {
        static void Main( string[] args )
        {
            KDerived d = new KDerived();
            d.Start();
            d.Update();
            KBase b = new KDerived();
            b.Start();
            b.Update();
            Console.ReadKey();
            /*
                KDerived::Start
                KBase::Update
                KDerived::Update
                KBase::Start
                KBase::Update
                KDerived::Update
            */
        }
    }
}
