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
        List<IEnumerator<object>> m_coroutines;

        public GameObject()
        {
            m_components = new List<Component>();
            m_coroutines = new List<IEnumerator<object>>();

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
        public void StartCoroutine(IEnumerator<object> routine)
        {
            m_coroutines.Add(routine);
        }

        public void UpdateCoroutine()
        {
            int numRoutines = m_coroutines.Count;
            foreach (var routine in m_coroutines)
            {
                if (routine.MoveNext())
                {
                    object item = routine.Current;
                    Console.WriteLine("value = {0}", item);
                }
            }
        }
    }

    public class Test : MonoBehavior
    {
        public void Start()
        {
            gameObject.StartCoroutine(GetNextValue2());
        }

        public void Update()
        {
            MeshRenderer renderer = gameObject.GetComponent<MeshRenderer>();
            renderer.GetMaterial();
        }

        IEnumerator<object> GetNextValue2()
        {
            yield return "hello";
            yield return "wonderful";
            yield return "world";
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
                e.BroadcastMessage("Update");
                e.UpdateCoroutine();
                System.Threading.Thread.Sleep(1000);
            }
        }
    }
}
