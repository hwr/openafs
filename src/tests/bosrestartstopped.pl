#!/usr/bin/env perl
use OpenAFS::CMU_copyright;
use OpenAFS::util qw(:DEFAULT %AFS_Help);
use OpenAFS::afsconf;
use OpenAFS::fs;
use OpenAFS::pts;
use OpenAFS::vos;
use OpenAFS::bos;

my ($host, %info, %info2, $linfo);
$host = `hostname`;
&AFS_Init();

&AFS_bos_restart(localhost,[sleeper],,,);
%info = &AFS_bos_status("localhost",[sleeper],);
$linfo=$info{'sleeper'};
%info2 = %$linfo;
if ($info2{'num_starts'} != 2) {
    exit 1;
}
if ($info2{'status'} ne "temporarily enabled, currently running normally.") {
    exit 1;
}
exit(0);



