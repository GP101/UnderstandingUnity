using System;
using System.Collections.Generic;
using System.Text;

namespace CSharpApplication
{
    public abstract class KBase
    {
        public abstract void Start();
        public abstract void Update();
    }

    public class KDerived : KBase
    {
        public int _idata = 0;
        public override void Start() { Console.WriteLine( "KDerived::Start" ); }
        public override void Update() { Console.WriteLine( "KDerived::Update" ); }
    }

    public class KFinal : KDerived
    {
        public override void Start() { Console.WriteLine( "KFinal::Start" ); }
        public override void Update() { Console.WriteLine( "KFinal::Update" ); }
    }

    class Program
    {
        static void Main( string[] args )
        {
            KDerived d = new KDerived();
            //System.Type t0 = typeof(KDerived);
            if( d.GetType() == typeof( KDerived ) )
            {
                Console.WriteLine( "d.GetType() == typeof( KDerived )" );
            }
            if( d is KBase )
            {
                Console.WriteLine( "d is KBase" );
            }
            if( d is KFinal )
            {
                Console.WriteLine( "d is KFinal" );
            }

            int radius = 1;
            Console.WriteLine( "Area = {0}", radius * radius * Math.PI );
            Console.WriteLine( "The type is {0}",
                              ( radius * radius * Math.PI ).GetType() );
            Console.WriteLine();

            System.Type type = d.GetType();
            {
                Console.WriteLine( "Type: " + type.Name + ", Base Type: " + type.BaseType );
                var props = type.GetProperties();
                foreach( var prop in props )
                {
                    Console.WriteLine( "\tProp: " + prop.Name );
                }
                var fields = type.GetFields();
                foreach( var field in fields )
                {
                    Console.WriteLine( "\tField: " + field.Name );
                }
                var methods = type.GetMethods();
                foreach( var method in methods )
                {
                    Console.WriteLine( "\tMethod: " + method.Name );
                }
            }
            /*  output:
                d.GetType() == typeof( KDerived )
                d is KBase
                Area = 3.14159265358979
                The type is System.Double

                Type: KDerived, Base Type: CSharpApplication.KBase
                        Field: _idata
                        Method: Start
                        Method: Update
                        Method: Equals
                        Method: GetHashCode
                        Method: GetType
                        Method: ToString
        */
        }
    }
}
