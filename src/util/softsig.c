/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 *
 * Portions Copyright (c) 2003 Apple Computer, Inc.
 */

#include <afsconfig.h>
#include <afs/param.h>
#include <afs/procmgmt.h> /* Must be before roken.h */

#include <roken.h>
#include <afs/opr.h>

#include <pthread.h>

#include "pthread_nosigs.h"

/*------------------------------------------------------------------------
 * Under Darwin 6.x (including 7.0), sigwait() is broken, so we use
 * sigsuspend() instead.  We also don't block signals we don't know
 * about, so they should kill us, rather than us returning zero status.
 *------------------------------------------------------------------------*/

static pthread_t softsig_tid;
static struct {
    void (*handler) (int);
    int pending;
#if !(defined(AFS_DARWIN_ENV) || (defined(AFS_NBSD_ENV) && !defined(AFS_NBSD50_ENV)))
    int fatal;
#endif /* !defined(AFS_DARWIN_ENV) || !defined(AFS_NBSD_ENV) */
    int inited;
} softsig_sigs[NSIG];

static void *
softsig_thread(void *arg)
{
    sigset_t ss, os;
    int i;

    sigemptyset(&ss);
    /* get the list of signals _not_ blocked by AFS_SIGSET_CLEAR() */
    pthread_sigmask(SIG_BLOCK, &ss, &os);
    pthread_sigmask(SIG_SETMASK, &os, NULL);
    sigaddset(&ss, SIGUSR1);
#if defined(AFS_DARWIN_ENV) || (defined(AFS_NBSD_ENV) && !defined(AFS_NBSD50_ENV))
    pthread_sigmask (SIG_BLOCK, &ss, NULL);
    sigdelset (&os, SIGUSR1);
#elif !defined(AFS_HPUX_ENV)
    /* On HPUX, don't wait for 'critical' signals, as things such as
     * SEGV won't cause a core, then. Some non-HPUX platforms may need
     * this, though, since apparently if we wait on some signals but not
     * e.g. SEGV, the softsig thread will still wait around when the
     * other threads were killed by the SEGV. */
    for (i = 0; i < NSIG; i++) {
	if (!sigismember(&os, i) && i != SIGSTOP && i != SIGKILL) {
	    sigaddset(&ss, i);
	    softsig_sigs[i].fatal = 1;
	}
    }
#endif /* defined(AFS_DARWIN_ENV) || defined(AFS_NBSD_ENV) */

    while (1) {
	void (*h) (int);
#if !defined(AFS_DARWIN_ENV) && (!defined(AFS_NBSD_ENV) || defined(AFS_NBSD50_ENV))
	int sigw;
#endif

	h = NULL;

	for (i = 0; i < NSIG; i++) {
	    if (softsig_sigs[i].handler && !softsig_sigs[i].inited) {
		sigaddset(&ss, i);
#if defined(AFS_DARWIN_ENV) || (defined(AFS_NBSD_ENV) && !defined(AFS_NBSD50_ENV))
		pthread_sigmask (SIG_BLOCK, &ss, NULL);
		sigdelset (&os, i);
#endif /* defined(AFS_DARWIN_ENV) || defined(AFS_NBSD_ENV) */
		softsig_sigs[i].inited = 1;
	    }
	    if (softsig_sigs[i].pending) {
		softsig_sigs[i].pending = 0;
		h = softsig_sigs[i].handler;
		break;
	    }
	}
	if (i == NSIG) {
#if defined(AFS_DARWIN_ENV) || (defined(AFS_NBSD_ENV) && !defined(AFS_NBSD50_ENV))
	    sigsuspend (&os);
#else /* !defined(AFS_DARWIN_ENV) && !defined(AFS_NBSD_ENV) */
	    sigwait(&ss, &sigw);
	    if (sigw != SIGUSR1) {
		if (softsig_sigs[sigw].fatal)
		    exit(0);
		softsig_sigs[sigw].pending = 1;
	    }
#endif /* defined(AFS_DARWIN_ENV) || defined(AFS_NBSD_ENV) */
	} else if (h)
	    h(i);
    }
    return NULL;
}

static void
softsig_usr1(int signo)
{
    signal (SIGUSR1, softsig_usr1);
}

void
softsig_init(void)
{
    int rc;
    AFS_SIGSET_DECL;
    AFS_SIGSET_CLEAR();
    rc = pthread_create(&softsig_tid, NULL, &softsig_thread, NULL);
    opr_Assert(0 == rc);
    AFS_SIGSET_RESTORE();
    signal (SIGUSR1, softsig_usr1);
}

static void
softsig_handler(int signo)
{
    signal(signo, softsig_handler);
    softsig_sigs[signo].pending = 1;
    pthread_kill(softsig_tid, SIGUSR1);
}

void
softsig_signal(int signo, void (*handler) (int))
{
    softsig_sigs[signo].handler = handler;
    softsig_sigs[signo].inited = 0;
    signal(signo, softsig_handler);
    pthread_kill(softsig_tid, SIGUSR1);
}

#if defined(TEST)
static void
print_foo(int signo)
{
    printf("foo, signo = %d, tid = %d\n", signo, pthread_self());
}

int
main(int argc, char **argv)
{
    softsig_init();
    softsig_signal(SIGINT, print_foo);
    printf("main is tid %d\n", pthread_self());
    while (1)
	sleep(60);
}
#endif
