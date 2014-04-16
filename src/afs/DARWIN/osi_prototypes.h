/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */
/*
 * osi_prototypes.h
 *
 * Exported macos support routines.
 */
#ifndef _OSI_PROTO_H_
#define _OSI_PROTO_H_

/* osi_file.c */
extern afs_rwlock_t afs_xosi;

/* osi_misc.c */
extern void darwin_notify_perms(struct unixuser *auser, int event);
extern int osi_lookupname(char *aname, enum uio_seg seg, int followlink,
			  struct vnode **vpp);
extern int osi_lookupname_user(user_addr_t aname, enum uio_seg seg,
			       int followlink, struct vnode **vpp);
extern int afs_suser(void *credp);
extern void get_vfs_context(void);
extern void put_vfs_context(void);

/* osi_sleep.c */
extern void afs_osi_fullSigMask(void);
extern void afs_osi_fullSigRestore(void);

/* osi_vm.c */
extern void osi_VM_NukePages(struct vnode *vp, off_t offset, off_t size);
extern int osi_VM_Setup(struct vcache *avc, int force);

/* osi_vnodeops.c */
extern int afs_darwin_getnewvnode(struct vcache *avc);
extern int afs_darwin_finalizevnode(struct vcache *avc, struct vnode *parent,
				    struct componentname *cnp, int isroot,
				    int locked);
#endif /* _OSI_PROTO_H_ */
