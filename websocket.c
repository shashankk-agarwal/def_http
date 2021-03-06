#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "civetweb.h"
#include "WebSockCallbacks.h"

int
main(void)
{
	struct httplib_context *ctx = 0;
	struct httplib_callbacks callback_funcs = {0};
	tWebSockContext ws_ctx;
	char inbuf[4];

	const char *server_options[] = {
	    /* document_root: The path to the test function websock.htm */
	    "document_root",
	    "../../examples/websocket",

	    /* port: use http standard to match websocket url in websock.htm:
	       ws://127.0.0.1/MyWebSock  */
	    /*       if the port is changed here, it needs to be changed in
	       websock.htm as well         */
	    "listening_ports",
	    "80",

	    NULL};

	callback_funcs.init_context = websock_init_lib;
	callback_funcs.exit_context = websock_exit_lib;

	ctx = httplib_start(&callback_funcs, &ws_ctx, server_options);

	httplib_set_websocket_handler(ctx,
	                         "/MyWebSock",
	                         NULL,
	                         websocket_ready_handler,
	                         websocket_data_handler,
	                         connection_close_handler,
	                         NULL);

	printf("Connect to localhost:%s/websock.htm\n",
	       httplib_get_option(ctx, "listening_ports"));

	puts("Enter an (ASCII) character or * to exit:");
	for (;;) {
		fgets(inbuf, sizeof(inbuf), stdin);

		if (inbuf[0] == '*') {
			break;
		}
		inbuf[0] = toupper(inbuf[0]);
		websock_send_broadcast(ctx, inbuf, 1);
	}

	httplib_stop(ctx);

	return 0;
}
