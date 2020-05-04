using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// step 1
public class RuntimeGameDataManager : MonoBehaviour
{
    //static public RuntimeGameDataManager instance;
    private static int _count = 0;
    private static int _levelCount = 0;
    private static int _level = 1;
    private static bool _isWin = false;

    void Awake()
    {
        //if( instance == null )
        //    instance = this;
    }

    public static void AddCount( int c )
    {
        _count += 1;
        _levelCount += 1;
        if( _levelCount >= 3 )
        {
            _levelCount = 0;
            _level += 1;
        }
        if( _count >= 12 )
            _isWin = true;
    }

    public static int GetCount()
    {
        return _count;
    }

    public static bool IsWin()
    {
        return _isWin;
    }
}
