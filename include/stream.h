#ifndef _STREAM_H_
#define _STREAM_H_

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct {
	char *ptr;
	char *base;
	char *end;
} FILE;

FILE *fmemopen(FILE *stream, void *buf, const char *mode);
int fmemprintf(FILE *stream, const char *fmt, ...);
int fseek(FILE *stream, long offset, int fromwhere);
int fclose(FILE *stream);

#endif /* _STREAM_H_ */
