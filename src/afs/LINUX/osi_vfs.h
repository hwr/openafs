/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */

/*
 * Linux interpretations of vnode and vfs structs.
 */

#ifndef OSI_VFS_H_
#define OSI_VFS_H_

typedef struct inode vnode_t;
#define vnode inode

/* Map vnode fields to inode fields */
#define i_number	i_ino
#define v_count		i_count
#define v_op		i_op
#define v_fop           i_fop
#define v_type		i_mode
#define v_vfsp		i_sb
#define v_data		u.generic_ip

/* v_type bits map to mode bits */
#define VNON 0
#define VREG S_IFREG
#define VDIR S_IFDIR
#define VBLK S_IFBLK
#define VCHR S_IFCHR
#define VLNK S_IFLNK
#define VSOCK S_IFSOCK
#define VFIFO S_IFIFO

/* vcexcl - used only by afs_create */
enum vcexcl { EXCL, NONEXCL } ;

/* afs_open and afs_close needs to distinguish these cases */
#define FWRITE	O_WRONLY|O_RDWR|O_APPEND
#define FTRUNC	O_TRUNC

#define IO_APPEND O_APPEND
#define FSYNC O_SYNC

#define VTOI(V)	(V)

/* Various mode bits */
#define VWRITE	S_IWUSR
#define VREAD	S_IRUSR
#define VEXEC	S_IXUSR
#define VSUID	S_ISUID
#define VSGID	S_ISGID

#define vfs super_block

typedef struct vattr {
    int		va_type;	/* One of v_types above. */
    afs_size_t	va_size;
    unsigned long va_blocks;
    unsigned long va_blocksize;
    int		va_mask;	/* AT_xxx operation to perform. */
    umode_t	va_mode;	/* mode bits. */
    uid_t	va_uid;
    gid_t	va_gid;
    int		va_fsid;	/* Not used? */
    dev_t	va_rdev;
    ino_t	va_nodeid;	/* Inode number */
    nlink_t	va_nlink;	/* link count for file. */
    struct timeval va_atime;
    struct timeval va_mtime;
    struct timeval va_ctime;
} vattr_t;

#define VATTR_NULL(A) memset(A, 0, sizeof(struct vattr))

#ifndef HAVE_LINUX_I_SIZE_READ
#define i_size_read(X) ((X)->i_size)
#define i_size_write(X,Y) (X)->i_size = Y
#endif

#endif /* OSI_VFS_H_ */
