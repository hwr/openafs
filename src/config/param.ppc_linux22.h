/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */

#ifndef AFS_PARAM_H
#define AFS_PARAM_H

#ifndef UKERNEL

/* This section for kernel libafs compiles only */
#define AFS_PPC_LINUX20_ENV	1
#define AFS_PPC_LINUX22_ENV	1

#else /* !defined(UKERNEL) */

/* This section for user space compiles only */
#define UKERNEL			1	/* user space kernel */

#endif /* !defined(UKERNEL) */

/* Machine / Operating system information */
#define SYS_NAME		"ppc_linux22"
#define SYS_NAME_ID		SYS_NAME_ID_ppc_linux22
#define AFS_SYSCALL		137
#define AFSBIG_ENDIAN		1

#endif /* AFS_PARAM_H */
