using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SphereCaster : MonoBehaviour
{
    public GameObject _currentHitObject;
    private float _currentHitDistance;

    public float _sphereRadius = 0.5f;
    public float _maxDiatance = 2.0f;
    public LayerMask _layerMask;
    private Vector3 _origin;
    private Vector3 _direction;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        _origin = transform.position;
        _direction = transform.forward;
        RaycastHit hit;
        if( Physics.SphereCast( _origin, _sphereRadius, _direction, out hit
            , _maxDiatance, _layerMask, QueryTriggerInteraction.UseGlobal ) )
        {
            _currentHitObject = hit.transform.gameObject;
            _currentHitDistance = hit.distance;
        }
        else
        {
            _currentHitDistance = _maxDiatance;
            _currentHitObject = null;
        }
    }

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Debug.DrawLine( _origin, _origin + _direction * _currentHitDistance );
        Gizmos.DrawWireSphere( _origin + _direction * _currentHitDistance, _sphereRadius );
    }
}
