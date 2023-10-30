/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    06.11.2014
 */

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus.h>

#include <libleddrv.h>

static modbus_mapping_t *mb_mapping_wrapper_new(void) {
	modbus_mapping_t *mb_mapping;
	mb_mapping = modbus_mapping_new(LEDDRV_LED_N, 0, 0, 0);

	return mb_mapping;
}

static void mb_mapping_wrapper_free(modbus_mapping_t *mb_mapping) {

	modbus_mapping_free(mb_mapping);
}

static void mb_mapping_getstates(modbus_mapping_t *mb_mapping) {
	int i;

	leddrv_getstates(mb_mapping->tab_bits);

	for (i = 0; i < mb_mapping->nb_bits; i++) {
		mb_mapping->tab_bits[i] = mb_mapping->tab_bits[i] ? ON : OFF;
	}
}

int main(int argc, char*argv[]) {
	int listen_socket, client_socket;
	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;
	uint8_t *query;
	const char *ip = "0.0.0.0";
	unsigned short port = 1502;
	int header_len;
	int opt;

	leddrv_init();

	while (-1 != (opt = getopt(argc, argv, "a:p:"))) {
		switch (opt) {
		case 'a':
			ip = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		}
	}
	
	ctx = modbus_new_tcp(ip, port);
	header_len = modbus_get_header_length(ctx);
	query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);

	modbus_set_debug(ctx, FALSE);

	mb_mapping = mb_mapping_wrapper_new();
	if (mb_mapping == NULL) {
		fprintf(stderr, "Failed to allocate the mapping: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	listen_socket = modbus_tcp_listen(ctx, 1);
	for (;;) {
		client_socket = modbus_tcp_accept(ctx, &listen_socket);
		if (-1 == client_socket) {
			break;
		}

		for (;;) {
			int query_len;

			query_len = modbus_receive(ctx, query);
			if (-1 == query_len) {
				/* Connection closed by the client or error */
				break;
			}

			if (query[header_len - 1] != MODBUS_TCP_SLAVE) {
				continue;
			}

			mb_mapping_getstates(mb_mapping);

			if (-1 == modbus_reply(ctx, query, query_len, mb_mapping)) {
				break;
			}

			leddrv_updatestates(mb_mapping->tab_bits);
		}

		close(client_socket);
	}
	printf("exiting: %s\n", modbus_strerror(errno));

	close(listen_socket);
	mb_mapping_wrapper_free(mb_mapping);
	free(query);
	modbus_free(ctx);

	return 0;
}
