<?php

function listProcsses(){
	$dirs = scandir('/proc');
	printf("%-10s %-10s\n", 'PID', 'COMMAND');
	$i = 0;
	foreach($dirs as $dir){
		if(!is_numeric($dir))
			continue;
		if($i > 10){
			echo "...\n";
			return;
		}

		$fp = '/proc/' . $dir;

		if(!file_exists($fp . '/cmdline'))
			continue;

		$cmd = explode("\0", file_get_contents($fp . '/cmdline'));
		if(empty($cmd[0]))
			continue;

		$i++;

		printf("%-10d %-10s\n", $dir, substr($cmd[0], 0 , 50) . (strlen($cmd[0]) > 50 ? '...' : ''));
	}
}

// Allocate a bunch of memory
//$a = str_repeat('A', 1024*1024*100);


echo "==== Hello from php ====\n";
echo "PID: ", getmypid(), "\n";
echo "Hostname: ", php_uname('n'), "\n";
echo "User: ", posix_geteuid(), "\n";
echo "===== Processlist ======\n";
listProcsses();

