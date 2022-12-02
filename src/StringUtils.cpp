#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    /*
        returns a stubstring of the string str,
        allows for negative values (for both start and end)
        as in python end == 0 means to include end of string.
    */
    std::string finalString;
    ssize_t finalStringStart;
    if (start >= 0) {
        finalStringStart = start;
    } else if (start < 0) {
        finalStringStart = str.length() + start;
    } 
    ssize_t finalStringEnd;
    if (end > 0) {
        finalStringEnd = end;
    } else if (end <= 0) {
        finalStringEnd = str.length() + end;
    }
    if (finalStringStart < 0 || finalStringEnd < 0 || finalStringStart > finalStringEnd){
        return "";
    } else {
        for (int i = finalStringStart; i < finalStringEnd; i++){
            finalString = finalString + str[i];
        }
    }
    return finalString;
}

std::string Capitalize(const std::string &str) noexcept{
    /*
        returns the capitalize string as in Python
    */
    if (str == "") {
        return "";
    }
    char firstChar;
    if (isalpha(str[0])) {
        firstChar = toupper(str[0]);
    } else {
        firstChar = str[0];
    }
    
    std::string finalString;
    finalString += firstChar;
    
    for (int i = 1; i < str.length(); i++){ 
        finalString += tolower(str[i]);
    }
    
    return finalString;
}

std::string Upper(const std::string &str) noexcept{
    /*
        returns the uppercase strings as in python.
    */
    std::string finalString = "";
    for (int i = 0; i < str.length(); i++){
        finalString += toupper(str[i]);
    }
    return finalString;
}

std::string Lower(const std::string &str) noexcept{
    /*
        returns the lowercase strings as in python.
    */
    std::string finalString = "";
    for (int i = 0; i < str.length(); i++){
        finalString += tolower(str[i]);
    }
    return finalString;
}

std::string LStrip(const std::string &str) noexcept{
    /*
        returns left stripped strings (white space characters are removed from the left)
    */
    std::string finalString = "";
    int started = 0;
    for (int i = 0; i < str.length(); i++){
        if (!isspace(str[i])){
            finalString += str[i];
            started = 1;
        } else if (started == 1) {
            finalString += str[i];
        }
    }
    return finalString;
}

std::string RStrip(const std::string &str) noexcept{
    /*
        returns right stripped strings (white space characters are removed from the right)
    */
    std::string finalString = "";
    int started = 0;
    for (int i = str.length() - 1; i >= 0; i--){
        if (!isspace(str[i])){
            finalString = str[i] + finalString;
            started = 1;
        } else if (started == 1) {
            finalString = str[i] + finalString;
        }
    }
    return finalString;
}

