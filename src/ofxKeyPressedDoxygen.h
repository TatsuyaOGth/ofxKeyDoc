#pragma once

#include "ofMain.h"

class ofxKeyPressedDoxygen
{
    string mFilePath;
    string mResult;
    unsigned int mNestCount;
    bool isKeyPressedLines;
    bool isReadKey;
    vector<string> mPatterns;
    vector<string> ofKeyDefines;
    
    // ===== singleton =====
    ofxKeyPressedDoxygen(){init();}
    ofxKeyPressedDoxygen(const ofxKeyPressedDoxygen&){init();}
    ofxKeyPressedDoxygen &operator=(const ofxKeyPressedDoxygen&){init();}
    // =====================
    
    void init()
    {
        mNestCount = 0;
        isKeyPressedLines = false;
        mPatterns.push_back("/**");
        mPatterns.push_back("///");
        
        ofKeyDefines.push_back("OF_KEY_MODIFIER");
        ofKeyDefines.push_back("OF_KEY_RETURN");
        ofKeyDefines.push_back("OF_KEY_ESC");
        ofKeyDefines.push_back("OF_KEY_TAB");
        ofKeyDefines.push_back("OF_KEY_COMMAND");
#ifdef TARGET_OSX
        ofKeyDefines.push_back("OF_KEY_BACKSPACE");
        ofKeyDefines.push_back("OF_KEY_DEL");
#else
        ofKeyDefines.push_back("OF_KEY_BACKSPACE");
        ofKeyDefines.push_back("OF_KEY_DEL");
#endif
        ofKeyDefines.push_back("OF_KEY_F1");
        ofKeyDefines.push_back("OF_KEY_F2");
        ofKeyDefines.push_back("OF_KEY_F3");
        ofKeyDefines.push_back("OF_KEY_F4");
        ofKeyDefines.push_back("OF_KEY_F5");
        ofKeyDefines.push_back("OF_KEY_F6");
        ofKeyDefines.push_back("OF_KEY_F7");
        ofKeyDefines.push_back("OF_KEY_F8");
        ofKeyDefines.push_back("OF_KEY_F9");
        ofKeyDefines.push_back("OF_KEY_F10");
        ofKeyDefines.push_back("OF_KEY_F11");
        ofKeyDefines.push_back("OF_KEY_F12");
        ofKeyDefines.push_back("OF_KEY_LEFT");
        ofKeyDefines.push_back("OF_KEY_UP");
        ofKeyDefines.push_back("OF_KEY_RIGHT");
        ofKeyDefines.push_back("OF_KEY_DOWN");
        ofKeyDefines.push_back("OF_KEY_PAGE_UP");
        ofKeyDefines.push_back("OF_KEY_PAGE_DOWN");
        ofKeyDefines.push_back("OF_KEY_HOME");
        ofKeyDefines.push_back("OF_KEY_END");
        ofKeyDefines.push_back("OF_KEY_INSERT");
        ofKeyDefines.push_back("OF_KEY_CONTROL");
        ofKeyDefines.push_back("OF_KEY_ALT");
        ofKeyDefines.push_back("OF_KEY_SHIFT");
        ofKeyDefines.push_back("OF_KEY_SUPER");
        ofKeyDefines.push_back("OF_KEY_LEFT_SHIFT");
        ofKeyDefines.push_back("OF_KEY_RIGHT_SHIFT");
        ofKeyDefines.push_back("OF_KEY_LEFT_CONTROL");
        ofKeyDefines.push_back("OF_KEY_RIGHT_CONTROL");
        ofKeyDefines.push_back("OF_KEY_LEFT_ALT");
        ofKeyDefines.push_back("OF_KEY_RIGHT_ALT");
        ofKeyDefines.push_back("OF_KEY_LEFT_SUPER");
        ofKeyDefines.push_back("OF_KEY_RIGHT_SUPER");
        ofKeyDefines.push_back("OF_KEY_LEFT_COMMAND");
        ofKeyDefines.push_back("OF_KEY_RIGHT_COMMAND");
    }
    
    static ofxKeyPressedDoxygen * getThis()
    {
        static ofxKeyPressedDoxygen * instance = new ofxKeyPressedDoxygen();
        return instance;
    }
    
    bool isEmpty(string & str)
    {
        if (str.empty() || str == "" || str == "\t") return true;
        return false;
    }
    
