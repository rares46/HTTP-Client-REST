HTTP, Client REST
Self-explatanory variables are used for cookies and JWT token, initialised with "N". Commands read from keyboard are being executed, showing to stdout relevant messages containing "SUCCES: [reason]" or "EROARE: [reason]".
Depending on the command (login and enter_library), cookie is verified, else JWT. For showing, json is being explicit parsed for screen displaing.
In general, for server communication, a socket is opened, the message is formated, then sent to the server, then a response is waited from the server. If the answer contains error, succes or eroare is displayed.
