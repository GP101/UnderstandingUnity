using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnderstandingUnityEngine
{
    public class Component
    {
    }

    public abstract class MonoBehavior : Component
    {
        public List<Component> m_components;
        public MonoBehavior()
        {
            m_components = new List<Component>();
        }
        public T GetComponent<T>() where T : Component
        {
            foreach (var comp in m_components)
            {
                if (comp.GetType() == typeof(T))
                {
                    return (T)comp;
                }
            }
            return default(T);
        }
    }

    public interface BoxCollider
    {
        void GetCollider();
    }
    public interface MeshRenderer
    {
        void GetMaterial();
    }

    public class GameObject : BoxCollider, MeshRenderer
    {
        public void GetCollider() { }
        public void GetMaterial() { }

        // Use this for initialization
        public void Start()
        {
        }

        public void Update()
        {
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            GameObject e = new GameObject();
            e.Start();
            e.Update();
        }
    }
}
