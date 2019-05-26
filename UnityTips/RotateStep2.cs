using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotate : MonoBehaviour
{
    public float m_rotSpeed = 45f;
    //public GameObject m_bullet;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        transform.Rotate( 0, 0, m_rotSpeed * Time.deltaTime, Space.Self );
        //if( Input.GetKeyDown(KeyCode.A) )
        //{
        //    GameObject go = Instantiate( m_bullet ) as GameObject;
        //    go.transform.position = transform.position + transform.up * 1.5f;
        //}
    }
}
