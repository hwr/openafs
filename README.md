openafs
=======

clone of openafs master with all what is needed to have AFS/OSD
Over time this should converge with openafs master when my commits
to openafs gerrit are merged. Right now they are not and this 
repository contains what is planned to have later in openafs.

In order to build openafs with AFS/OSD support do the following;

clone this repository
cd openafs/src
git clone https://github/hwr/afsosd.git

This creates a subdirectory afsosd with some source code.

create a build-directory (outside the cloned repos)

run there the openafs/configure from the clone
and then
 
make 

You will get the full openafs tree compiled plus the contents of the
subdirectory openafs/src/afsosd. In this directory are some shared libraries 
which you best install under /lib64 or /lib (in case of a 32bit machine).
the dafileserver, davolserver and the afsd require then additional parameter
"-libafsosd" to switch AFS/OSD on. In order to really use it you need also
an instance of the osddbserver on your database servers and any number of
OSDs which run "rxosd" and should be registered in the osddb-database.

Good luck!

If you have questions feel free to contact me: 
hartmut.reuter@gmx.de
