#include <lib.h>

struct Find_res res __attribute__((aligned(PAGE_SIZE)));
int main() {
	find("/", "motd", &res);
	for (int i = 0; i < res.count; i++) {
		debugf("%s\n", res.file_path[i]);
	}
	return 0;
}
