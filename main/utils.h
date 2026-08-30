#ifndef		__UTILS_H
#define		__UTILS_H

void err_halt(char*,...);
void hexdump(uint8_t *, size_t);
int extract_src_addr(char *, uint8_t *);

#endif
