using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DecalPlacer : MonoBehaviour
{
    [SerializeField]
    private GameObject _decalPrefab;

    // Update is called once per frame
    void Update()
    {
        if( Input.GetMouseButtonDown( 0 ) )
        {
            RaycastHit hitInfo;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if( Physics.Raycast( ray, out hitInfo ) )
            {
                SpawnDecal( hitInfo );
            }
        }
    }
    private void SpawnDecal( RaycastHit hitInfo )
    {
        var decal = Instantiate(_decalPrefab);
        decal.transform.position = hitInfo.point;
        decal.transform.forward = hitInfo.normal * -1.0f;
    }
}
