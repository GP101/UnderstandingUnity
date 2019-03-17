using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/** @date   jintaeks on 2017-01-18_00-26
    @desc   KakeMake character controller
*/
[RequireComponent( typeof( Animator ) )]
[RequireComponent( typeof( Rigidbody ) )]
[RequireComponent( typeof( CapsuleCollider ) )]
public class KakeMakeControl2 : MonoBehaviour
{
    public float        animSpeed = 2.0f;
    public VirtualJoystickHandler
                        joystickHandler = null;

    readonly float      speedThreshold = 0.1f;
    bool                ikActive = false;
    Animator            anim;
    Rigidbody           rigidbody2;
    float               collStayTime = 0.0f;
    HashSet<string>     collList;
    float               curSpeed = 0.0f;

    void Awake()
    {
        collList = new HashSet<string>();
    }//Awake()

    // Use this for initialization
    void Start ()
    {
        anim = GetComponent<Animator>();
        anim.speed = animSpeed;
        rigidbody2 = GetComponent<Rigidbody>();
    }//Start()

    void FixedUpdate()
    {
        float h = Input.GetAxis( "Horizontal" );
        float v = Input.GetAxis("Vertical");
        Vector3 direction = joystickHandler.inputDirection; //InputDirection can be used as per the need of your project
        h = direction.x;
        v = direction.y;
        if (direction.magnitude != 0)
        {
        }
        string s = string.Format( "h={0}, v={1}", h, v );
        Debug.Log( s );

        Vector3 dir = Vector3.zero;
        dir.Set( h, 0f, v );
        float speed = dir.magnitude;
        //Debug.LogFormat( "Speed={0}", speed );
        if( ikActive == true ) {
            speed /= 2;
        }//if
        curSpeed = Mathf.Lerp( curSpeed, speed, Time.deltaTime * 10 );

        anim.SetFloat( "Speed", curSpeed );
        if( speed > 0.05 ) {
            Turning( h, v, dir, speed );
        }//if
        if( curSpeed > speedThreshold ) {
            anim.Play( "Move" );
        }//if
    }//FixedUpdate()

    void Turning( float h, float v, Vector3 dir, float speed )
    {
        Quaternion newRotation = Quaternion.LookRotation( dir );
        rigidbody2.MoveRotation( newRotation );
    }//Turning()

    void OnCollisionEnter(Collision collision)
    {
        //collList.Add( collision.transform.name );
        //Debug.LogFormat( "Coll {0}", collision.collider.name );

        foreach( ContactPoint contact in collision.contacts ) {
            float dotValue = Vector3.Dot( -contact.normal, transform.forward );
            //Debug.LogFormat( "Dot {0}", dotValue );
            // check side-to-side collision
            // jintaeks on 2017-01-18_00-53
            if( dotValue >= 0.9 ) {
                collList.Add( collision.transform.name );
            }//if
        }//foreach
        ikActive = ( collList.Count >= 1 );
    }//OnCollisionEnter()

    void OnCollisionExit(Collision collision)
    {
        collList.Remove( collision.transform.name );
        ikActive = ( collList.Count >= 1 );
    }//OnCollisionExit()

    void OnCollisionStay(Collision collision)
    {
    }//OnCollisionStay()

    void OnAnimatorIK( int layerIndex )
    {
        //if the IK is active, set the position and rotation directly to the goal. 
        if(ikActive) {
            collStayTime += Time.deltaTime * 2;
            if (collStayTime > 1.0f)
                collStayTime = 1.0f;
            Vector3 position = transform.position + transform.forward * 10.0f;

            // Set the look target position, if one has been assigned
            {
                anim.SetLookAtWeight(collStayTime);
                anim.SetLookAtPosition(position);
            }    

            // Set the right hand target position and rotation, if one has been assigned
            {
                anim.SetIKPositionWeight(AvatarIKGoal.RightHand,collStayTime);
                anim.SetIKRotationWeight(AvatarIKGoal.RightHand,collStayTime);  
                anim.SetIKPosition(AvatarIKGoal.RightHand, position);

                anim.SetIKPositionWeight(AvatarIKGoal.LeftHand,collStayTime);
                anim.SetIKRotationWeight(AvatarIKGoal.LeftHand,collStayTime);  
                anim.SetIKPosition(AvatarIKGoal.LeftHand, position);

                Quaternion quat;
                quat = Quaternion.LookRotation( Vector3.up );
                /* Rotating by the product lhs * rhs is the same as applying the two rotations
                   in sequence: lhs first and then rhs, relative to the reference frame resulting
                   from lhs rotation. Note that this means rotations are not commutative,
                   so lhs * rhs does not give the same rotation as rhs * lhs. */
                quat = transform.rotation * quat;
                anim.SetIKRotation(AvatarIKGoal.RightHand, quat);
                anim.SetIKRotation(AvatarIKGoal.LeftHand, quat);
            }
        }
        //if the IK is not active, set the position and rotation of the hand and head back to the original position
        else {
            collStayTime -= Time.deltaTime * 2;
            if( collStayTime < 0.0f ) {
                collStayTime = 0.0f;
            }
            
            anim.SetIKPositionWeight(AvatarIKGoal.RightHand,collStayTime);
            anim.SetIKRotationWeight(AvatarIKGoal.RightHand,collStayTime); 
            anim.SetIKPositionWeight(AvatarIKGoal.LeftHand,collStayTime);
            anim.SetIKRotationWeight(AvatarIKGoal.LeftHand,collStayTime); 
            anim.SetLookAtWeight(collStayTime);
        }//if.. else..
    }//OnAnimatorIK()

}
