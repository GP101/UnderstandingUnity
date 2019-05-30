using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    public Transform _target;
    public Vector3 _offset = Vector3.zero;
    private Vector3 _lerpedTarget;
    private float _lerpFactor = 2f;
    private bool _isFixedUpdate = false;

    // Start is called before the first frame update
    void Start()
    {
        _lerpedTarget = _target.position;
    }

    // Update is called once per frame
    //void Update()
    void FixedUpdate()
    {
        _lerpedTarget = Vector3.Lerp(_lerpedTarget, _target.position, Time.deltaTime * _lerpFactor);
        gameObject.transform.LookAt(_lerpedTarget);
        gameObject.transform.position = _lerpedTarget + _offset;
        _isFixedUpdate = true;
    }
    private void LateUpdate()
    {
        if (_isFixedUpdate == false)
        {
            Vector3 newPos = Vector3.Lerp(_lerpedTarget, gameObject.transform.position, 0.0f);
            gameObject.transform.position = newPos + _offset;
        }
        _isFixedUpdate = false;
    }
}
