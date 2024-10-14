
#include "requests.h"
#include "helpers.h"

char *compute_post_request(const char *host, const char *url, const char *type, string json, int json_fields_count, string cookies[50], int nr_cookies) {

  char *message = (char *)calloc(BUFLEN, sizeof(char));
  char *line = (char *)calloc(LINELEN, sizeof(char));
  int content_len = 0;

  content_len = json.length();

  sprintf(line, "POST %s HTTP/1.1", url);
  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  sprintf(line, "Content-Type: %s", type);
  compute_message(message, line);

  if (cookies != NULL) {
    for (int i = 0; i < nr_cookies; i++) {
      compute_message(message, cookies[i].c_str());
    }
  }

  sprintf(line, "Content-Length: %d", content_len);
  compute_message(message, line);

  compute_message(message, "");

  memset(line, 0, LINELEN);
  if (json_fields_count > 0) {
    compute_message(message, json.c_str());
  }

  free(line);
  return message;
}

char *compute_get_request(const char *host, const char *url, char *params, string cookies[50], int nr_cookies) {

  char *message = (char *)calloc(BUFLEN, sizeof(char));
  char *line = (char *)calloc(LINELEN, sizeof(char));

  if (params != NULL) {
    sprintf(line, "GET %s?%s HTTP/1.1", url, params);
  } else {
    sprintf(line, "GET %s HTTP/1.1", url);
  }

  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  if (cookies != NULL) {
    for (int i = 0; i < nr_cookies; i++) {
      compute_message(message, cookies[i].c_str());
    }
  }
  compute_message(message, "");
  return message;
}

char *compute_delete_request(const char *host, const char *url, char *params, string cookies[50], int nr_cookies) {

  char *message = (char *)calloc(BUFLEN, sizeof(char));
  char *line = (char *)calloc(LINELEN, sizeof(char));

  if (params != NULL) {
    sprintf(line, "DELETE %s?%s HTTP/1.1", url, params);
  } else {
    sprintf(line, "DELETE %s HTTP/1.1", url);
  }

  compute_message(message, line);

  sprintf(line, "Host: %s", host);
  compute_message(message, line);

  if (cookies != NULL) {
    for (int i = 0; i < nr_cookies; i++) {
      compute_message(message, cookies[i].c_str());
    }
  }
  compute_message(message, "");
  return message;
}