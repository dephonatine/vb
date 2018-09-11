/* 	R3 HGCM IOCTLS */
#include <stdbool.h>
#define		IOCTL_HGCM_CONNECT		4
#define		IOCTL_HGCM_DISCONNECT	5
#define		IOCTL_HGCM_CALL32		6
#define		IOCTL_HGCM_CALL64		7

#define		VBOX_USER_DEV			"/dev/vboxuser"
#define		VBOX_GUEST_DEV			"/dev/vboxguest" /* Note: Can only be opened by root */


typedef struct VBGLREQHDR
{
	/* IN: The request input size, and output size if cbOut is zero.
	   @sa VMMDevRequestHeader::size */
	uint32_t        cbIn;
	/* IN: Structure version (VBGLREQHDR_VERSION)
	   @sa VMMDevRequestHeader::version */
	uint32_t        uVersion;
	/* IN: The VMMDev request type, set to VBGLREQHDR_TYPE_DEFAULT unless this is a
	   kind of VMMDev request. */
	uint32_t        uType;
	/* OUT: The VBox status code of the operation, out direction only. */
	int32_t         rc;
	/* IN: The output size.  This is optional - set to zero to use cbIn as the
	 *      * output size. */
	uint32_t        cbOut;
	/* Reserved, MBZ. */
	uint32_t        uReserved;
} VBGLREQHDR;

typedef struct VBGLIOCHGCMCONNECT
{
	char svc[128];
	uint32_t type;


} VBGLIOCHGCMCONNECT;

typedef struct VBGLIOCHGCMCALL
{
	/** Common header. */
//	VBGLREQHDR  Hdr;
	/** Input: The id of the caller. */
	uint32_t    u32ClientID;
	/** Input: Function number. */
	uint32_t    u32Function;
	/** Input: How long to wait (milliseconds) for completion before cancelling the
	 *         * call.  This is ignored if not a VBGL_IOCTL_HGCM_CALL_TIMED or
	 *                 * VBGL_IOCTL_HGCM_CALL_TIMED_32 request. */
	uint32_t    cMsTimeout;
	/** Input: Whether a timed call is interruptible (ring-0 only).  This is ignored
	 *         * if not a VBGL_IOCTL_HGCM_CALL_TIMED or VBGL_IOCTL_HGCM_CALL_TIMED_32
	 *                 * request, or if made from user land. */
	bool        fInterruptible;
	/** Explicit padding, MBZ. */
	uint8_t     bReserved;
	/** Input: How many parameters following this structure.
	 *         *
	 *                 * The parameters are either HGCMFunctionParameter64 or HGCMFunctionParameter32,
	 *                         * depending on whether we're receiving a 64-bit or 32-bit request.
	 *                                 *
	 *                                         * The current maximum is 61 parameters (given a 1KB max request size,
	 *                                                 * and a 64-bit parameter size of 16 bytes).
	 *                                                         *
	 *                                                                 * @note This information is duplicated by Hdr.cbIn, but it's currently too much
	 *                                                                         *       work to eliminate this. */
	uint16_t    cParms;
	/* Parameters follow in form HGCMFunctionParameter aParms[cParms] */
}; 


/*
   typedef struct VBGLIOCHGCMDISCONNECT
   {
   VBGLREQHDR Hdr;
   union
   {
   struct
   {
   uint32_t idClient;
   } In;
   } u;
   } VBGLIOCHGCMDISCONNECT;
   */
