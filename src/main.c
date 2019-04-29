#include <stdio.h>
#include <string.h>
#define SSID_BUF_LEN 32
#define PWD_BUF_LEN 64

char char_hex2dec(char ch)
{
    char buffer;
    if (ch < 'A') {
        buffer = ch - 48;                           
    }else if (ch < 'a'){
        buffer = ch - 55;
    }else {
        buffer = ch - 87;
    }
    return buffer;
}

typedef struct _Wifi_config_t{
    char ssid[SSID_BUF_LEN];
    char pwd[PWD_BUF_LEN];
}Wifi_config_t;

int parse_config(Wifi_config_t *wconfig, char *text)
{
    size_t len = strlen(text);
    memset(wconfig->ssid, 0, SSID_BUF_LEN);
    memset(wconfig->pwd, 0, PWD_BUF_LEN);
    char temp[PWD_BUF_LEN] = {0};
    char *value_buf = NULL;
    char *pt = text;
    char *item_start = text;
    while(pt < text + len){
        // printf("%c\n", *pt);
        if(*pt == '=')
        {
            memcpy(temp, item_start, pt-item_start);
            // printf("temp = %s\n", temp);
            // printf("strcmp(\"ssid\", temp) = %d\n", strcmp("ssid", temp));
            // printf("strcmp(\"pwd\", temp) = %d\n", strcmp("pwd", temp));
            if(strcmp("ssid", temp) == 0){
                value_buf = wconfig->ssid;
            }
            else if(strcmp("pwd", temp) == 0)
            {
                value_buf = wconfig->pwd;
            }
            item_start = pt+1;
            memset(temp, 0, sizeof(temp));
            pt++;
        }
        else if(*pt == '+')     //代表空格
        {
            *pt = ' ';
            pt++;
        }
        else if(*pt == '%')     //'%'则紧跟两位十六进制表示的特殊字符
        {
            *pt = char_hex2dec(*(pt+1))*16 + char_hex2dec(*(pt+2));
            pt += 2;
        }
        else if(*pt == '&'|| (pt == text + len -1))     //'&'表示value_buf已经结束，name即将开始
        {
            memcpy(value_buf, item_start, pt-item_start);
            printf("value_buf=%s\n", value_buf);
            item_start = pt + 1;
            pt++;
        }
        else
        {
            pt++;
        }
    }
    printf("ssid = %s, pwd = %s\n", wconfig->ssid, wconfig->pwd);
    return 0;
}

int main(int argc, char const *argv[])
{
    char text[] = "ssid=abcd&pwd=gome1234";
    Wifi_config_t wconfig = {.ssid = {0}, .pwd = {0}};
    parse_config(&wconfig, text);
    
}