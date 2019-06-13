using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PortalBeginController : MonoBehaviour
{
    public Transform _destination;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            Transform player = other.gameObject.transform;
            CapsuleController controller = player.GetComponent<CapsuleController>();
            controller.SetDestination(_destination.position);
        }
    }
}
