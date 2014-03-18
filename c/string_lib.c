#include <stdio.h>
#include "string_lib.h"


//get string length
unsigned int string_len(const char *str)
{
    const char *temp = str;

    while(*temp++);

    return temp - str - 1;
}

//reverse the string param
boolean string_reverse(char *need_rev)
{
    if (need_rev == NULL)
    {
        return false;
    }

    int len = string_len(need_rev) - 1;
    int i = 0;
    for (i = (len - 1) >> 1; i >= 0; i--)
    {
        char temp = need_rev[i];
        need_rev[i] = need_rev[len - i];
        need_rev[len - i] = temp;
    }
    if (i == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//get char pos of string
int string_firstindexof(const char *str, char ch)
{
    const char *temp = str;
    int cnt = 0;
    while (*temp)
    {
        cnt++;
        if (*temp == ch)
        {
            return cnt;
        }
        temp++;
    }
    return -1;
}

//return substring of param between 2 index
boolean string_substr_2index(const char *str, char *sub_str, int offset, int end)
{
    if (str == NULL)
    {
        return false;
    }
    int index = 0;
    //out of bounds
    if (offset < end && (offset > string_len(str) || end > string_len(str)))
    {
        return false;
    }
    for (int i = offset; i <= end; i++)
        sub_str[index++] = str[i];

    sub_str[index] = '\0';
    return true;
}
//return substring with index and length
boolean string_substr_length(const char *str, char *sub_str, int offset, int len)
{
    if (str == NULL)
    {
        return false;
    }
    int str_len = string_len(str);

    int index = 0;
    if (offset >= 0 && offset < str_len)
    {
        len = (len > str_len) ? str_len : len;
        for (int i = offset; i < len + offset; i++)
        {
            sub_str[index++] = str[i];
        }
        sub_str[index] = '\0';
    }
    else if (offset < 0 && -offset < str_len)
    {	
        //compute start point when cut string from tail
        int start = (len > str_len) ? 0 : offset + len;
        //length up overflow, reset to max string length 
        len = (len > str_len) ? str_len : len;
        for (int i = start; i < len + offset; i++)
        {
            sub_str[index++] = str[i];
        }
        sub_str[index] = '\0';
    }
    return true;
}

int main(int argc, const char *argv[])
{
    char str[] = "1234567890";
    printf("%u\n", string_len(str));


    boolean b = string_reverse(str);
    printf("%d\n", b);
    printf("%s\n", str);
    printf("%d\n", string_firstindexof(str, '3'));
    char t[32] = "\0";
    //string_substr_2index(str, t, 5, 4);
    string_substr_length(str, t, -2, 14);
    printf("t == %s\n", t);

    return 0;
}
