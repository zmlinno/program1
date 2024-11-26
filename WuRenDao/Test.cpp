#include<iostream>
#include<cctype>
#include<string>
#include<algorithm>

using namespace std;
// bool isNumber(const string& str)
// {
//     return all_of(str.begin(),str.end(),::isdigit);
// }
// int main()
// {
//     //char str[] = {1,2,3,4,5,6};
//     string str = "12345";
//     cout<<isNumber(str)<<endl;

//     cout<<endl;
//     return 0;
// }

int isNumber(const string& str)
{
    if(all_of(str.begin(),str.end(),::isdigit))
    {
        return true;
    }
    else return -1;
}
int main()
{
    string str1 = "123456";
    string str2 = "abcdef";
    cout<<isNumber(str1)<<endl;
    cout<<isNumber(str2)<<endl;
    cout<<endl;
    return 0;
}