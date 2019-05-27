using System.Collections;
using System.Collections.Generic;
using UnityEngine;

enum KeyCommandState { Idle, KeyInput };

public struct KKeyCommand
{
    readonly static double  interKeyTime = 0.2;
    public string           commandString;
    int                     keyCursor;
    KeyCommandState         keyState;
    double                  keyTimer;

    public KKeyCommand( string command_ )
    {
        commandString = command_;
        keyCursor = 0;
        keyState = KeyCommandState.Idle;
        keyTimer = 0.0;
    }//KKeyCommand

    public bool ProcessKey( char key )
    {
        if( keyCursor >= commandString.Length )
        {
            //keyCursor += 1;
            return false;
        }//if

        if( commandString[keyCursor] == key )
        {
            if( keyCursor == 0 )
            {
                // move to 'KeyInput' state from 'Idle'
                keyState = KeyCommandState.KeyInput;
            }//if
            keyCursor += 1; // move to next key
            keyTimer = 0.0; // reset valid key input timer
            if( keyCursor == commandString.Length )
            {
                // this key command is matched
                return true;
            }//if
        }//if

        return false;
    }//ProcessKey()

    // must be called for each 'Update()'
    public void Update( double elapsedTime_ )
    {
        keyTimer += elapsedTime_;
        if( keyTimer >= interKeyTime )
        {
            // move to 'Idle' state if timer expire
            keyState = KeyCommandState.Idle;
            keyCursor = 0;
            keyTimer = 0.0;
        }//if
    }//Update()

    // check this key command is triggered or not.
    public bool IsTrigger()
    {
        if( keyState == KeyCommandState.KeyInput && keyCursor == commandString.Length )
        {
            return true;
        }//if

        return false;
    }//IsTrigger()

    public int GetCursor()
    {
        return keyCursor;
    }//GetCursor()
};//struct KKeyCommand

public class BoxControl : MonoBehaviour {
    //readonly static double  inputThreshold = 0.05; // NOT used in this demo, maybe required if you use a joystick.
    KKeyCommand[]       keyCommands;

    // Unity maintains key input in a lerped way. So we need to current lerped value to detect new key input.
    // - jintaeks on 2017-03-21_15-57
    float               lerpHorizontalKey;
    float               lerpVerticalKey;

    // for showing key command success.
    // when key command is triggered, we will add force to upward.
    // - jintaeks on 2017-03-21_15-56
    Rigidbody           rigidBody;
    public float        forceFactor = 10.0F;

	// Use this for initialization
	void Start () {
        keyCommands = new KKeyCommand[]
        {
            new KKeyCommand("UR"),
            new KKeyCommand("LDR"),
            new KKeyCommand("UD"),
            new KKeyCommand("LRLRD"),
        };//keyCommands[]

        rigidBody = GetComponent<Rigidbody>();
    }//Start()

    void FixedUpdate()
    {
        for( int i = 0; i < keyCommands.Length; ++i ) {
            if( keyCommands[i].IsTrigger() == true ) {
                Debug.Log( keyCommands[i].commandString );
                Vector3 force = new Vector3( 0, 1, 0 );
                rigidBody.AddForce( transform.up * forceFactor );
            }//if
        }//for
    }//FixedUpdate()
	
	// Update is called once per frame
	void Update () {
        float newHorizontalKey = Input.GetAxis( "Horizontal" );
        float newVerticalKey = Input.GetAxis( "Vertical" );
        bool isNewLeftKey = false;
        bool isNewRightKey = false;
        bool isNewUpKey = false;
        bool isNewDownKey = false;

        if( lerpHorizontalKey <= 0 && newHorizontalKey > 0 )
            isNewRightKey = true;
        if( lerpHorizontalKey >= 0 && newHorizontalKey < 0 )
            isNewLeftKey = true;
        if( lerpVerticalKey <= 0 && newVerticalKey > 0 )
            isNewUpKey = true;
        if( lerpVerticalKey >= 0 && newVerticalKey < 0 )
            isNewDownKey = true;

        lerpHorizontalKey = newHorizontalKey;
        lerpVerticalKey = newVerticalKey;

        // 'foreach' generate more garbage than 'for'
        //foreach( KKeyCommand command in keyCommands )
        //
        for( int i = 0; i < keyCommands.Length; ++i)
        {
            if( isNewLeftKey )
                keyCommands[i].ProcessKey( 'L' );
            if( isNewRightKey )
                keyCommands[i].ProcessKey( 'R' );
            if( isNewUpKey )
                keyCommands[i].ProcessKey( 'U' );
            if( isNewDownKey )
                keyCommands[i].ProcessKey( 'D' );

            keyCommands[i].Update( Time.deltaTime );
        }//foreach
        //Debug.Log( "hello" );
	}//Update()

    void OnGUI()
    {
        int y = 0;
        foreach( KKeyCommand command in keyCommands )
        {
            string s = string.Format( "command {0}, cursor {1},", command.commandString, command.GetCursor() );
            GUI.Label( new Rect( 0, y, 200, 20 ), s );
            y += 20;
        }//foreach
    }//OnGUI()
}//class BoxControl
