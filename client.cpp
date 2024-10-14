#include "helpers.h"
#include "requests.h"

int main() {

  int sockfd;
  const char *ip_string = "34.246.184.49";
  struct in_addr ip_addr;

  char *message;
  char *response;

  // convert the string to a struct in_addr
  if (inet_aton(ip_string, &ip_addr) == 0) {
      cout << "Invalid IP address: " << ip_string << endl;
  }

  // convert the struct in_addr back to a string
  char *ip = inet_ntoa(ip_addr);
  if (ip == NULL) {
    cout << "inet_ntoa" << endl;
  }

  string autentific = "N";
  string JWT = "N";    // initioalizare cu "N". Daca se modifica, e cookie sau JWT

  char comanda[14];
  // inceperea citirii stdin
  while (1) {
    fgets(comanda, 14, stdin);

    if (!strncmp(comanda, "register", 8)) {
      string username;
      string password;

      json regist = {
          {"username", username},
          {"password", password},
      };

      cout << "username=";
      cin >> username;
      regist["username"] = username;

      cout << "password=";
      cin >> password;
      regist["password"] = password;

      // deschid conexiunea
      sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

      // construire mesaj de trimitere
      message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/register",
        "application/json", regist.dump(), 1, NULL, 0);

      // trimitere mesaj socket
      send_to_server(sockfd, message);

      // primire raspuns
      response = receive_from_server(sockfd);

      // in functie de mesaj se afiseaza corespunzator
      char *final = strstr(response, "error");
      if (final == NULL) {
        cout << "SUCCES: Utilizator inregistrat!" << endl;
      } else {
        cout << "EROARE: " << username << " e deja folosit!" << endl;
      }

      close_connection(sockfd);

    } else if (!strncmp(comanda, "login", 5)) {

      string username;
      string password;

      json log = {
          {"username", username},
          {"password", password},
      };

      cout << "username=";
      cin >> username;
      log["username"] = username;

      cout << "password=";
      cin >> password;
      log["password"] = password;

      sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

      message = compute_post_request("34.246.184.49", "/api/v1/tema/auth/login",
      "application/json", log.dump(), 1, NULL, 0);

      send_to_server(sockfd, message);

      response = receive_from_server(sockfd);

      char *final = strstr(response, "error");
      if (final == NULL) {
        cout << "SUCCES: Te ai logat!" << endl;
        // ia cookie
        string str(response);
        int unde_incep = str.find("Cookie");
        int unde_termin = str.find("Path");
        autentific = str.substr(unde_incep, unde_termin - unde_incep - 2);
      } else {
        cout << "EROARE: Date de logare gresite!" << endl;
      }

      close_connection(sockfd);

    } else if (!strncmp(comanda, "enter_library", 13)) {

      // verificare cookie
      if (autentific == "N") {
        cout << "EROARE: Nu esti logat!" << endl;
      } else {
        string cookies[1];
        cookies[0] = autentific;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_get_request("34.246.184.49", "/api/v1/tema/library/access", NULL, cookies, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        string str(response);
        int unde_incepe = str.find("token");
        int unde_termin = str.find("}");

        // se construieste JWT
        JWT = "";
        JWT = JWT + "Authorization: Bearer " +
                    str.substr(unde_incepe + strlen("token:") + 2,
                               unde_termin - unde_incepe - strlen("token:") - 3);
        cout << "SUCCES: Acces biblioteca" << endl;
        
        close_connection(sockfd);
      }

    } else if (!strncmp(comanda, "get_books", 9)) {

      if (JWT == "N") {
        cout << "EROARE: Nu ai acces la biblioteca!" << endl;
      } else {
        string tokens[1];
        tokens[0] = JWT;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_get_request("34.246.184.49", "/api/v1/tema/library/books", NULL, tokens, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        string str(response);

        // verific daca exista carti adaugate prin parantezele de la json
        if (str.find("[{") != string::npos && str.find("}]") != string::npos) {
          int unde_incep = str.find("[{");
          int unde_termin = str.find("}]");
          string afis = str.substr(unde_incep, unde_termin - unde_incep + 2);

          json json = json::parse(afis);

          // afisare
          for (int i = 0; i < static_cast<int>(json.size()); ++i) {
            cout << "id: " << json[i]["id"] << "," << endl;
            cout << "title: " << json[i]["title"] << endl;
          }
        } else {
          cout << "EROARE: Nu sunt carti" << endl;
        }

        close_connection(sockfd);
      }

    } else if (!strncmp(comanda, "get_book", 8)) {

      if (JWT == "N") {
        cout << "EROARE: Nu ai acces la biblioteca!" << endl;
      } else {
        int id;
        cout << "id=";
        cin >> id;

        // construire str ruta pt trimitere la server
        char ruta[30] = "/api/v1/tema/library/books/";
        
        string id_str = std::to_string(id);

        strcat(ruta, id_str.c_str());

        string tokens[1];
        tokens[0] = JWT;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_get_request("34.246.184.49", ruta, NULL, tokens, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        char *final = strstr(response, "error");
        if (final == NULL) {
          std::string str(response);

          // verific daca exista carti adaugate   
          if (str.find("{") != string::npos && str.find("}") != string::npos) {
            int first = str.find("{");
            int last = str.find("}");
            string afis = str.substr(first, last - first + 2);
            json json = json::parse(afis);

            // afisare
            for (int i = 0; i < static_cast<int>(json.size()); ++i) {
              cout << "id: " << json["id"] << "," << endl;
              cout << "title: " << json["title"].get<std::string>() << endl;
              cout << "author: " << json["author"].get<std::string>() << endl;
              cout << "publisher: " << json["publisher"].get<std::string>() << endl;
              cout << "genre: " << json["genre"].get<std::string>() << endl;
              cout << "page_count: " << json["page_count"] << endl;
              break;
            }
          } else {
            cout << "EROARE: Nu sunt carti!" << endl;
          }
        } else {
          cout << "EROARE: Nu e cartea gen id gresit!" << endl;
        }

        close_connection(sockfd);
      }

    } else if (!strncmp(comanda, "add_book", 8)) {

      if (JWT == "N") {
        cout << "EROARE: Nu ai acces la biblioteca!" << endl;
      } else {
        string title;
        string author;
        string genre;
        string publisher;
        string page_count;

        cout << "title=";
        getline(cin, title);

        cout << "author=";
        getline(cin, author);

        cout << "genre=";
        getline(cin, genre);

        cout << "publisher=";
        getline(cin, publisher);

        cout << "page_count=";
        getline(cin, page_count);

        json json_message = {
            {"title", title},
            {"author", author},
            {"genre", genre},
            {"page_count", page_count},
            {"publisher", publisher},
        };

        string tokens[1];
        tokens[0] = JWT;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_post_request("34.246.184.49", "/api/v1/tema/library/books", "application/json",
            json_message.dump(), 1, tokens, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        char *final = strstr(response, "error");
        if (final == NULL) {
          cout << "SUCCES: carte adaugata!" << endl;
        } else {
          cout << "EROARE: Esti amuzant! Informatii incomplete sau gresit formatate!" << endl;
        }

        close_connection(sockfd);
      }

    } else if (!strncmp(comanda, "delete_book", 11)) {

      if (JWT == "N") {
        cout << "EROARE! Nu ai acces la biblioteca!" << endl;
      } else {
        int id;
        cout << "id=";
        cin >> id;

        char ruta[30] = "/api/v1/tema/library/books/";
        std::string id_str = std::to_string(id);
        strcat(ruta, id_str.c_str());

        string tokens[1];
        tokens[0] = JWT;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_delete_request("34.246.184.49", ruta, NULL, tokens, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        char *final = strstr(response, "error");
        if (final == NULL) {
          cout << "SUCCES: S a sters cartea!" << endl;
        } else {
          cout << "EROARE: Nu s a sters cartea!" << endl;
        }

        close_connection(sockfd);
      }

    } else if (!strncmp(comanda, "logout", 6)) {

      if (autentific == "N") {
        cout << "EROARE: Nu esti logat!" << endl;
      } else {
        string cookies[1];
        cookies[0] = autentific;

        sockfd = open_connection(ip, SERVER_PORT, PF_INET, SOCK_STREAM, 0);

        message = compute_get_request("34.246.184.49", "/api/v1/tema/auth/logout", NULL, cookies, 1);

        send_to_server(sockfd, message);

        response = receive_from_server(sockfd);

        char *final = strstr(response, "error");
        if (final == NULL) {
          cout << "SUCCES: A mers delogarea" << endl;
        } else {
          cout << "EROARE: Nu a mers delogarea!" << endl;
        }

        // resetare autentificator si JWT
        autentific = "N";
        JWT = "N";

        // opreste conexiune server
        close_connection(sockfd);
      }
    } else if (!strncmp(comanda, "exit", 4)) {
      // opreste client
      break;
    }
  }
}