#ifndef REQUESTS_H
#define REQUESTS_H

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <locale>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

char *compute_get_request(const char *host, const char *url, char *params,
                          string cookies[50], int nr_cookies);

char *compute_post_request(const char *host, const char *url,
                           const char *type, string json,
                           int json_fields_count, string cookies[50],
                           int nr_cookies);

char *compute_delete_request(const char *host, const char *url,
                             char *params, string cookies[50],
                             int nr_cookies);

#endif