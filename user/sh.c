#include <args.h>
#include <lib.h>

#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()"

/* Overview:
 *   Parse the next token from the string at s.
 *
 * Post-Condition:
 *   Set '*p1' to the beginning of the token and '*p2' to just past the token.
 *   Return:
 *     - 0 if the end of string is reached.
 *     - '<' for < (stdin redirection).
 *     - '>' for > (stdout redirection).
 *     - '|' for | (pipe).
 *     - 'w' for a word (command, argument, or file name).
 *
 *   The buffer is modified to turn the spaces after words into zero bytes ('\0'), so that the
 *   returned token is a null-terminated string.
 */
int _gettoken(char *s, char **p1, char **p2) {
	*p1 = 0;
	*p2 = 0;
	if (s == 0) {
		return 0;
	}

	while (strchr(WHITESPACE, *s)) {
		*s++ = 0;
	}
	if (*s == 0) {
		return 0;
	}

	if (strchr(SYMBOLS, *s)) {
		int t = *s;
		*p1 = s;
		*s++ = 0;
		*p2 = s;
		return t;
	}

	*p1 = s;
	while (*s && !strchr(WHITESPACE SYMBOLS, *s)) {
		s++;
	}
	*p2 = s;
	return 'w';
}

int gettoken(char *s, char **p1) {
	static int c, nc;
	static char *np1, *np2;

	if (s) {
		nc = _gettoken(s, &np1, &np2);
		return 0;
	}
	c = nc;
	*p1 = np1;
	nc = _gettoken(np2, &np1, &np2);
	return c;
}

#define MAXARGS 128

int parsecmdlist(char **cmds, char *s) {
	while (*s && strchr(WHITESPACE SYMBOLS, *s)) {
		s++;
	}
	if (*s == 0) {
		return 0;
	}
	int cnt = 1, flag = 1;
	cmds[0] = s;
	while (*s)
	{
		if (*s == ';') {
			*s = 0;
			s++;
			while (*s && strchr(WHITESPACE SYMBOLS, *s)) {
				s++;
			}
			if (*s == 0) {
				return cnt;
			} else {
				cmds[cnt++] = s;
			}
		} else {
			s++;
		}
	}
	return cnt;
}

int parsecmd(char **argv, int *rightpipe) {
	int argc = 0;
	while (1) {
		char *t;
		int fd, r;
		int c = gettoken(0, &t);
		switch (c) {
		case 0:
			return argc;
		case 'w':
			if (argc >= MAXARGS) {
				debugf("too many arguments\n");
				exit();
			}
			argv[argc++] = t;
			break;
		case '<':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: < not followed by word\n");
				exit();
			}
			// Open 't' for reading, dup it onto fd 0, and then close the original fd.
			// If the 'open' function encounters an error,
			// utilize 'debugf' to print relevant messages,
			// and subsequently terminate the process using 'exit'.
			/* Exercise 6.5: Your code here. (1/3) */
			fd = open(t, O_RDONLY);
			if (fd < 0) {
				debugf("failed to open '%s'\n", t);
				exit();
			}
			dup(fd, 0);
			close(fd);

			// user_panic("< redirection not implemented");

			break;
		case '>':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: > not followed by word\n");
				exit();
			}
			// Open 't' for writing, create it if not exist and trunc it if exist, dup
			// it onto fd 1, and then close the original fd.
			// If the 'open' function encounters an error,
			// utilize 'debugf' to print relevant messages,
			// and subsequently terminate the process using 'exit'.
			/* Exercise 6.5: Your code here. (2/3) */
			fd = open(t, O_WRONLY | O_CREAT | O_TRUNC);
			if (fd < 0) {
				debugf("failed to open '%s'\n", t);
				exit();
			}
			dup(fd, 1);
			close(fd);

			// user_panic("> redirection not implemented");

			break;
		case '|':;
			/*
			 * First, allocate a pipe.
			 * Then fork, set '*rightpipe' to the returned child envid or zero.
			 * The child runs the right side of the pipe:
			 * - dup the read end of the pipe onto 0
			 * - close the read end of the pipe
			 * - close the write end of the pipe
			 * - and 'return parsecmd(argv, rightpipe)' again, to parse the rest of the
			 *   command line.
			 * The parent runs the left side of the pipe:
			 * - dup the write end of the pipe onto 1
			 * - close the write end of the pipe
			 * - close the read end of the pipe
			 * - and 'return argc', to execute the left of the pipeline.
			 */
			int p[2];
			/* Exercise 6.5: Your code here. (3/3) */
			r = pipe(p);
			if (r != 0) {
				debugf("pipe: %d\n", r);
				exit();
			}
			r = fork();
			if (r < 0) {
				debugf("fork: %d\n", r);
				exit();
			}
			*rightpipe = r;
			if (r == 0) {
				dup(p[0], 0);
				close(p[0]);
				close(p[1]);
				return parsecmd(argv, rightpipe);
			} else {
				dup(p[1], 1);
				close(p[1]);
				close(p[0]);
				return argc;
			}

			// user_panic("| not implemented");

			break;
		// 支持 ;
		case ';':
			if((r = fork()) < 0) {
				debugf("fork: %d\n", r);
				exit();
			}
			*rightpipe = r;
			if(r == 0) return argc;
			else {
				wait(r);
				return parsecmd(argv, rightpipe);
			}
			break;
		}
	}

	return argc;
}

