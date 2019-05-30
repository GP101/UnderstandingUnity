using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChangeVelocity : MonoBehaviour
{
    private Rigidbody _rigidbody;
    public float _speedFactor = 5.0f;

    // Start is called before the first frame update
    void Start()
    {
        _rigidbody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        float h = Input.GetAxis("Horizontal");
        _rigidbody.velocity = new Vector3(0, 0, h * _speedFactor);
    }
}
