using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireABullet : MonoBehaviour {

    public GameObject   bullet;
    public float power = 20.0f;
    public float rotSpeed = 100.0f;
	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
        Vector3 axis = new Vector3(1,0,0);
        Quaternion quatDelta = Quaternion.AngleAxis( Time.deltaTime * rotSpeed, axis );
        transform.rotation *= quatDelta;
        if( Input.GetButtonDown( "Fire1" ) ) {
            GameObject go = Instantiate( bullet ) as GameObject;
            // 'forward' vector is rendered with blue arrow.
            go.transform.position = transform.position + transform.forward * 2.0f;
        }//if
	}
}
