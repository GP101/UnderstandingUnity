using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

// step 1
public class UIController : MonoBehaviour
{
    public Text countText;
    public Text winText;

    // Update is called once per frame
    void Update()
    {
        int count = RuntimeGameDataManager.GetCount();
        countText.text = count.ToString();
        if( RuntimeGameDataManager.IsWin() )
        {
            winText.text = "You Win!";
        }
    }
}
