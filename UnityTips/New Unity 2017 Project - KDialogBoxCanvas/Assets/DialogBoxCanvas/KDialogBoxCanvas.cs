using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEngine.UI;

/// <summary>
/// @class  KDialogBoxCanvas
/// @desc   Create Dialog Box with caption and message
/// @date   20190327_jintaeks
/// </summary>
public class KDialogBoxCanvas : MonoBehaviour
{
    private class KDialogBoxInfo
    {
        public string _caption;
        public string _message;
        public string _button1Text;
        public string _button2Text;
        public UnityAction<int> _action;
    }

    private static GameObject canvasContainer;
    public Text captionText; // caption text
    public Text messageText; // message text
    public Text button1Text; // left(Yes) button
    public Text button2Text; // right(No) button
    private UnityAction<int> buttonAction;
    private Stack<KDialogBoxInfo> _dialogBoxInfoStack;
    private bool _isShowgDialogBox = false;
    
    /// <summary>
    /// create dialog box - 20190327_jintaeks
    /// </summary>
    /// <param name="dialogBoxCanvas"> KDialogBoxCanvas game object in the hierarchy </param>
    /// <param name="caption"></param>
    /// <param name="message"></param>
    /// <param name="action"> Button down event handler </param>
    /// <returns></returns>
    /** <example>
        private void OnGUI()
        {
            if (GUI.Button(new Rect(0, 0, 200, 40), "Test"))
            {
                KDialogBoxCanvas.DialogBox( "My Caption"
                    , "My Message"
                    , "Yes"
                    , "No"
                    , (int buttonId) =>
                    {
                        // 1 : left button
                        // 2 : right button
                        Debug.Log("My Button " + buttonId as string );
                    } );
            }
        }
        </example> */
    public static KDialogBoxCanvas DialogBox( string caption, string message
        , string strLeftButton, string strRightButton, UnityAction<int> action)
    {
        KDialogBoxInfo info = new KDialogBoxInfo() { _caption = caption, _message = message
            , _button1Text = strLeftButton, _button2Text = strRightButton, _action = action };
        KDialogBoxCanvas handler = canvasContainer.GetComponent<KDialogBoxCanvas>();
        handler.PushDialogBoxInfo(info);

        //KDialogBoxCanvas handler = canvasContainer.GetComponent<KDialogBoxCanvas>();
        //handler.SetTexts(caption, message);
        //handler.SetButtonTexts(strLeftButton, strRightButton);
        //handler.SetButtonAction(action);
        //canvasContainer.SetActive(true);
        return handler;
    }

    public static void OnEndUnzoomPanelCallback()
    {
        Debug.Log("EndUnzoom");
    }

    private void Awake()
    {
        canvasContainer = gameObject;
        canvasContainer.SetActive(false);
        _dialogBoxInfoStack = new Stack<KDialogBoxInfo>();
    }

    public void Update()
    {
        if (_dialogBoxInfoStack.Count >= 1)
        {
            if(_isShowgDialogBox == false )
            {
                _isShowgDialogBox = true;

                KDialogBoxInfo info = _dialogBoxInfoStack.Peek(); // get dialog box info from top of the stack
                //KDialogBoxCanvas handler = canvasContainer.GetComponent<KDialogBoxCanvas>();
                SetTexts(info._caption, info._message);
                SetButtonTexts(info._button1Text,info._button2Text);
                SetButtonAction(info._action);
                canvasContainer.SetActive(true);
                _dialogBoxInfoStack.Pop(); // remove used dialog box info
            }
        }
    }

    private void PushDialogBoxInfo( KDialogBoxCanvas.KDialogBoxInfo info )
    {
        _dialogBoxInfoStack.Push(info);
        gameObject.SetActive(true);
    }

    public void SetTexts(string strCaption, string strMessage)
    {
        captionText.text = strCaption;
        messageText.text = strMessage;
    }

    public void SetButtonTexts(string strLeftButton, string strRightButton)
    {
        button1Text.text = strLeftButton;
        button2Text.text = strRightButton;
    }

    public void SetButtonAction(UnityAction<int> action)
    {
        buttonAction = action;
    }

    public void OnButton1Down()
    {
        //Debug.Log("Hello Button1");
        if (buttonAction != null)
            buttonAction.Invoke(1);
        //gameObject.SetActive(false);
        _isShowgDialogBox = false;
        if (_dialogBoxInfoStack.Count == 0)
            gameObject.SetActive(false);
    }

    public void OnButton2Down()
    {
        //Debug.Log("Hello Button2");
        if (buttonAction != null)
            buttonAction.Invoke(2);
        //gameObject.SetActive(false);
        _isShowgDialogBox = false;
        if (_dialogBoxInfoStack.Count == 0)
            gameObject.SetActive(false);
    }
}
