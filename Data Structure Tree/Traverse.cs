using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Traverse : MonoBehaviour {
    private Queue<Transform>   m_queue;

    void TreeTraverse( Transform go )
    {
        foreach( Transform child in go )
        {
            //Debug.Log( child.name ); // pre-order
            TreeTraverse( child );
            Debug.Log( child.name ); // post-order
        }
    }

    void LevelOrder( Transform go )
    {
        m_queue.Clear();
        m_queue.Enqueue( go );
        while( m_queue.Count >= 1 )
        {
            Transform node = m_queue.Dequeue();
            Debug.Log( node.name );
            foreach( Transform child in node)
            {
                m_queue.Enqueue( child);
            }
        }
    }

    // Use this for initialization
    void Start () {
        //TreeTraverse( transform );
        m_queue = new Queue<Transform>();
        LevelOrder( transform );
    }

    // Update is called once per frame
    void Update () {
		
	}
}