std::string Strip(const std::string &str) noexcept{
    /*
        returns stripped strings (white space characters are removed from the left and right)
    */
    std::string finalString;
    finalString = LStrip(str);
    finalString = RStrip(finalString);
    return finalString;
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    /*
        Returns the center justified strings
    */
    std::string finalString = "";
    if (width <= str.length()) {
        finalString = str;
        return finalString;
    }
    
    if ((width - str.length()) % 2 != 0){ //odd
        width = (width - str.length())/2;
        for (int i = 0; i < width + 1; i++) {
            finalString = finalString + fill;
        }
    } else {
        width = (width - str.length())/2;
        for (int i = 0; i < width; i++) {
            finalString = finalString + fill;
        }
    }
    
    for (int i = 0; i < str.length(); i++) {
        finalString = finalString + str[i];
    }
    
    for (int i = 0; i < width; i++) {
        finalString = finalString + fill;
    }
    return finalString;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    std::string finalString = "";
    if (width < str.length()) {
        finalString = str;
        return finalString;
    }
    width = width - str.length();
    for (int i = 0; i < str.length(); i++) {
        finalString = finalString + str[i];
    }
    for (int i = 0; i < width; i++) {
        finalString = finalString + fill;
    }
    return finalString;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    std::string finalString = "";
    if (width < str.length()) {
        finalString = str;
        return finalString;
    }
    width = width - str.length();
    for (int i = 0; i < width; i++) {
        finalString = finalString + fill;
    }
    for (int i = 0; i < str.length(); i++) {
        finalString = finalString + str[i];
    }
    return finalString;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    /*
        returns the string str with all instances of old replaced with rep
    */
    std::string finalString;
    int oldSize = old.length();
    int i = 0;
    while (i < str.length()) {
        std::string twoChar = Slice(str, i, i + oldSize);
        if (twoChar != old) {
            finalString += str[i];
            i++;
            continue;
        } else {
            finalString += rep;
            i = i + oldSize;
        }
    }
    return finalString;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
   /*
         Splits the string up into a vector of strings based on splt parameter, if splt parameter
         is empty string, then split on white space.
    */
    std::string finalString = "";
std::vector <std::string> finalVector;
    if (splt == "") {
        for (int i = 0; i < str.length(); i++) {
            if (isspace(str[i])) {
                if (finalString != "") {
                    finalVector.push_back(finalString);
                    finalString = "";
                }
                continue;
            } else {
                finalString = finalString + str[i];
                if (i == str.length() - 1) {
                    finalVector.push_back(finalString);
                }
            }
        }
        return finalVector;
    } else {
        // std::string finalString = "";
        // std::vector <std::string> finalVector;
        std::string split = splt;
        if (isspace(splt[0])) {
            std::string copyStr = str;
            int copyStrLength = copyStr.length();
            for (int i = 0; i < copyStrLength; i++){
                if (isspace(copyStr[i])|| i == copyStrLength - 1){
                    if (finalString != ""){
                    finalVector.push_back(finalString);
                    finalString = "";
                    }
                    continue;
                }
                finalString = finalString + copyStr[i];
            }
            return finalVector; 
        } else {
            int spltSize = splt.length();
            int i = 0;
            int already_pushed = 0; //notpushed
            while (i < str.length()) {
                std::string temp = Slice(str, i, i + spltSize);
                if (temp != splt) {
                    already_pushed = 0;
                    finalString += str[i];
                    i++;
                } else {
                    if (already_pushed == 0) {
                        finalVector.push_back(finalString);
                        finalString = "";
                        already_pushed = 1; //pushed
                    }
                    i = i + spltSize;
                }
            }
            finalVector.push_back(finalString);
            return finalVector;
        }
    }
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    /*
        Joins a vector of strings into a single string
    */
    int vectorSize = vect.size();
    std::string finalString;
    
    for (int i = 0; i < vectorSize; i++) {
        if (vectorSize == 1) {
            finalString += vect[i];
        } else if (vectorSize > 1){
            finalString += vect[i];
            if (i != vectorSize - 1) {
                finalString += str;
            }
        }
    }
    return finalString;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    /*
        Replaces tabs with spaces aligning at the tabstops
    */
    std::string copyStr = str;
    std::string finalString = "";
    int nextTabIndex = tabsize - 1;
    while (true) {
        int tabFound = copyStr.find("\t");
        if (tabFound == std::string::npos) {
            finalString = finalString + copyStr; //we will cut copyStr off little by little.
            break;
        }
        while (nextTabIndex <= tabFound + finalString.length() - 1) {
            nextTabIndex = nextTabIndex + tabsize;
        }
        finalString = finalString + Slice(copyStr, 0, tabFound);
        int spacesNeeded = nextTabIndex - finalString.length() + 1;
        for (int i = 0; i < spacesNeeded; i++) {
            finalString = finalString + " ";
        }
        copyStr = Slice(copyStr, tabFound, 0);
        copyStr = LStrip(copyStr);
    }
    return finalString;  
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    /*
        Calculates the Levenshtein distance (edit distance) between 2 strings.
        if ignorecase = true -> case insensitive.
           ignorecase = false -> case sensitive.
        Reference: https://www.youtube.com/watch?v=Dd_NgYVOdLk
    */
    std::string leftString;
    std::string rightString;
    if (ignorecase) { //case insensitive
        leftString = Lower(left);
        rightString = Lower(right);
    } else { //case sensitive
        leftString = left;
        rightString = right;
    }
    
    //Create 2D array and initialize to 0.
    std::vector <std::vector<int>> table (rightString.size() + 1, std::vector<int>(leftString.size()+ 1, 0));
    
    for (int i = 0; i < table.size(); ++i) {
        table[i][0] = i; //inserting to go from " " to a string.
    }
    for (int i = 0; i < table[0].size(); ++i){
        table[0][i] = i; //removing to go from something to " ".
    }
    for (int row = 1; row < table.size(); ++row) {
        for (int col = 1; col < table[0].size(); ++col){
            if (leftString[col - 1] == rightString[row - 1]){
                table[row][col] = table[row - 1][col - 1];
            } else {
                int val = table[row][col - 1];
                if (table[row - 1][col] < val){
                    val = table[row - 1][col];
                }
                if (table[row - 1][col - 1] < val) {
                    val = table[row - 1][col-1];
                }
                table[row][col] = val + 1;
            }
        }
    }
    return table[table.size() - 1][table[0].size() - 1];
}
};