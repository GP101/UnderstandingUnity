using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class LookAt : MonoBehaviour {

    public Transform target;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        Vector3 dir = target.position - transform.position;
        transform.rotation = Quaternion.LookRotation( dir );
	}

    private void OnDrawGizmos()
    {
        Debug.DrawLine(transform.position, target.position, Color.red);
    }
}
