#include "StringUtils.h"
#include <iostream>
#include <cctype>
#include <vector>

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    int N = str.length();


    //if blank string
    if (N == 0){
        return "";
    }


    //End is more than last index
    if (end >=N){
        end=N;
    }

    //negative end
    if (end<0){
        end=N+end;
    }  

    //make sure end is within bounts
    if (end < 0){
        return "";
    }    
    //if end ==0
    if (end==0){
        end = N;
    }
    

    //negative start
    if (start<0){
        start=N+start;
    }

    //make sure start is within bounds(cannot be N, max is N-1)
    if (start>=N || start < 0){
        return "";
    }
  
  

    //populate return string 
    std::string out = "";
    for (int i=start; i< end; i++){
        out = out + str[i];
    }
    return out;
}

std::string Capitalize(const std::string &str) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }

    std::string Return = str;
    Return[0] = toupper(str[0]);
    for (int i=1; i < N; i++){
        Return[i] = tolower(str[i]);
    }
    return Return;
}

std::string Upper(const std::string &str) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }

    std::string ret = "";
    for (int i=0; i < N; i++){
        ret = ret + char(toupper(str[i]));
    }

    

    return ret;
}

std::string Lower(const std::string &str) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }

    std::string ret = "";
    for (int i=0; i < N; i++){
        ret = ret + char(tolower(str[i]));
    }

    

    return ret;
}

std::string LStrip(const std::string &str) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }

    int i = 0;
    while (i<N && isspace(str[i])){
        i += 1;
    }

    return StringUtils::Slice(str, i);
}

std::string RStrip(const std::string &str) noexcept{
    int N = str.length();

    if (N==0){
        return "";
    }

    int i = N-1;
    while (i>-1 && isspace(str[i])){
        i -= 1;
    }
    if (i==-1){
        return "";
    }
    i += 1;
    
    if (i==N){
        i=0;
    }
    return StringUtils::Slice(str, 0, i);
}

std::string Strip(const std::string &str) noexcept{
    return StringUtils::RStrip(StringUtils::LStrip(str));
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }
    if (N>=width){
        return str;
    } 
    std::string out = str;
    std::string side = "right";
    for (int i = 0; i < width-N; i++){
        if (side=="right"){
            out = out + fill;
            side = "left";
        } else {
            out= fill + out;
            side= "right";
        }
    }
    
    return out;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }
    if (N>=width){
        return str;
    } 
    std::string out = str;
    for (int i = 0; i < width-N; i++){
        out = out + fill;
    }
    
    return out;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }
    if (N>=width){
        return str;
    } 
    std::string out = "";
    
    for (int i = N; i < width; i++){
        
        out = out + fill;
    }
    out = out + str;

    return out;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }

    int subPointer = 0;
    int streak = 0;
    std::string ret = str;
    for (int i = 0; i < N; i++){
        if (str[i]==old[subPointer]){
            streak += 1;

            if (streak == old.length()){
                ret.replace(i-old.length()+1, old.length(), rep);
                
                
                streak  = 0;
                subPointer = 0;
            }
            subPointer+= 1;
        } else {
            subPointer = 0;
            streak = 0;
        }
    }
    return ret;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    int N = str.length();
    if (N==0){
        return {};
    }
    std::string dlim = splt;
    if (splt==""){
        dlim = " ";
    } 
    
    std::vector<std::string> vec;
    int l = 0;
    int r = 0;

    while (r < N){
        std::string s(1, str[r]);
        if (s == dlim){
            vec.push_back(StringUtils::Slice(str, l, r));
            l = r+1;
        }
        r += 1;
    }
    vec.push_back(StringUtils::Slice(str, l, 0));
    return vec;
}

std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    std::string ret = "";
    int N =vect.size();
    for (int i=0; i< N; i++){
        ret = ret + vect[i];
        if (i != N-1){
            ret = ret + str;
        }
    }
    return ret;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    int N = str.length();
    if (N==0){
        return "";
    }
    std::string copy = str;
    std::string rep = "";
    for (int j=0; j< tabsize; j++){
        rep = rep + " ";
    }

    for (int i=0; i< N; i++){
        if (copy[i] == '\t'){
            
            copy.replace(i, 1, rep);
        }
    }
    return copy;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    int leftN = left.length(); 
    int rightN = right.length();

    if (leftN==0 || rightN==0){
        return leftN+rightN;
    }
    std::vector<std::vector<int>> dp;
    for (int i=0; i<leftN+1;i++){
        std::vector<int> row;
        for (int j=0;j<rightN+1;j++){
            row.push_back(0);
        }
        dp.push_back(row);
    }

    for (int i=0; i< leftN+1; i++){
        dp[i][0]=i;
    }

    for (int i=0; i< rightN+1; i++){
        dp[0][i]=i;
    }   

    for (int i=1; i< leftN+1; i++){
        for (int j=1; j< rightN+1; j++){
            //std::cout<<i<<", "<<j<<std::endl;
            int l = dp[i-1][j]+1;

            int d = dp[i][j-1]+1;
            int ld = dp[i-1][j-1];
            if (
                (left[i-1] != right[j-1] && ignorecase==false)
                || 
                (
                    ignorecase==true && ( toupper(left[i-1]) != toupper(right[j-1]) )
                )
                ){
                ld += 1;
            }
            int lvd = std::min(l,d);
            dp[i][j] = std::min(lvd, ld);
    
        }
    } 
    return dp[leftN][rightN];
;
}

};