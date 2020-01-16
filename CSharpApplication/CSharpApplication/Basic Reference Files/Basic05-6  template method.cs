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
        public virtual void LateUpdate() { Console.WriteLine( "KBase::LateUpdate" ); }
        public void DoUpdate()
        {
            Update();
            LateUpdate();
        }
    }

    public class KDerived : KBase
    {
        public override void Start() { Console.WriteLine( "KDerived::Start" ); }
        public override void Update() { Console.WriteLine( "KDerived::Update" ); }
        public override void LateUpdate() { Console.WriteLine( "KDerived::LateUpdate" ); }
    }


    class Tester
    {
        static void Main( string[] args )
        {
            KBase b = new KDerived();
            b.DoUpdate();
            Console.ReadKey();
            /*
                KDerived::Update
                KDerived::LateUpdate
            */
        }
    }
}
