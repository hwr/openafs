#!/usr/bin/env perl
use OpenAFS::CMU_copyright;
use OpenAFS::util qw(:DEFAULT %AFS_Help);
use OpenAFS::afsconf;
use OpenAFS::fs;
use OpenAFS::pts;
use OpenAFS::vos;
use OpenAFS::bos;

my ($host, @hosts, $this, $cell, $cnt);
$host = `hostname`;
chomp $host;
&AFS_Init();
$cell = &AFS_fs_wscell();

&AFS_bos_removehost(localhost,"128.2.1.2",);
@hosts = &AFS_bos_listhosts(localhost,);
$this = shift(@hosts);
if ($this ne $cell) {
    exit (1);
}
while ($this = shift(@hosts)) {
    if ($this ne $host) {
	exit (1);
    }
}
exit(0);



