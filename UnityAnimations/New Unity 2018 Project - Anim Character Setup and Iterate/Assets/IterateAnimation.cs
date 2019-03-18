using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IterateAnimation : MonoBehaviour {

    Animator anim;
    int currentAnimClip = 0;
	// Use this for initialization
	void Start () {
        anim = GetComponent<Animator>();
	}//Start()
	
	// Update is called once per frame
	void Update () {
        if( Input.GetKeyDown( KeyCode.Space ) ) {
            NextAnimation();
        }//if
	}//Update()

    void OnGUI()
    {
        if( GUI.Button( new Rect( 10, 10, 150, 100 ), "Next" ) ) {
            NextAnimation();
        }//if
    }//OnGUI()

    public void NextAnimation()
    {
        int numAnimClips = anim.runtimeAnimatorController.animationClips.GetLength( 0 );
        currentAnimClip = ( currentAnimClip + 1 ) % numAnimClips;

        int current = 0;
        foreach( AnimationClip ac in anim.runtimeAnimatorController.animationClips ) {
            // look at all the animation clips here!
            if( current == currentAnimClip ) {
                anim.Play( ac.name );
            }//if
            current = current + 1;
        }//foreach
    }//NextAnimation()

}
