using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rotate : MonoBehaviour {

    public float m_rotSpeed = 2f;
    // Use this for initialization
    void Start () {
	}
	
	// Update is called once per frame
	void Update () {
        gameObject.transform.Rotate(Vector3.up, Time.deltaTime * m_rotSpeed );
        //Debug.Log(Vector3.up);
	}
}