    bool isIn(const string & str, const string & targetStr)
    {
        unsigned int loc = str.find(targetStr, 0);
        return loc != string::npos;
    }
    
    void trim(string & str)
    {
        ofStringReplace(str, " ", ""); // trim space
        ofStringReplace(str, "\t", ""); // trim tab
    }
    
    void trimSideSpace(string & str)
    {
        unsigned int cut = 0;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == ' ' || str[i] == '\t')
            {
                cut++;
            }
            else break;
        }
        if (cut > 0) str.erase(0, cut);
        cut = str.size() - 1;
        for (int i = str.size() - 1; i >= 0; i--)
        {
            if (str[i] == ' ' || str[i] == '\t')
            {
                cut--;
            }
            else break;
        }
        if (cut < str.size() - 1) str.erase(cut);
    }
    
    unsigned int getDiscriptionLoc(const string & str)
    {
        for (int i = 0; i < mPatterns.size(); i++)
        {
            unsigned int loc = str.find(mPatterns[i], 0);
            if (loc != string::npos) return loc;
        }
        return -1;
    }
    
    string getDiscription(const string & src)
    {
        unsigned int start = getDiscriptionLoc(src);
        if (start == -1) return "";
        string dst = src;
        unsigned int end = dst.rfind("*/");
        if (end == string::npos)
        {
            return dst.substr(start + 3);
        }
        else {
            return dst.substr(start + 3, end);
        }
    }
    
    string getBetweenApostrophe(const string & src)
    {
        string dst = "";
        string tmp = src;
        while (1)
        {
            unsigned int loc = tmp.find("\'", 0);
            if (loc != string::npos)
            {
                if (!dst.empty()) dst += ", ";
                string key = tmp.substr(loc + 1, 1);
                dst += key;
                tmp = tmp.substr(loc + 3);
            } else break;
        }
        return dst;
    }
    
    string getAssigendOfKeyDefine(const string & src)
    {
        for (vector<string>::iterator it = ofKeyDefines.begin(); it != ofKeyDefines.end(); it++)
        {
            if (isIn(src, *it))
            {
                return (*it).substr(7);
            }
        }
    }
    
    string getAssignedKey(const string & src)
    {
        if (isIn(src, "\'"))
        {
            return getBetweenApostrophe(src);
        }
        else {
            return getAssigendOfKeyDefine(src);
        }
    }
    
    void execute()
    {
        stringstream dst;
        ofFile file(mFilePath);
        if (!file.exists())
        {
            ofLogError("ofxKeyAssignreader") << "fail not exist: " << mFilePath;
            return;
        }
        
        ofBuffer buffer(file);
        unsigned long tmpCount = 0;
        while (!buffer.isLastLine()) {
            string line = buffer.getNextLine();
            string trimedLine = line;
            trim(trimedLine);
            if (isEmpty(trimedLine)) continue;
            if (isKeyPressedLines)
            {
                if (isIn(line, "{")) mNestCount++;
                if (isIn(line, "}")) mNestCount--;
                
                if (mNestCount > 0)
                {
                    string dsc = getDiscription(line);
                    if (!dsc.empty())
                    {
                        trimSideSpace(dsc);
                        if (isReadKey)
                        {
                            string key = getAssignedKey(line);
                            dst << "[ " << key << " ] ";
                        }
                        dst << dsc << endl;
                    }
                }
                else {
                    isKeyPressedLines = false;
                }
            }
            else {
                if (isIn(trimedLine, "void") && isIn(trimedLine, "keyPressed(intkey)")) isKeyPressedLines = true;
                while (mNestCount == 0)
                {
                    if (isIn(line, "{")) mNestCount++;
                    else line = buffer.getNextLine();
                }
            }
        }
        mResult = dst.str();
    }
    
    
public:
    
    static void read(const string & filePath, bool read_key = true)
    {
        if (filePath.empty())
        {
            ofLogError("ofxKeyPressedDoxygen") << "err file name";
            return;
        }
        ofxKeyPressedDoxygen * o = getThis();
        o->mFilePath = filePath;
        o->isReadKey = read_key;
        o->execute();
    }
    
    static void saveDoc(const string & filePath = "keyassign.txt")
    {
        string & res = getThis()->mResult;
        ofstream out( filePath.c_str());
        out << res;
        out.close();
    }
    
    static string & getDoc() { return getThis()->mResult; }
    static string & getFilePath() { return getThis()->mFilePath; }
};
