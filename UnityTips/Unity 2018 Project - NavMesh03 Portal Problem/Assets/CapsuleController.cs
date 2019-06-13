using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class CapsuleController : MonoBehaviour
{
    Camera _cam;
    NavMeshAgent _agent;
    // Start is called before the first frame update
    void Start()
    {
        _cam = GameObject.FindGameObjectWithTag("MainCamera").gameObject.GetComponent<Camera>();
        _agent = GetComponent<NavMeshAgent>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Ray ray = _cam.ScreenPointToRay(Input.mousePosition);
            RaycastHit hit;
            bool isHit = Physics.Raycast(ray, out hit);
            if (isHit)
            {
                _agent.SetDestination(hit.point);
            }
        }
    }

    public void SetDestination(Vector3 targetPos)
    {
        _agent.ResetPath();
        _agent.enabled = false;
        transform.position = targetPos; 
    }
}
