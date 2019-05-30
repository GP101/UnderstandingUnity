using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SphereTest : MonoBehaviour
{
    public Transform _plane;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnDrawGizmos()
    {
        Vector3 hitPointOnPlane = Math3d.ProjectPointOnPlane(_plane.up, _plane.position
            , transform.position);
        Debug.DrawLine(transform.position, hitPointOnPlane, Color.red );
    }
}
