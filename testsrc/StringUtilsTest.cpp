#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    std::string str = "MinhGiangTran";
    EXPECT_EQ(StringUtils::Slice(str, 0, 0), "MinhGiangTran"); //From start to end.
    EXPECT_EQ(StringUtils::Slice(str, -3), "ran"); //last 3 chars
    EXPECT_EQ(StringUtils::Slice(str, -6, -3), "ngT"); //start and end both negative.
    EXPECT_EQ(StringUtils::Slice(str, 0, -3), "MinhGiangT");
    EXPECT_EQ(StringUtils::Slice(str, -18), ""); //start is out of bounds
    EXPECT_EQ(StringUtils::Slice(str, -0, -17), ""); //end is out of bounds
    EXPECT_EQ(StringUtils::Slice(str, 9, 6), ""); //start comes after end.
    EXPECT_EQ(StringUtils::Slice(str, 1), "inhGiangTran"); //positive start.
    
    std::string str2 = "";
    EXPECT_EQ(StringUtils::Slice(str2, -6, -3), "");//empty string.
    
    std::string str3 = "H";
    EXPECT_EQ(StringUtils::Slice(str3, -1, -1), "");//start and end are the same. should return empty string.
}

TEST(StringUtilsTest, Capitalize){
    std::string str = "minZhengtt";
    EXPECT_EQ(StringUtils::Capitalize(str), "Minzhengtt"); //only letters
    
    std::string str1 = "11minZhe3ngtt";
    EXPECT_EQ(StringUtils::Capitalize(str1), "11minzhe3ngtt"); //letters and numbers
    
    std::string str2 = "  11minzhe 3ngtt";
    EXPECT_EQ(StringUtils::Capitalize(str2), "  11minzhe 3ngtt"); //letters and numbers and space at the beginning and middle
    
    std::string str3 = "";
    EXPECT_EQ(StringUtils::Capitalize(str3), ""); //empty string    
}

TEST(StringUtilsTest, Upper){
    std::string str = "HeLlO 123 Ln";
    EXPECT_EQ(StringUtils::Upper(str), "HELLO 123 LN"); // has numbers, letters and spaces.
    
    std::string str1 = "heyyyy";
    EXPECT_EQ(StringUtils::Upper(str1), "HEYYYY"); //has only lowercase letters.
    
    std::string str2 = "";
    EXPECT_EQ(StringUtils::Upper(str2), ""); //empty string
    
    std::string str3 = "SUP PPP";
    EXPECT_EQ(StringUtils::Upper(str3), "SUP PPP");  //has only uppercase letters & a space.
}

TEST(StringUtilsTest, Lower){
    std::string str = "HeLlO 123 Ln";
    EXPECT_EQ(StringUtils::Lower(str), "hello 123 ln"); // has numbers, letters and spaces.
    
    std::string str1 = "heyyyy";
    EXPECT_EQ(StringUtils::Lower(str1), "heyyyy"); //has only lowercase letters.
    
    std::string str2 = "";
    EXPECT_EQ(StringUtils::Lower(str2), ""); //empty string
    
    std::string str3 = "SUP PPP";
    EXPECT_EQ(StringUtils::Lower(str3), "sup ppp");  //has only uppercase letters & a space.

}

TEST(StringUtilsTest, LStrip){
    std::string str = "   HeLlO 123 Ln  ";
    EXPECT_EQ(StringUtils::LStrip(str), "HeLlO 123 Ln  "); //has spaces on the left.
    
    std::string str1 = "Hiiii";
    EXPECT_EQ(StringUtils::LStrip(str1), "Hiiii"); //doesn't have spaces on the left.
    
    std::string str2 = "   ";
    EXPECT_EQ(StringUtils::LStrip(str2), ""); //has only spaces.
    
    std::string str3 = "";
    EXPECT_EQ(StringUtils::LStrip(str3), ""); //empty string.
}

TEST(StringUtilsTest, RStrip){
    std::string str = "   HeLlO 123 Ln  ";
    EXPECT_EQ(StringUtils::RStrip(str), "   HeLlO 123 Ln"); //has spaces on the right.
    
    std::string str1 = "Hiiii";
    EXPECT_EQ(StringUtils::RStrip(str1), "Hiiii"); //doesn't have spaces on the right.
    
    std::string str2 = "   ";
    EXPECT_EQ(StringUtils::RStrip(str2), ""); //has only spaces.
    
    std::string str3 = "";
    EXPECT_EQ(StringUtils::RStrip(str3), ""); //empty string.
}

TEST(StringUtilsTest, Strip){
    std::string str = "   HeLlO 123 Ln  ";
    EXPECT_EQ(StringUtils::Strip(str), "HeLlO 123 Ln"); //has spaces on both sides.
    
    std::string str1 = "Hiiii";
    EXPECT_EQ(StringUtils::Strip(str1), "Hiiii"); //doesn't have spaces on either side.
    
    std::string str2 = "   ";
    EXPECT_EQ(StringUtils::Strip(str2), ""); //has only spaces.
    
    std::string str3 = "";
    EXPECT_EQ(StringUtils::Strip(str3), ""); //empty string.
}

TEST(StringUtilsTest, Center){
    /* 
        Check when...
        - we need to center an odd number.
        - we need to center an even number.
        - str longer than width.
    */

    std::string str = "I love geeksforgeeks";
    EXPECT_EQ(StringUtils::Center(str, 41, '#'), "###########I love geeksforgeeks##########");
    EXPECT_EQ(StringUtils::Center(str, 40, '#'), "##########I love geeksforgeeks##########");
    EXPECT_EQ(StringUtils::Center(str, 40), "          I love geeksforgeeks          "); //default value
    EXPECT_EQ(StringUtils::Center(str, 5, '#'), "I love geeksforgeeks"); //less than size

    std::string str1 = " ";
    EXPECT_EQ(StringUtils::Center(str1, 3, '-'), "- -");
    EXPECT_EQ(StringUtils::Center(str1, 4, '-'), "-- -");

    std::string str2 = "";
    EXPECT_EQ(StringUtils::Center(str2, 4, '-'), "----"); //empty string

}

