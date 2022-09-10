// C program to find Burrows Wheeler transform
// of a given text
//To be notice => the max length for thestring is 100
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <fstream>
#include <algorithm>

using namespace std;

// Structure to store data of a rotation
struct rotation {
    int index;
    char* suffix;
};

// Compares the rotations and
// sorts the rotations alphabetically
int cmpfunc(const void* x, const void* y)
{
    struct rotation* rx = (struct rotation*)x;
    struct rotation* ry = (struct rotation*)y;
    return strcmp(rx->suffix, ry->suffix);
}

// Takes text to be transformed and its length as
// arguments and returns the corresponding suffix array
int* computeSuffixArray(char* input_text, int len_text)
{
    // Array of structures to store rotations and
    // their indexes
    struct rotation suff[100];

    // Structure is needed to maintain old indexes of
    // rotations after sorting them
    for (int i = 0; i < len_text; i++) {
        suff[i].index = i;
        suff[i].suffix = (input_text + i);
    }

    // Sorts rotations using comparison
    // function defined above
    qsort(suff, len_text, sizeof(struct rotation),
        cmpfunc);

    // Stores the indexes of sorted rotations
    int* suffix_arr
        = (int*)malloc(len_text * sizeof(int));
    for (int i = 0; i < len_text; i++)
        suffix_arr[i] = suff[i].index;

    // Returns the computed suffix array
    return suffix_arr;
}


char* findLastChar(char* input_text,
    int* suffix_arr, int n)
{
    // Iterates over the suffix array to find
    // the last char of each cyclic rotation
    char* bwt_arr = (char*)malloc(n * sizeof(char));
    int i;
    for (i = 0; i < n; i++) {
        // Computes the last char which is given by
        // input_text[(suffix_arr[i] + n - 1) % n]
        int j = suffix_arr[i] - 1;
        if (j < 0)
            j = j + n;
        
        bwt_arr[i] = input_text[j];
    }

    bwt_arr[i] = '\0';

    // Returns the computed Burrows - Wheeler Transform
    return bwt_arr;
}

map<char, int> C;
map<char, vector<int>> OCC;
map<char, int> keep;
string l;

void getC(string str, int len) {
    int i = 1;
    while (i < str.size()) {
        C[str[i]] = i;
        int r = str.rfind(str[i]);
        i = r + 1;
    }
}

void getOCC() {
    vector<int> v((l.size() - 1) / 5 + 1, 0);
    for (int i = 0; i < l.size(); i++) {
        if (l[i] == '$') { continue; }
        OCC[l[i]] = v;
    }

    for (int i = 1; i <= 5 * (v.size() - 1); i++) {
        if (l[i - 1] == '$') {
            continue;
        }
        keep[l[i - 1]]++;
        //cout << l[i] << " " << keep[l[i]] << "\n";
        if (i % 5 == 0) {
            OCC[l[i - 1]][i / 5] = keep[l[i - 1]];
            continue;
        }
        OCC[l[i - 1]][i / 5 + 1] = keep[l[i - 1]];
    }
}

int fastCalculateOCC(char c, int index) {
    if (index % 5 == 0) {
        return OCC[c][index / 5];
    }
    else {
        int sum = 0;
        if (index % 5 == 1 || index % 5 == 2) {
            index--;
            while (index % 5 != 0) {
                if (l[index] == c) {
                    if (l[index] == c) {
                        sum++;
                    }
                }
                index--;
            }
            if (l[index] == c) {
                if (l[index] == c) {
                    sum++;
                }
            }
            //cout << "sum: " << sum << endl;
            return OCC[c][index / 5] + sum;
        }
        else {
            while (index % 5 != 0) {
                if (l[index] == c) {
                    if (l[index] == c) {
                        sum++;
                    }
                }
                index++;
            }
            //cout << "sum: " << sum << endl;
            return OCC[c][index / 5] - sum;
        }
    }
}

bool match(string p) {
    int i = p.size() - 1;
    //cout << "range: " << i << endl;
    char c = p[i];
    auto it = C.find(c);
    if (it == C.end()) {
        return false;
    }
    int sp = (it++)->second;
    if (it == C.end()) {
        return false;
    }
    int ep = it->second;
    //cout << sp << " " << ep << endl;
    //cout << "drbgr";
    i--;

    while (i >= 0) {
        c = p[i];
        it = C.find(c);
        if (it == C.end()) {
            return false;
        }
        int skip = it->second;

        if (sp < l.size()) {
            sp = fastCalculateOCC(c, sp) + skip;
        }
        else {
            sp = fastCalculateOCC(c, l.size() - 1) + skip + (l[l.size() - 1] == c);
        }

        if (ep < l.size()) {
            ep = fastCalculateOCC(c, ep) + skip;
        }
        else {
            ep = fastCalculateOCC(c, l.size() - 1) + skip + (l[l.size() - 1] == c);
        }
        //cout << sp << " " << ep << endl;
        i--;
        if (ep <= sp) {
            return false;
        }
    }

    return true;
}

// Driver program to test functions above
int main()
{
    int sum = 0, num = 0;
    ifstream fin("pattern.txt");
    char s[10];
    vector<string> pattern_vec;

    while (fin >> s) {
        pattern_vec.push_back(s);
    }
    
    char input_text[10];
    // scanf("%s",input_text);
    ifstream fin2("string.txt");
    while (fin2 >> input_text) {
        num++;
        int len_text = strlen(input_text);
        input_text[len_text++] = '$';
        //cout << input_text;

        // Computes the suffix array of our text
        int* suffix_arr = computeSuffixArray(input_text, len_text);

        // Adds to the output array the last char
        // of each rotation
        char* bwt_arr = findLastChar(input_text, suffix_arr, len_text);

        // printf("Burrows - Wheeler Transform : %s\n",            bwt_arr);

        l = bwt_arr;
        string str = input_text;
        str = str.substr(0, len_text);
        sort(str.begin(), str.end());
        getC(str, len_text);
        getOCC();

        //pattern matching
        string p;
        // printf("Please input the pattern: ");
        //--------------------------------------------
        // cin >> p;
        for (int i = 0; i < pattern_vec.size(); i++) {

            p = pattern_vec[i];
            bool re = match(p);
            // cout<<i;
            if (re) {
                sum++;
                //cout << p << " " << input_text << endl;
                break;
            }



        }
        //cout << "fewu";
        OCC.clear();
        C.clear();
        //-----------------------------------------------


    }
    cout << "match: " << sum << endl;
    cout << "number: " << num << endl;


    //BWT



    return 0;
}