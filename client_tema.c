#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"



int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char *host = "34.118.48.238";
    char text[100];
    char username[100], password[100];
    char cookie[200];
    char token[300];



    while (1) {
        fgets(text, 100, stdin);
        if (strcmp(text, "register\n") == 0) {
            
            printf("username=");
            fgets(username, 100, stdin);
            for (int i = 0; i < 200; i++) {
                if (username[i] == '\n') {
                    username[i] = '\0';
                }
            }
            printf("password=");
            fgets(password, 100, stdin);
            for (int i = 0; i < 200; i++) {
                if (password[i] == '\n') {
                    password[i] = '\0';
                }
            }



            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            char *credentials = NULL;
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            credentials = json_serialize_to_string_pretty(root_value);
            json_value_free(root_value);

            // printf("Stringul rezultat este: \n%s\n", credentials);
            sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);

            message = compute_post_request(host, "/api/v1/tema/auth/register", "application/json", credentials, NULL, 0, NULL);
            // message = compute_post_request(host, "/api/v1/tema/auth/register", );
            send_to_server(sockfd, message);
            // printf("message = %s", message);
            // printf("username = %s\npass = %s", username, password);
            response = receive_from_server(sockfd);
            for (int j = 0; j < strlen(response)+1; j++) {
                if (response[j] == '{') {
                    printf("\n%s\n\n", response + j);
                    break;
                }
                if (response[j] == '\0') {
                    printf("\n-----Register successful-----\n\n");
                    break;
                }
            }

            // printf("%s\n", response);


        }
        else 
            if (strcmp(text, "login\n") == 0) {
                
                printf("username=");
                fgets(username, 100, stdin);
                for (int i = 0; i < 200; i++) {
                    if (username[i] == '\n') {
                        username[i] = '\0';
                    }
                }
                printf("password=");
                fgets(password, 100, stdin);
                for (int i = 0; i < 200; i++) {
                    if (password[i] == '\n') {
                        password[i] = '\0';
                    }
                }
                printf("\n");

                JSON_Value *root_value = json_value_init_object();
                JSON_Object *root_object = json_value_get_object(root_value);
                char *credentials = NULL;
                json_object_set_string(root_object, "username", username);
                json_object_set_string(root_object, "password", password);
                credentials = json_serialize_to_string_pretty(root_value);
                json_value_free(root_value);

                sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_post_request(host, "/api/v1/tema/auth/login", "application/json", credentials, NULL, 0, NULL);
                send_to_server(sockfd, message);
                // printf("message = %s", message);
                // printf("username = %s\npass = %s", username, password);
                response = receive_from_server(sockfd);
                // printf("response from login:\n%s", response);

                for (int j = 0; j < strlen(response)+1; j++) {
                    if (response[j] == 'S' && response[j+1] == 'e' && 
                    response[j+2] == 't') {
                        int k = j+12;
                        int l = 0;
                        // while (response[k] != '\n') {
                        while (response[k] != ';') {
                            cookie[l] = response[k];
                            k++;
                            l++;
                        }
                        // cookie[k+1] = '\n';
                        cookie[l+1] = '\0';
                        // printf("Cookie = %s\n", cookie);                    
                    }

                    


                    if (response[j] == '{') {
                        printf("%s", response + j);
                        break;
                    }
                    if (response[j] == '\0') {
                        printf("\n-----Login successful!-----\n\n");
                        break;
                    }
                }
                // printf("%s", response);
                
            }
        else
            if (strcmp(text, "enter_library\n") == 0) {
                sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, "/api/v1/tema/library/access", NULL, cookie);
                // printf("-----------Cookie = %s----------\n", cookie);
                // printf("%s", message);
                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                // printf("Response from library:\n%s", response);

                int i = 0;
                while (response[i] != '{') {
                    i++;
                }
                i = i+10;
                // printf("%s\n", response);
                int j = 0;
                while (response[i] != '"') {
                    token[j] = response[i];
                    j++;
                    i++;
                }
                token[j] = '\0';
                // printf("Token = %s", token);
                // for (j = 0; j < strlen(token); j++) {
                //     printf("%d ", token[j]);
                // }
                // printf("%d", token[j+1]);


                if (response[9] == '2') {
                    printf("\n-----Library entered successfully!-----\n\n");
                }
                else 
                    if (token[0] == 'Y') {
                        printf("\n-----You are not logged in-----\n\n");
                    }
                    else
                        if (response[9] == '4' && response[11] == '1') {
                            printf("\n-----Unauthorized to enter the library-----\n\n");
                        }
                        else 
                            if (response[9] == '4' && response[11] == '0') {
                                printf("\n-----Bad request-----\n\n");
                            }

            }
        else 
            if (strcmp(text, "get_books\n") == 0) {
                sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, "/api/v1/tema/library/books", token, NULL);
                // printf("Message:\n%s", message);

                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                // printf("Response:\n%s\n", response);

                int i=0,j=0,k=0;
                char u[100];
                while (i < strlen(response)) {
                    if (response[i] == '[') {
                        // printf("i = %d\n",i);
                        j = i;
                            while (j < strlen(response)) {
                                u[k] = response[j];
                                k++;
                                j++;
                            }
                    }
                    i++;
                        // strcpy(u, response+i);
                }
                u[k] = '\0';

                if (response[9] == '5') {
                    printf("\n-----Error(no token)-----\n\n");
                }
                else
                    printf("\n-----The books currently in the librare are -----\n%s\n\n", u);
            }
        else
            if (strcmp(text, "get_book\n") == 0) {
                char id[100];
                printf("id=");
                // scanf("%s", &id);
                fgets(id, 100, stdin);
                // printf("id = %s", id);
                
                char str[100] = "/api/v1/tema/library/books/";
                // printf("%s", str);
                strcat(str, id);
                str[strlen(str)-1] = '\0';
                // printf("%s", rez);
                // printf("%s", str);

                sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, str, token, NULL);
                // printf("Message:\n%s", message);

                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                // printf("Response:\n%s\n", response);


                char u[300];
                int i = 0;
                int j = 0;
                while (i < strlen(response)) {
                    if (response[i] == '[') {
                        while (i< strlen(response)) {
                            u[j] = response[i];
                            j++;
                            i++;
                        }
                    }
                    i++;
                }
                // printf("%s", u);
                id[strlen(id)-1] = '\0';


                if (response[9] == '2') {
                    printf("\n-----Book with id = %s has following data-----\n", id);
                    for (i = 0; i < strlen(u); i++) {
                        if (u[i] == ',') 
                            printf("\n");
                        printf("%c", u[i]);
                    }
                    printf("\n\n");
                }
                else 
                    if (response[9] == '4') {
                        printf("\n-----Bood with id = %s has not been found-----\n\n", id);
                    }




            }

        else 
            if (strcmp(text, "add_book\n") == 0) {
                char title[100], author[100], genre[100], publisher[100];
                int page_count;
                
                
                printf("title=");
                fgets(title, 100, stdin);
                title[strlen(title)-1] = '\0';

                printf("author=");
                fgets(author, 100, stdin);
                author[strlen(author)-1] = '\0';

                printf("genre=");
                fgets(genre, 100, stdin);
                genre[strlen(genre)-1] = '\0';

                printf("publisher=");
                fgets(publisher, 100, stdin);
                publisher[strlen(publisher)-1] = '\0';

                printf("page_count=");
                scanf("%d", &page_count);


                JSON_Value *root_value = json_value_init_object();
                JSON_Object *root_object = json_value_get_object(root_value);
                char *book = NULL;

                json_object_set_string(root_object, "title", title);
                json_object_set_string(root_object, "author", author);
                json_object_set_string(root_object, "genre", genre);
                json_object_set_string(root_object, "publisher", publisher);
                json_object_set_number(root_object, "page_count", page_count);
                book = json_serialize_to_string_pretty(root_value);
                json_value_free(root_value);

                // printf("%s", book);




                sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_post_request(host, "/api/v1/tema/library/books", "application/json", book, NULL, 0, token);
                // printf("Message:\n%s", message);

                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                // printf("Response:\n%s\n", response);

                if (response[9] == '2') {
                    printf("\n-----Book successfully added-----\n\n");
                }
                else
                    printf("\n-----Book could not be added-----\n\n");


        }
        else
            if (strcmp(text, "delete_book\n") == 0) {
                char id[100];
                printf("id=");
                // scanf("%s", &id);
                fgets(id, 100, stdin);
                // printf("id = %s", id);
                
                char str[100] = "/api/v1/tema/library/books/";
                // printf("%s", str);
                strcat(str, id);
                str[strlen(str)-1] = '\0';
                // printf("%s", rez);
                // printf("%s", str);

                
            }

        else 
            if (strcmp(text, "logout\n") == 0) {
                sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);
                message = compute_get_request(host, "/api/v1/tema/auth/logout", NULL, cookie);
                // printf("Message:\n%s", message);

                send_to_server(sockfd, message);
                response = receive_from_server(sockfd);
                // printf("Response:\n%s\n", response);

                if (response[9] == '2') {
                    printf("\n-----Logged out successfully!-----\n\n");
                }
                else
                    printf("\n-----You are not logged in-----\n\n");


            }
        else 
            if (strcmp(text, "exit\n") == 0) {
                if (sockfd)
                    close(sockfd);
            break;
        }
    }



}