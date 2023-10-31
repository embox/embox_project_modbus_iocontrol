/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    06.11.2014
 */

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#include <modbus.h>

static struct timespec diff_timespec(const struct timespec *time1,
    const struct timespec *time0) {
  assert(time1);
  assert(time0);
  struct timespec diff = {.tv_sec = time1->tv_sec - time0->tv_sec, //
      .tv_nsec = time1->tv_nsec - time0->tv_nsec};
  if (diff.tv_nsec < 0) {
    diff.tv_nsec += 1000000000; // nsec/sec
    diff.tv_sec--;
  }
  return diff;
}

static void print_timespec(struct timespec spec) {
	long            ms;
    time_t          s;

	s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6);
    if (ms > 999) {
        s++;
        ms = 0;
    }
	ms += (s * 1000);
    printf("%ld milliseconds\n", ms);
}

int main(int argc, char *argv[]) {
	const char *ip = "127.0.0.1";
	unsigned short port = 1502;
	modbus_t *ctx;
	char bit_value;
	unsigned int bit_n;
	bool input_err;
	int opt;
	struct timespec spec_start, spec_stop;
 

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


	input_err = true;
	if (argc - optind == 2) {
		bool op_set, op_clr;
		if ((op_set = (0 == strcmp(argv[optind], "set")))
				|| (op_clr = (0 == strcmp(argv[optind], "clr")))) {
			bit_value = op_set ? ON : OFF;
			bit_n = atoi(argv[optind + 1]);
			input_err = false;
		}
	}

	if (input_err) {
		fprintf(stderr, "invalid invocation\n"
			"Usage: %s [-a ADDR] [-p PORT] [set|clr] BIT_NUM\n", argv[0]);
		return -1;
	}

	ctx = modbus_new_tcp(ip, port);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return -1;
	}

	modbus_set_debug(ctx, TRUE);
	modbus_set_error_recovery(ctx,
			MODBUS_ERROR_RECOVERY_LINK |
			MODBUS_ERROR_RECOVERY_PROTOCOL);

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed: %s\n",
				modbus_strerror(errno));
		modbus_free(ctx);
		return -1;
	}

	//time_t time_begin = time_now();
	clock_gettime(CLOCK_REALTIME, &spec_start);
	if (1 == modbus_write_bit(ctx, bit_n, bit_value)) {
		printf("OK\n");
	} else {
		printf("FAILED\n");
	}
	//time_t time_end = time_now();
	clock_gettime(CLOCK_REALTIME, &spec_stop);
	print_timespec(diff_timespec(&spec_stop, &spec_start));
	
	/* Close the connection */
	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}
