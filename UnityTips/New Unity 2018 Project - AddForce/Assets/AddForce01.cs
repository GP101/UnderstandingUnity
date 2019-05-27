using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AddForce01 : MonoBehaviour
{
    private Rigidbody _rigidbody;
    public float _forceAmount = 100;
    private float _impulseTime = 0.0f;
    // Start is called before the first frame update
    void Start()
    {
        _rigidbody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (Input.GetKeyDown(KeyCode.A))
        {
            // this code actually add impulse to the rigid body.
            // jintaeks on 2019/05/09 19:18
            _rigidbody.AddForce(transform.forward * _forceAmount);
        }
    }

    private void OnGUI()
    {
        if (GUI.Button(new Rect(10, 70, 50, 30), "Click"))
            _rigidbody.AddForce(transform.forward * _forceAmount);
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
