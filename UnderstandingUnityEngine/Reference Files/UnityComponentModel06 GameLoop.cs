using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace UnderstandingUnityEngine
{
    public abstract class Component
    {
        public GameObject gameObject { get; set; }
        public void SendMessage( string methodName ) { }
    }

    public class Behavior : Component
    {
    }

    public class MonoBehavior : Behavior
    {
        public void Invoke( string methodName ) { }
    }

    public class BoxCollider : MonoBehavior
    {
        public void GetCollider() { }
    }

    public class MeshRenderer : MonoBehavior
    {
        public void GetMaterial()
        {
            Console.WriteLine( "GetMaterial" );
        }
    }

    public class GameObject
    {
        private List<Component> m_components;

        public GameObject()
        {
            m_components = new List<Component>();

            AddComponent<BoxCollider>();
            AddComponent<MeshRenderer>();
            AddComponent<Test>();
        }

        public T AddComponent<T>() where T : Component, new()
        {
            T component = new T();
            component.gameObject = this;
            m_components.Add( component );
            return component;
        }

        public T GetComponent<T>() where T : Component
        {
            foreach( var comp in m_components )
            {
                if( comp.GetType() == typeof( T ) )
                {
                    return ( T )comp;
                }
            }
            return default( T );
        }

        public void BroadcastMessage( string methodName )
        {
            foreach( var comp in m_components )
            {
                Type t = comp.GetType();
                MethodInfo minfo = t.GetMethod( methodName );
                if( minfo != null )
                {
                    minfo.Invoke( comp, null );
                }
            }
        }
    }

    public class Test : MonoBehavior
    {
        public void Start()
        {
        }

        public void Update()
        {
            MeshRenderer renderer = gameObject.GetComponent<MeshRenderer>();
            renderer.GetMaterial();
        }
    }

    class Program
    {
        static void Main( string[] args )
        {
            GameObject e = new GameObject();
            e.BroadcastMessage( "Start" );

            while( true )
            {
                if( Console.KeyAvailable )
                {
                    if( Console.ReadKey().Key == ConsoleKey.Escape )
                        break;
                }
                e.BroadcastMessage( "Update" );
                System.Threading.Thread.Sleep( 1000 );
            }
        }
    }
}
