#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "lwip/sockets.h"

#define HEXDUMP_LINELEN		16
#define SRC_ADDR_OFFSET		12

static void hexdump_line(uint8_t *, size_t);

void 
err_halt(char *fmt,...)
{
	va_list		ap;

	va_start(ap, fmt);

	while (1) {
		vfprintf(stderr, fmt, ap);
		fprintf(stderr, "\n");
	}
}

void 
hexdump(uint8_t *buf, size_t len)
{
	size_t		linelen;

	if (buf == NULL)
		return;

	for (size_t i = 0; i < len; i += 16) {
		/* if remaining bytes is less than 8, use that instead */
		linelen = ((len - i) < 16) ? (len - i) : 16;		
		hexdump_line(buf, linelen);
		buf += linelen;
	}
}

void 
hexdump_line(uint8_t *line, size_t linelen)
{
	uint8_t		padding = 0;

	padding = HEXDUMP_LINELEN - linelen;

	for (int i = 0; i < linelen; i++)
		printf("%.2x ", line[i]);

	for (int i = 0; i < padding; i++)
		printf("   ");
	putchar('\t');

	putchar('|');
	for (int i = 0; i < linelen; i++) {
		if (isprint(line[i]))
			putchar(line[i]);
		else
			putchar('.');
	}

	printf("|\n");
}

int
extract_src_addr(char *addrbuf, uint8_t *packet)
{
	packet += SRC_ADDR_OFFSET;

	if (inet_ntop(AF_INET, packet, addrbuf, INET_ADDRSTRLEN) == NULL)
		return -1;

	return 0;
}

