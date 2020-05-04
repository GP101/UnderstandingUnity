using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

// step 1
public class UIController : MonoBehaviour
{
    public Text countText;
    public Text winText;
    private int _uiDataStamp = 0;

    // Update is called once per frame
    void Update()
    {
        // observer pattern
        int uiDataStamp = RuntimeGameDataManager.GetDataStamp();
        if( uiDataStamp != _uiDataStamp )
        {
            _uiDataStamp = uiDataStamp; // update ui data stamp

            int count = RuntimeGameDataManager.GetCount();
            countText.text = count.ToString();
            if( RuntimeGameDataManager.IsWin() )
            {
                winText.text = "You Win!";
            }
        }
    }
}
