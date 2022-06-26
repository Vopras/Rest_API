#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *token,
                            char *cookies)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "GET %s HTTP/1.1", url);

    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (cookies != NULL) {
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    }
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    
    compute_message(message, "");
    strcat(message, "\n");

    return message;
}
char *compute_delete_request(char *host, char *url, char *token,
                            char *cookies)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));


        
    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    if (cookies != NULL) {
        sprintf(line, "Cookie: %s", cookies);
        compute_message(message, line);
    }
    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }
    compute_message(message, "");
    strcat(message, "\n");

    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char *body_data,
                             char **cookies, int cookies_count, char *token)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);
    
    strcpy(body_data_buffer, "");
    strcat(body_data_buffer, body_data);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    sprintf(line, "Content-Length: %ld", strlen(body_data_buffer));
    compute_message(message, line);


    if (cookies != NULL) {
        int i = 0;
        while(cookies_count > i ) {
            sprintf(line, "Cookie: %s", cookies[i]);
            compute_message(message, line);
            i++;
        }

    }

    if (token != NULL) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }


    compute_message(message, "");

    memset(line, 0, LINELEN);

    compute_message(message, body_data_buffer);

    free(line);
    return message;
}
