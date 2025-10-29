#ifndef UTILS_H
#define UTILS_H
#include <bits/stdc++.h>
using namespace std;

bool validatePhoneStrict(const string& in);
string normalizeName(const string& in, bool& ok);
string fitLeft(const string& s, int w);
void drawSep(const vector<int>& W);
void drawRow(const vector<int>& W,const vector<string>& cells);

#endif
