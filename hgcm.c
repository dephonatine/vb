#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "hgcm.h"

#define	THROW_ERROR(x) do { printf("error in function %s on line %d: %s\n", __func__, __LINE__, strerror(x)); exit(0); } while (0) 
#define	THROW_ERROR_CUSTOM(x) do { printf("%s on line %d\n", x, __LINE__); exit(0); } while (0)
#define	VBGL_IOCTL_CODE_SIZE(f, s) 0xC0005600 + (s << 16) + f

int fd;
int errno;

int vbox_ioctl(int func, void *data, uint16_t outsz) {
	if (!data)
		THROW_ERROR_CUSTOM("invalid data pointer");
	size_t datalen = malloc_usable_size(data);
	uint8_t *buf = malloc(24 + datalen);
	printf("datalen: %d\n", datalen);
	VBGLREQHDR req;
	req.cbIn = 24 + datalen;
	req.uVersion = 0x10001;
	req.uType = 0;
	req.rc = -225;
	req.cbOut = 24 + outsz;
	req.uReserved = 0;
	memcpy(buf, &req, sizeof(VBGLREQHDR));
	memcpy(buf, data, datalen);
	
	int rc = ioctl(fd, VBGL_IOCTL_CODE_SIZE(func, datalen), buf, 1);
	return rc;
}

void hgcm_connect(char *service) {
	VBGLIOCHGCMCONNECT *conn = malloc(sizeof(VBGLIOCHGCMCONNECT));
	strncpy(conn->svc, service, 127);
	conn->type = 2; 
	int rc = vbox_ioctl(IOCTL_HGCM_CONNECT, (void *)conn, 4);
	if (rc < 0)
		THROW_ERROR(errno);
}


void main(int argc, char *argv[]) {
	fd = open(VBOX_USER_DEV, O_RDWR);
	if (fd < 0) {
		THROW_ERROR(errno);
	}
	hgcm_connect("VBoxGuestPropSvc");
}