int command_cd(int argc, char *argv[]) {
	char *path = argv[0];
	if (argv[0] == 0) {
		syscall_chdir("/");
		return 0;
	}
	if (argc > 2) {
		printf("Too many args for cd command\n");
		return 1;
	}
	int fd;
	if ((fd = open(path, O_RDONLY)) < 0) {
		printf("cd: The directory '%s' does not exist\n", path);
		return 1;
	}
	struct Filefd *ffd = (struct Filefd *)num2fd(fd);
	if (ffd -> f_file.f_type != FTYPE_DIR) {
		printf("cd: '%s' is not a directory\n", path);
		return 1;
	}
	if(path[0] == '/') {
		syscall_chdir(path);
	} else {
		char buf[MAXPATHLEN], buf2[MAXPATHLEN];
		syscall_getcwd(buf);
		int len = strlen(buf);
		if(buf[len - 1] != '/') strcat(buf, "/");
		strcat(buf, path);
		solve_relative_path(buf, buf2);
		syscall_chdir(buf2);
	}
	return 0;
}

int command_pwd(int argc, char *argv[]) {
	if(argc > 1) {
		printf("pwd: expected 0 arguments; got %d\n", argc - 1);
		return 2;
	}
	char buf[MAXPATHLEN];
	syscall_getcwd(buf);
	printf("%s\n", buf);
	return 0;
}

void runcmd(char *s_all) {
	char *cmd_list[MAXARGS];
	int cmd_cnt = parsecmdlist(cmd_list, s_all), i;
	for(i = 0; i < cmd_cnt; i++) {
		int lst = (i == cmd_cnt - 1);
		char *s = cmd_list[i];
		gettoken(s, 0);

		char *argv[MAXARGS];
		int rightpipe = 0, r;
		int argc = parsecmd(argv, &rightpipe);
		if (argc == 0) {
			return;
		}
		argv[argc] = 0;

		if(strcmp(argv[0], "cd") == 0) {
			command_cd(argc, argv + 1);
			if(lst) exit();
		} else if(strcmp(argv[0], "pwd") == 0) {
			command_pwd(argc, argv + 1);
			if(lst) exit();
		} else if(strcmp(argv[0], "exit") == 0)
			exit();
		else {
			int child = spawn(argv[0], argv);
			if(lst) close_all();
			if (child >= 0) {
				wait(child);
			} else {
				debugf("spawn %s: %d\n", argv[0], child);
			}
			if (rightpipe) {
				wait(rightpipe);
			}
			if(lst) exit();
		}
	}
}