TEST(StringUtilsTest, LJust){

    std::string str = "I love geeksforgeeks";
    EXPECT_EQ(StringUtils::LJust(str, 25, '#'), "I love geeksforgeeks#####"); //non-default value.
    EXPECT_EQ(StringUtils::LJust(str, 25), "I love geeksforgeeks     "); //default value.

    std::string str1 = "Heyyy";
    EXPECT_EQ(StringUtils::LJust(str1, 0, '#'), "Heyyy"); //non-default value.
    EXPECT_EQ(StringUtils::LJust(str1, 0), "Heyyy"); //default value.
    
    std::string str2 = "";
    EXPECT_EQ(StringUtils::LJust(str2, 4, '-'), "----"); //empty string
}

TEST(StringUtilsTest, RJust){

    std::string str = "I love geeksforgeeks";
    EXPECT_EQ(StringUtils::RJust(str, 28, '#'), "########I love geeksforgeeks"); //non-default value.
    EXPECT_EQ(StringUtils::RJust(str, 28), "        I love geeksforgeeks"); //default value.

    std::string str1 = "Heyyy";
    EXPECT_EQ(StringUtils::RJust(str1, -3, '#'), "Heyyy"); //non-default value.
    EXPECT_EQ(StringUtils::RJust(str1, 3), "Heyyy"); //default value.

    std::string str2 = "";
    EXPECT_EQ(StringUtils::RJust(str2, 4, '-'), "----"); //empty string
    
}

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("\\Path\\To\\A\\Directory","\\","/"), "/Path/To/A/Directory");
    EXPECT_EQ(StringUtils::Replace("a = b + c * b + d","b","e"), "a = e + c * e + d");
    EXPECT_EQ(StringUtils::Replace("aabbccaaabbbcccaaaa","aa","ee"), "eebbcceeabbbccceeee");
}

TEST(StringUtilsTest, Split){
    auto PathComponents = StringUtils::Split("/Path/To/A/Directory","/");
    ASSERT_EQ(PathComponents.size(), 5);
    EXPECT_EQ(PathComponents[0], "");
    EXPECT_EQ(PathComponents[1], "Path");
    EXPECT_EQ(PathComponents[2], "To");
    EXPECT_EQ(PathComponents[3], "A");
    EXPECT_EQ(PathComponents[4], "Directory");
   
   
    auto SentenceComponents = StringUtils::Split("A  tougher\ttest\nto   pass!");
    ASSERT_EQ(SentenceComponents.size(), 5);
    EXPECT_EQ(SentenceComponents[0], "A");
    EXPECT_EQ(SentenceComponents[1], "tougher");
    EXPECT_EQ(SentenceComponents[2], "test");
    EXPECT_EQ(SentenceComponents[3], "to");
    EXPECT_EQ(SentenceComponents[4], "pass!");
    
}

TEST(StringUtilsTest, Join){
    std::vector <std::string> vector;
    //empty vector
    EXPECT_EQ(StringUtils::Join("ab", vector), "");

    vector.push_back("Heyy");
    vector.push_back("Hii");
    EXPECT_EQ(StringUtils::Join("ab", vector), "HeyyabHii");
    EXPECT_EQ(StringUtils::Join(" ", vector), "Heyy Hii");
    EXPECT_EQ(StringUtils::Join("", vector), "HeyyHii"); //empty string

    vector.push_back("Hello");
    EXPECT_EQ(StringUtils::Join("abc", vector), "HeyyabcHiiabcHello");
    EXPECT_EQ(StringUtils::Join(" ", vector), "Heyy Hii Hello");
    EXPECT_EQ(StringUtils::Join("", vector), "HeyyHiiHello"); //empty string    
}

TEST(StringUtilsTest, ExpandTabs){
        EXPECT_EQ(StringUtils::ExpandTabs("1\t2\t3\t4"), "1   2   3   4");
        EXPECT_EQ(StringUtils::ExpandTabs("1\t12\t123\t1234"), "1   12  123 1234");
        EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1"), "1234    123 12  1");
        EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",3), "1234  123   12 1");
        EXPECT_EQ(StringUtils::ExpandTabs("1234\t123\t12\t1",0), "1234123121");
}

TEST(StringUtilsTest, EditDistance){
    std::string left = "replace";
    std::string right = "dElete";
    //case sensitive
    EXPECT_EQ(StringUtils::EditDistance(left, right, false), 5);
    //case insensitive
    EXPECT_EQ(StringUtils::EditDistance(left, right, true), 4);

    left = "sundaY";
    right = "saturday";
    //case sensitive
    EXPECT_EQ(StringUtils::EditDistance(left, right, false), 4);
    //case insensitive
    EXPECT_EQ(StringUtils::EditDistance(left, right, true), 3); 

    left = "";
    right = "dElete";
    //6 deletes -> case sensitive/insensitive doesn't matter.
    EXPECT_EQ(StringUtils::EditDistance(left, right, false), 6);
    EXPECT_EQ(StringUtils::EditDistance(left, right, true), 6);

    left = "";
    right = "";
    //2 empty spaces.
    EXPECT_EQ(StringUtils::EditDistance(left, right, false), 0);
}
