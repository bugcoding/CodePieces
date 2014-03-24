#include <stdio.h>
#include <ctype.h>
#include "string_lib.h"


//get string length
unsigned int string_len(const char *str)
{
    const char *temp = str;

    while(*temp++);

    return temp - str - 1;
}

//reverse the string param
char *string_reverse(char *need_rev)
{
    if (need_rev == NULL)
    {
        return NULL;
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
        return need_rev;
    }
    else
    {
        return NULL;
    }
}

//get char pos of string
int string_firstindexof(const char *str, char ch)
{
    if (str == NULL)
    {
        return -1;
    }
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
//get char last pos of string
int string_lastindexof(const char *str, char ch)
{
    if (str == NULL)
    {
        return -1;
    }
    int len = string_len(str);
    const char *temp = &*(str + len - 1);
    int cnt = len - 1;
    while (*temp)
    {
        cnt--;
        if (*temp == ch)
        {
            return cnt;
        }
        temp--;
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
//delete space
boolean string_trim(char *str)
{
    if (str == NULL)
    {
        return false;
    }
    string_deltailspace(str);
    string_reverse(str);
    string_deltailspace(str);
    string_reverse(str);
    return true;
}

void string_deltailspace(char *str)
{
    int len = string_len(str);
    for (int i = len - 1; i >= 0; i--)
    {
        if (str[i] != ' ')
        {
            str[i + 1] = '\0';
            break;
        }
    }
}

//check string is words or not
boolean string_iswords(const char *str)
{
    const char *temp = str;
    if (isdigit(*temp))
    {
        return false;
    }

    while (*temp)
    {
        if ((*temp < 'a' && *temp > 'Z' && *temp != '_') || *temp > 'z' 
                || (*temp < 'A' && !isdigit(*temp)))
        {
            return false;
        }
        temp++;
    }
    return true;
}

int main(int argc, const char *argv[])
{
    char str[] = "  s___A";
    printf("%u\n", string_len(str));


    //boolean b = string_reverse(str);
    //printf("%d\n", b);
    //printf("%s\n", str);
    //printf("%d\n", string_lastindexof(str, '3'));
    //char t[32] = "\0";
    //string_substr_2index(str, t, 5, 4);
    //string_substr_length(str, t, -2, 14);
    //printf("t == %s\n", t);
    string_trim(str);
    printf("str == %s", str);
    printf("---------------\n");
    
    boolean b = string_iswords(str);
    printf("is_words %d\n", b);

    
    return 0;
}
