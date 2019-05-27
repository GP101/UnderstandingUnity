using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugPlot : MonoBehaviour
{
    public AnimationCurve plot = new AnimationCurve();

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float value = Mathf.Sin(Time.time);
        plot.AddKey(Time.realtimeSinceStartup, value);
    }
}
