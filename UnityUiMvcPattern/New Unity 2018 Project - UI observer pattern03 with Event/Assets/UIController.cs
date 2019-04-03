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


    private void OnEnable()
    {
        RuntimeGameDataManager.OnDataUpdated += OnDataUpdate;
    }
    private void OnDisable()
    {
        RuntimeGameDataManager.OnDataUpdated -= OnDataUpdate;
    }

    private void OnDataUpdate()
    {
        _UpdateData();
    }

    private void Update()
    {
        //_UpdateData();
    }
    // Update is called once per frame
    void _UpdateData()
    {
        // observer pattern
        int uiDataStamp = RuntimeGameDataManager.instance.GetDataStamp();
        if( uiDataStamp != _uiDataStamp )
        {
            _uiDataStamp = uiDataStamp; // update ui data stamp

            int count = RuntimeGameDataManager.instance.GetCount();
            countText.text = count.ToString();
            if( RuntimeGameDataManager.instance.IsWin() )
            {
                winText.text = "You Win!";
            }
        }
    }
}
