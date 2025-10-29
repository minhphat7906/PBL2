#include "Utils.h"

bool validatePhoneStrict(const string& in){
    if(in.empty())return false;
    for(char c:in)if(!isdigit((unsigned char)c))return false;
    if(!(in.size()==9||in.size()==10||in.size()==11))return false;
    if(in[0]!='0')return false;
    if(in.size()>=2&&in[1]=='0')return false;
    return true;
}

string normalizeName(const string& in,bool& ok){
    ok=false; string out; bool lastSpace=true,hasLetter=false;
    for(unsigned char c:in){
        if(c==' '){if(!lastSpace){out.push_back(' ');lastSpace=true;}continue;}
        if(isdigit(c)||ispunct(c)||iscntrl(c))return string();
        out.push_back(char(c));lastSpace=false;hasLetter=true;
    }
    if(!out.empty()&&out.back()==' ')out.pop_back();
    if(!hasLetter)return string();
    ok=true; return out;
}

string fitLeft(const string& s,int w){if((int)s.size()>=w)return s.substr(0,w);return s+string(w-s.size(),' ');}
void drawSep(const vector<int>& W){cout<<'+';for(int w:W)cout<<string(w,'-')<<'+';cout<<"\n";}
void drawRow(const vector<int>& W,const vector<string>& cells){cout<<'|';for(size_t i=0;i<W.size();++i){string cell=(i<cells.size()?cells[i]:"");cout<<fitLeft(cell,W[i])<<'|';}cout<<"\n";}
