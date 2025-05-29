#include <lib.h>

static char *msg1 = "Let the ruling classes tremble at a Communistic "
		    "revolution.\nThe proletarians have nothing to lose but "
		    "their chains.\nThey have a world to win.\n";

static char *msg2 = "Workers of the world, unite!   ";

static char msg2_encrypted[] = {0xee, 0x83, 0x4c, 0xd0, 0x60, 0xa4, 0x6e, 0x00, 0xc2, 0xc8, 0x64,
				0x6a, 0x0e, 0x9a, 0x28, 0x22, 0x92, 0x62, 0x61, 0x29, 0x8f, 0x2e,
				0xa3, 0x19, 0x49, 0xbf, 0x82, 0x91, 0x74, 0xbf, 0x60, 0xcc};

int main() {
	int r;
	int key_fd, msg_fd;
	char buf[512];

	memset(buf, 0, sizeof(buf));
	// Open key file
	if ((r = open("/key0.key", O_RDWR)) < 0) {
		user_panic("[EXAMPLE] cannot open /key0.key: %d\n", r);
	}
	key_fd = r;
	debugf("[EXAMPLE] open key0.key is good\n");

	// Set the key
	if ((r = fskey_set(key_fd)) < 0) {
		user_panic("[EXAMPLE] fskey_set() failed: %d\n", r);
	}
	debugf("[EXAMPLE] fskey_set() is good\n");

	// Close key file
	if ((r = close(key_fd)) < 0) {
		user_panic("[EXAMPLE] cannot close /key0.key: %d\n", r);
	}
	debugf("[EXAMPLE] close key0.key is good\n");

	// Check if the key is set
	if (fskey_isset() != 1) {
		user_panic("[EXAMPLE] fskey_isset() failed: %d\n", r);
	}
	debugf("[EXAMPLE] fskey_isset() is good\n");

	// Read (/msg)
	if ((r = open("/msg", O_RDONLY | O_ENCRYPT)) < 0) {
		user_panic("[EXAMPLE] cannot open /msg: %d\n", r);
	}
	msg_fd = r;
	if ((r = read(msg_fd, buf, 511)) < 0) {
		user_panic("[EXAMPLE] cannot read /msg: %d\n", r);
	}
	for (int i = 0; i < strlen(msg1) + 1; i++) {
		if (buf[i] != msg1[i]) {
			user_panic("[EXAMPLE] read /msg returned wrong data at %d: %02x != %02x", i,
				   (unsigned char)buf[i], (unsigned char)msg1[i]);
		} else {
			debugf("%c", buf[i]);
		}
	}
	if ((r = close(msg_fd)) < 0) {
		user_panic("[EXAMPLE] cannot close /msg: %d\n", r);
	}
	debugf("[EXAMPLE] read is good\n");

	// Write (/newmsg)
	if ((r = open("/newmsg", O_RDWR | O_ENCRYPT | O_CREAT)) < 0) {
		user_panic("[EXAMPLE] cannot create and open /newmsg: %d\n", r);
	}
	msg_fd = r;
	for (int i = 0; i < 4096; i += 32) {
		if ((r = write(msg_fd, msg2, strlen(msg2) + 1)) < 0) {
			user_panic("[EXAMPLE] cannot write /newmsg at %d: %d\n", i, r);
		}
	}
	if ((r = close(msg_fd)) < 0) {
		user_panic("[EXAMPLE] cannot close /newmsg: %d\n", r);
	}
	// Read (/newmsg)
	if ((r = open("/newmsg", O_RDONLY)) < 0) {
		user_panic("[EXAMPLE] cannot open /newmsg: %d\n", r);
	}
	msg_fd = r;
	if ((r = read(msg_fd, buf, 511)) < 0) {
		user_panic("[EXAMPLE] cannot read /newmsg: %d\n", r);
	}
	for (int i = 0; i < strlen(msg2) + 1; i++) {
		if (buf[i] != msg2_encrypted[i]) {
			user_panic("[EXAMPLE] read /newmsg returned wrong data at %d: %02x != %02x",
				   i, (unsigned char)buf[i], (unsigned char)msg2_encrypted[i]);
		}
	}
	for (int i = 0; i < strlen(msg2) + 1; i++) {
		debugf(" %c ", msg2[i]);
	}
	debugf("\n");
	for (int i = 0; i < strlen(msg2) + 1; i++) {
		debugf("%02x ", (unsigned char)buf[i]);
	}
	debugf("\n");
	if ((r = close(msg_fd)) < 0) {
		user_panic("[EXAMPLE] cannot close /newmsg: %d\n", r);
	}
	debugf("[EXAMPLE] write is good\n");

	if ((r = fskey_unset()) < 0) {
		user_panic("[EXAMPLE] fskey_unset() failed: %d\n", r);
	}
	if (fskey_isset() != 0) {
		user_panic("[EXAMPLE] fskey_isset() failed: %d\n", r);
	}
	debugf("[EXAMPLE] fskey_unset() is good\n");

	return 0;
}
