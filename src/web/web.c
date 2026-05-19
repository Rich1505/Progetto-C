// #include <stdio.h>
// #include <string.h>

// #include "civetweb.h"

// static int handler(struct mg_connection *conn, void *cbdata)
// {
//     const char *html =
//         "<h1>Hello from CivetWeb</h1>";

//     mg_printf(conn,
//               "HTTP/1.1 200 OK\r\n"
//               "Content-Type: text/html\r\n"
//               "Content-Length: %zu\r\n"
//               "\r\n"
//               "%s",
//               strlen(html),
//               html);

//     return 200;
// }

// int main(void)
// {
//     const char *options[] = {
//         "listening_ports", "8080",
//         NULL
//     };

//     struct mg_callbacks callbacks = {0};

//     struct mg_context *ctx =
//         mg_start(&callbacks, NULL, options);

//     if (!ctx) {
//         fprintf(stderr, "Errore avvio server\n");
//         return 1;
//     }

//     mg_set_request_handler(ctx, "/", handler, NULL);

//     printf("Server su http://localhost:8080\n");
//     getchar();

//     mg_stop(ctx);

//     return 0;
// }