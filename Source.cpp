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

    cout << "suffix araay:\n";
    for (int i = 0; i < len_text; i++) {
        cout << "index: " << suff[i].index << " array: " << suff[i].suffix << "\n";
    }

    // Returns the computed suffix array
    return suffix_arr;
}

// Takes suffix array and its size
// as arguments and returns the
// Burrows - Wheeler Transform of given text
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

void getC(string str, int len) {
    int i = 1;
    while (i < str.size()) {
        C[str[i]] = i;
        int r = str.rfind(str[i]);
        i = r + 1;
    }
}

void getOCC(string f, string l, string p) {
    for (int i = 0; i < l.size(); i++) {
        if (l[i] == '$') {
            continue;
        }
        auto it = OCC.find(l[i]);
        if (it != OCC.end()) {
            int num = *(it->second.end() - 1) + 1;
            while ((it->second.size() - 1) != i) {
                it->second.push_back(num);
            }
        }
        else {
            OCC[l[i]].push_back(0);
            for (int j = 0; j < i; j++) {
                OCC[l[i]].push_back(0);
            }
        }
    }
}

bool match(string p) {
    int i = p.size() - 1;
    char c = p[i];
    auto it = C.find(c);
    if (it == C.end()) {
        return false;
    }
    int sp = (it++)->second, ep = it->second;
    //cout << sp << " " << ep << endl;
    i--;

    while (i >= 0) {
        c = p[i];
        it = C.find(c);
        if (it == C.end()) {
            return false;
        }
        int skip = it->second;

        if (sp >= OCC[c].size()) {
            sp = *(OCC[c].end() - 1) + skip + 1;
        }
        else {
            sp = OCC[c][sp] + skip;
        }

        if (ep >= OCC[c].size()) {
            ep = *(OCC[c].end() - 1) + skip + 1;
        }
        else {
            ep = OCC[c][ep] + skip;
        }
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
    char input_text[100];
    //BWT
    printf("Input text :");
    scanf("%s", input_text);
    int len_text = strlen(input_text);
    input_text[len_text++] = '$';
    //cout << input_text;

    // Computes the suffix array of our text
    int* suffix_arr = computeSuffixArray(input_text, len_text);

    // Adds to the output array the last char
    // of each rotation
    char* bwt_arr = findLastChar(input_text, suffix_arr, len_text);

    printf("Burrows - Wheeler Transform : %s\n",
        bwt_arr);

    //pattern matching
    string p;
    printf("Please input the pattern: ");
    cin >> p;

    string str = input_text;
    str = str.substr(0, len_text);
    sort(str.begin(), str.end());
    getC(str, len_text);
    getOCC(str, bwt_arr, p);
    bool re = match(p);
    
    if (re) {
        printf("Success!\n");
    }
    else {
        printf("Fail\n");
    }



    return 0;
}