using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddForceTest : MonoBehaviour
{
    public Rigidbody[] _rigidbody;
    public float _forceAmount = 100;
    private float _impulseTime = 0.0f;

    // Start is called before the first frame update
    void Start()
    {
        _rigidbody = new Rigidbody[4];
        int childindex = 0;
        foreach (Transform child in transform)
        {
            _rigidbody[childindex] = child.gameObject.GetComponent<Rigidbody>();
            childindex += 1;
        }
    }

    void _ApplyForce()
    {
        _rigidbody[0].AddForce(transform.forward * _forceAmount, ForceMode.Force);
        _rigidbody[1].AddForce(transform.forward * _forceAmount * Time.fixedDeltaTime, ForceMode.Impulse);
        Vector3 v = transform.forward * _forceAmount * Time.fixedDeltaTime / _rigidbody[1].mass;
        _rigidbody[2].AddForce(v, ForceMode.VelocityChange);
        _rigidbody[3].AddForce(v / Time.fixedDeltaTime, ForceMode.Acceleration);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            _ApplyForce();
        }
    }

    private void OnGUI()
    {
        if (GUI.Button(new Rect(10, 70, 50, 30), "Click"))
        {
            _ApplyForce();
        }
    }

    //void FixedUpdate()
    //{
    //    if (Input.GetKeyDown(KeyCode.A))
    //    {
    //        _impulseTime = 0.8f;
    //    }
    //    if (_impulseTime > 0.0f)
    //    {
    //        _impulseTime -= Time.fixedDeltaTime;
    //        _rigidbody.AddForce(transform.forward * _forceAmount);
    //    }
    //}

    //void Update()
    //{
    //    if (Input.GetKeyDown(KeyCode.A))
    //    {
    //        // first mistake: it does not guarantee 
    //        // that same impulse is applied for rigid body.
    //        _rigidbody.AddForce(transform.forward * _forceAmount * Time.deltaTime);
    //    }

    //    // other mistake: it does not guarantee 
    //    // that same impulse is applied for rigid body.
    //    _rigidbody.AddForce(transform.forward * _forceAmount * Time.fixedDeltaTime);

    //    // solution: Physics related function like AddForce() must be called
    //    // inside of FixedUpdate() with Time.fixedDeltaTime;
    //}
}
