//
//  main.c
//  nali
//
//  Created by wenlong on 15/12/9.
//  Copyright © 2015年 wenlong. All rights reserved.
//

#include "qqwry.h"
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <unistd.h>
#include <errno.h>

#define NALI_QQWRY_PATH "/usr/local/share/QQWry.Dat"

FILE *wry_file = NULL;
regex_t regex;
iconv_t cd;

char *g_country = NULL;
char *g_area = NULL;
char *g_output = NULL;

void locationStringForIPString (const char *ipCstring, char *result) {
    memset(g_country, 0, BUFSIZ);
    memset(g_area, 0, BUFSIZ);
    memset(g_output, 0, BUFSIZ);
    
    qqwry_get_location(g_country,g_area,ipCstring,wry_file);
    
    if ( strlen(g_country) > 0 ) {
        result[0] = '[';
        size_t inbytesleft = strlen(g_country);
        char *outbuf = g_output;
        char *p = g_country;
        size_t bufsize = BUFSIZ;
        iconv(cd, &p, &inbytesleft, &outbuf, &bufsize);
        //iconv will make change to the parameters, always copy and use;
        strcat(result, g_output);
    }
    if ( strlen(g_area) > 0 ) {
        size_t inbytesleft = strlen(g_area);
        char *outbuf = g_output;
        char *p = g_area;
        size_t bufsize = BUFSIZ;
        iconv(cd, &p, &inbytesleft, &outbuf, &bufsize);
        strcat(result, " ");
        strcat(result, g_output);
    }
    if (strlen(result) == 0) {
        strcat(result, "未找到");
    }
    strcat(result, "]");
    return;
}

void processOneLine(const char *input, char *result) {
        /* "P" is a pointer into the string which points to the end of the
         previous match. */
        const char * p = input;
        /* "N_matches" is the maximum number of matches allowed. */
        const int n_matches = 10;
        /* "M" contains the matches found. */
        regmatch_t m[n_matches];
    
    size_t lastpos = 0;
        while (1) {
            int nomatch = regexec (&regex, p, n_matches, m, 0);
            if (nomatch) {
                break;
            } else {
                if (m[0].rm_so == -1) {
                    break;
                } else {
                    strlcat(result + strlen(result), p, m[0].rm_so + 1);
                    strlcat(result + strlen(result), p + m[0].rm_so, m[0].rm_eo - m[0].rm_so + 1);
                    char ipstring[BUFSIZ] = {0};
                    char location[BUFSIZ] = {0};
                    strncpy(ipstring, p + m[0].rm_so, m[0].rm_eo - m[0].rm_so + 1);
                    locationStringForIPString(ipstring, location);
                    strcat(result + strlen(result), location);
                }
                lastpos = m[0].rm_eo;
            }
            p += m[0].rm_eo;
        }
    strcat(result, input + lastpos);
}

int main(int argc, const char * argv[]) {
    if ((wry_file = fopen(NALI_QQWRY_PATH,"r")) == NULL ) {
        fprintf(stdout, "file: %s %s\n", NALI_QQWRY_PATH, "can not opened");
        return EXIT_FAILURE;
    }
    int reti = regcomp(&regex, "([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])\\.([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])", REG_EXTENDED);
    if (reti) {
        //wont happed here
        exit(0);
    }
    cd = iconv_open("utf-8","gb2312");
    if (cd==0) {
        return EXIT_FAILURE;
    }
    g_country = malloc(BUFSIZ);
    g_area = malloc(BUFSIZ);
    g_output = malloc(BUFSIZ);
    
    char inputString[BUFSIZ] = {0};
    char result[BUFSIZ] = {0};
    
    size_t pos = 0, i = 0;
    switch (argc) {
        case 1:
            while (fgets(inputString, BUFSIZ, stdin) != NULL) {
                memset(result, 0, BUFSIZ);
                processOneLine(inputString, result);
                write(STDOUT_FILENO, result, strlen(result));
            }
            break;
        case 2:
            processOneLine(inputString, result);
            printf("%s", result);
            break;
        default:
            printf("Usage:\n1)    # nali 8.8.8.8\n2)    # nali\n      > 8.8.8.8\n");
            break;
    }
    
    free(g_country);
    free(g_area);
    free(g_output);
    
    regfree(&regex);
    iconv_close(cd);
    fclose(wry_file);
    
    return EXIT_SUCCESS;
}
