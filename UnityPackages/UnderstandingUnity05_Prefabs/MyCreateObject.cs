using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCreateObject : MonoBehaviour {

    public GameObject m_go;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.A))
        {
            if(m_go != null )
                Instantiate(m_go, new Vector3(0, 2, 0), Quaternion.identity);
        }
	}
}
