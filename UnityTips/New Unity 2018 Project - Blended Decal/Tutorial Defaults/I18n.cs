/*
 * Internationalization 
 * 
 * Author: Daniel Erdmann
 * 
 * 1. Add this File to you Project
 * 
 * 2. Add the language files to the folder Assets/Resources/I18n. (Filesnames: en.txt, es.txt, pt.txt, de.txt, and so on)
 *    Format: en.txt:           es.txt:
 *           =============== =================
 *           |hello=Hello  | |hello=Hola     |
 *           |world=World  | |world=Mundo    |
 *           |...          | |...            |
 *           =============== =================
 *           
 * 3. Use it! 
 *    Debug.Log(I18n.Fields["hello"] + " " + I18n.Fields["world"]); //"Hello World" or "Hola Mundo"
 * 
 * Use \n for new lines. Fallback language is "en"
 */

using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

class I18n
{
    /// <summary>
    /// Text Fields
    /// Usage: Fields[key]
    /// Example: I18n.Fields["world"]
    /// </summary>
    public static Dictionary<String, String> Fields { get; private set; }

    /// <summary>
    /// Init on first use
    /// </summary>
    static I18n()
    {
        LoadLanguage();
    }

    /// <summary>
    /// Load language files from resources
    /// </summary>
    private static void LoadLanguage()
    {
        if( Fields == null )
            Fields = new Dictionary<string, string>();

        Fields.Clear();
        string lang = Get2LetterISOCodeFromSystemLanguage().ToLower();
        //lang = "de";
        var textAsset = Resources.Load(@"I18n/" + lang); //no .txt needed
        string allTexts = "";
        if( textAsset == null )
            textAsset = Resources.Load( @"I18n/en" ) as TextAsset; //no .txt needed
        if( textAsset == null )
            Debug.LogError( "File not found for I18n: Assets/Resources/I18n/" + lang + ".txt" );
        allTexts = ( textAsset as TextAsset ).text;
        string[] lines = allTexts.Split(new string[] { "\r\n", "\n" },
            StringSplitOptions.None);
        string key, value;
        for( int i = 0; i < lines.Length; i++ )
        {
            if( lines[i].IndexOf( "=" ) >= 0 && !lines[i].StartsWith( "#" ) )
            {
                key = lines[i].Substring( 0, lines[i].IndexOf( "=" ) );
                value = lines[i].Substring( lines[i].IndexOf( "=" ) + 1,
                        lines[i].Length - lines[i].IndexOf( "=" ) - 1 ).Replace( "\\n", Environment.NewLine );
                Fields.Add( key.Trim(), value.Trim() );
            }
        }
    }

    /// <summary>
    /// get the current language
    /// </summary>
    /// <returns></returns>
    public static string GetLanguage()
    {
        return Get2LetterISOCodeFromSystemLanguage().ToLower();
    }

    /// <summary>
    /// Helps to convert Unity's Application.systemLanguage to a 
    /// 2 letter ISO country code. There is unfortunately not more
    /// countries available as Unity's enum does not enclose all
    /// countries.
    /// </summary>
    /// <returns>The 2-letter ISO code from system language.</returns>
    public static string Get2LetterISOCodeFromSystemLanguage()
    {
        SystemLanguage lang = Application.systemLanguage;
        string res = "EN";
        switch( lang )
        {
            case SystemLanguage.Afrikaans: res = "AF"; break;
            case SystemLanguage.Arabic: res = "AR"; break;
            case SystemLanguage.Basque: res = "EU"; break;
            case SystemLanguage.Belarusian: res = "BY"; break;
            case SystemLanguage.Bulgarian: res = "BG"; break;
            case SystemLanguage.Catalan: res = "CA"; break;
            case SystemLanguage.Chinese: res = "ZH"; break;
            case SystemLanguage.ChineseSimplified: res = "ZH"; break;
            case SystemLanguage.ChineseTraditional: res = "ZH"; break;
            case SystemLanguage.Czech: res = "CS"; break;
            case SystemLanguage.Danish: res = "DA"; break;
            case SystemLanguage.Dutch: res = "NL"; break;
            case SystemLanguage.English: res = "EN"; break;
            case SystemLanguage.Estonian: res = "ET"; break;
            case SystemLanguage.Faroese: res = "FO"; break;
            case SystemLanguage.Finnish: res = "FI"; break;
            case SystemLanguage.French: res = "FR"; break;
            case SystemLanguage.German: res = "DE"; break;
            case SystemLanguage.Greek: res = "EL"; break;
            case SystemLanguage.Hebrew: res = "IW"; break;
            case SystemLanguage.Hungarian: res = "HU"; break;
            case SystemLanguage.Icelandic: res = "IS"; break;
            case SystemLanguage.Indonesian: res = "IN"; break;
            case SystemLanguage.Italian: res = "IT"; break;
            case SystemLanguage.Japanese: res = "JA"; break;
            case SystemLanguage.Korean: res = "KO"; break;
            case SystemLanguage.Latvian: res = "LV"; break;
            case SystemLanguage.Lithuanian: res = "LT"; break;
            case SystemLanguage.Norwegian: res = "NO"; break;
            case SystemLanguage.Polish: res = "PL"; break;
            case SystemLanguage.Portuguese: res = "PT"; break;
            case SystemLanguage.Romanian: res = "RO"; break;
            case SystemLanguage.Russian: res = "RU"; break;
            case SystemLanguage.SerboCroatian: res = "SH"; break;
            case SystemLanguage.Slovak: res = "SK"; break;
            case SystemLanguage.Slovenian: res = "SL"; break;
            case SystemLanguage.Spanish: res = "ES"; break;
            case SystemLanguage.Swedish: res = "SV"; break;
            case SystemLanguage.Thai: res = "TH"; break;
            case SystemLanguage.Turkish: res = "TR"; break;
            case SystemLanguage.Ukrainian: res = "UK"; break;
            case SystemLanguage.Unknown: res = "EN"; break;
            case SystemLanguage.Vietnamese: res = "VI"; break;
        }
        //		Debug.Log ("Lang: " + res);
        return res;
    }

}