using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Runtime.InteropServices;
using System.IO;
public class WebsiteCaller : MonoBehaviour
{
    [SerializeField] TMPro.TextMeshProUGUI websiteText;
    [SerializeField] TMPro.TextMeshProUGUI uploadText;
    [SerializeField] Button button;
    string currentText;
    
    [DllImport("CPPToCSharp.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern int GetCharSize();

    [DllImport("CPPToCSharp.dll", CallingConvention = CallingConvention.Cdecl)]
    static extern char GetWebsiteContent(int index);

    [DllImport("CPPToCSharp.dll", EntryPoint = "PostToWebsite", CallingConvention = CallingConvention.Cdecl)]
    static extern char PostToWebsite(char c_fullpath, bool lastIndex, int index);

    [DllImport("CPPToCSharp.dll", EntryPoint = "SetWebsiteLength", CallingConvention = CallingConvention.Cdecl)]
    static extern void SetWebsiteLength(int i);

    // Start is called before the first frame update
    void Start()
    {
        int size = GetCharSize();
        string s = "";
        for (int i = 0; i < size; i++)
            s += GetWebsiteContent(i);
        currentText = s;
        websiteText.text = currentText;
    }
    public void SendToFile(string s,string fullpath)
    {
        if (!File.Exists(fullpath))
            File.Create(fullpath).Close();
        using (StreamWriter writer = new StreamWriter(fullpath))
        {
            writer.WriteLine(s);
        }
        int lastI = 0;
        for (int i = 0; i < fullpath.Length; i++)
        {
            PostToWebsite(fullpath[i], false, i);
            lastI = i;
        }
        print(PostToWebsite('\0', true, lastI+1));
    }
    public void SetWebsite()
    {
        currentText = uploadText.text;
        button.interactable = false;
        string folder = "Assets";
        string currentDir = Directory.GetCurrentDirectory();
        string subFolder = currentDir + "\\" + folder;
        string fileName = "Messanger.txt";
        string fullPath = $@"{subFolder}\{fileName}";
        SetWebsiteLength(fullPath.Length+1);
        SendToFile(currentText, fullPath);
    }
}