void readline(char *buf, u_int n) {
	buf[0] = '\0';
	int len = 0, pos = 0, r, mode = 0;
	char c;

	/*
		左箭头：ESC [ D（对应 ASCII 码：27 91 68）
		右箭头：ESC [ C（对应 ASCII 码：27 91 67）
			初始状态：esc_mode = 0
			ESC 键触发：当读取到 ESC（ASCII 27）时，进入第一阶段转义模式（esc_mode = 1）
			检测 [ 字符：在 esc_mode = 1 状态下，若下一个字符是 [（ASCII 91），则进入第二阶段转义模式（esc_mode = 2）
			识别方向键：在 esc_mode = 2 状态下，根据后续字符判断具体方向：
			D（ASCII 68）表示左箭头
			C（ASCII 67）表示右箭头
	*/
	while(len < n - 1) {
		if((r = read(0, &c, 1)) != 1) {
			if(r < 0) debugf("read error: %d\n", r);
			exit();
		}

		if(mode == 2) {
			if(c == 'D') {
				if(pos > 0) pos -= 1;
				else printf("\033[C");
			} else if(c == 'C') {
				if(pos , len) pos += 1;
				else printf("\033[D");
			}
			mode = 0;
			continue;
		} else if(mode == 1) {
			mode = ((c == '[') ? 2 : 0);
			continue;
		}

		if(c == 27) mode = 1;
		else if(c == '\r' || c == '\n') {
			buf[len] = '\0';
			return;
		} else if(c == 0x7f || c == '\b') {
			int i;
			if(pos > 0) {
				// 移动
				for(i = pos - 1; i < len - 1; i++)
					buf[i] = buf[i + 1];
				len -= 1;
				pos -= 1;
				buf[len] = '\0';

				printf("\b");
				if(pos < len) {
					write(1, buf + pos, len - pos);
					write(1, " ", 1);
					for(i = len; i >= pos; i--) 
						write(1, "\b", 1);
				} else printf(" \b");
			}
		} else if(c <= 126 && c >= 32) {
			int i;
			if(pos < len) 
				for(i = len; i > pos; i -= 1) 
					buf[i] = buf[i - 1];
			
			buf[pos] = c;
			len += 1, pos += 1;
			buf[len] = '\0';

			if(pos != len) {
				write(1, buf + pos, len - pos);
				for(i = pos; i < len; i += 1) 
					write(1, "\033[D", 3);
			}
		}
	}
	// int r;
	// for (int i = 0; i < n; i++) {
	// 	if ((r = read(0, buf + i, 1)) != 1) {
	// 		if (r < 0) {
	// 			debugf("read error: %d\n", r);
	// 		}
	// 		exit();
	// 	}
	// 	if (buf[i] == '\b' || buf[i] == 0x7f) {
	// 		if (i > 0) {
	// 			i -= 2;
	// 		} else {
	// 			i = -1;
	// 		}
	// 		if (buf[i] != '\b') {
	// 			printf("\b");
	// 		}
	// 	}
	// 	if (buf[i] == '\r' || buf[i] == '\n') {
	// 		buf[i] = 0;
	// 		return;
	// 	}
	// }
	debugf("line too long\n");
	while ((r = read(0, buf, 1)) == 1 && buf[0] != '\r' && buf[0] != '\n') {
		;
	}
	buf[0] = 0;
}

char buf[1024];

void usage(void) {
	printf("usage: sh [-ix] [script-file]\n");
	exit();
}

int main(int argc, char **argv) {
	int r;
	int interactive = iscons(0);
	int echocmds = 0;
	printf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	printf("::                                                         ::\n");
	printf("::                     MOS Shell 2024                      ::\n");
	printf("::                                                         ::\n");
	printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	ARGBEGIN {
	case 'i':
		interactive = 1;
		break;
	case 'x':
		echocmds = 1;
		break;
	default:
		usage();
	}
	ARGEND

	if (argc > 1) {
		usage();
	}
	if (argc == 1) {
		close(0);
		if ((r = open(argv[0], O_RDONLY)) < 0) {
			user_panic("open %s: %d", argv[0], r);
		}
		user_assert(r == 0);
	}
	for (;;) {
		if (interactive) {
			printf("\n$ ");
		}
		readline(buf, sizeof buf);

		if (buf[0] == '#') {
			continue;
		}
		// 注释
		int i;
		for(i = 0; buf[i]; i += 1) {
			if(buf[i] == '#') {
				buf[i] = '\0';
				break;
			}
		}
		if (echocmds) {
			printf("# %s\n", buf);
		}
		
		if(strncmp(buf, "exit ", 5) == 0 || strcmp(buf, "exit") == 0) {
			runcmd(buf);
		} else {
			if ((r = fork()) < 0) {
				user_panic("fork: %d", r);
			}
			if (r == 0) {
				runcmd(buf);
				exit();
			} else {
				wait(r);
			}
		}
	}
	return 0;
}
