#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
// recursive helper
void wordleHelper(const string& in, string& curr, int blanks, int pos, int floatCount, const set<string> dict, map<char, int>& floatings, set<string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    int n = in.size();
    set<string> results;

    // count number of floating for each character
    map<char, int> floatings;
    for(string::const_iterator it = floating.begin(); it != floating.end(); ++it){
        char c = *it;
        floatings[c]++;
    }
    int floatCount = floating.size();

    // count number of blanks
    int blanks = 0;
    for(int i = 0; i < n; i++){
        if(in[i] == '-'){
            blanks++;
        }
    }

    // call recursive helper
    string curr = in; // copy of in to avoid modifying in
    int pos = 0; // current "location" we are checking
    wordleHelper(in, curr, blanks, pos, floatCount, dict, floatings, results);

    // output the results
    return results;
}

// Define any helper functions here
void wordleHelper(const string& in, string& curr, int blanks, int pos, int floatCount, const set<string> dict, map<char, int>& floatings, set<string>& results){
    // base case: return if full word; only if floatings are used and word is valid, we add the word to results
    if(pos == in.size()){
        if(floatCount == 0 && dict.find(curr) != dict.end()){
            results.insert(curr);
        }
        return;
    }

    // already known letter, go to next recursive step
    if(in[pos] != '-'){
        curr[pos] = in[pos];
        wordleHelper(in, curr, blanks, pos + 1, floatCount, dict, floatings, results);
        return;
    }

    // iterative loop to recurse for floating and free letters
    for(char c = 'a'; c <= 'z'; c++){
        map<char, int>::iterator it = floatings.find(c);
        bool isFloating = false;
        if(it != floatings.end() && it->second > 0){
            isFloating = true;
        }
        if(isFloating || blanks > floatCount){
            // make a guess if floating allows, or we have extra unknown letters to skip this floating
            curr[pos] = c;
            if(isFloating){
                // if the floating is used, decrease the floatings and floatCount
                it->second -= 1; // assume we use this specific floating letter
                wordleHelper(in, curr, blanks - 1, pos + 1, floatCount - 1, dict, floatings, results);
                it->second += 1; // after we're finished, 'undo' the changes (matching words are already stored in results)
            } else {
                // not floating letter, just proceed and recurse
                wordleHelper(in, curr, blanks - 1, pos + 1, floatCount, dict, floatings, results);
            }
        }
    }
}
