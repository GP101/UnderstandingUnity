using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PurchaseScript : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    
    // jintaeks 2019/04/09 14:53
    public void BuyComplete(UnityEngine.Purchasing.Product product)
    {
        Debug.Log("Purchase Complete");
    }

    // 20190409_jintaeks
    public void BuyFailed(UnityEngine.Purchasing.Product product
        , UnityEngine.Purchasing.PurchaseFailureReason failure)
    {
        Debug.Log("Purchase Failed");
    }
}
