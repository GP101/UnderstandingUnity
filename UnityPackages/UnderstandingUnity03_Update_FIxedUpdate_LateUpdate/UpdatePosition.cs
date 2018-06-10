using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UpdatePosition : MonoBehaviour
{
    public GameObject target;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void LateUpdate () { // camera jittering problem.
        if (target != null)
        {
            Quaternion quat = Camera.main.transform.rotation;
            Vector3 pos = Camera.main.transform.position;
            pos.x = target.transform.position.x;
            Camera.main.transform.SetPositionAndRotation(pos, quat);
        }
	}
}
