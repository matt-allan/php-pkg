<?php

$filename = $argv[1] ?? false;

if (!$filename || !file_exists($filename)) {
    echo 'Usage: php package.php [filename]';
    exit(1);
}

$src = fopen(__DIR__ . '/build/embed', 'r');
$binary = fread($src, filesize(__DIR__ . '/build/embed'));
fclose($src);

$src = fopen($filename, 'r');
$code = fread($src, filesize($filename));
fclose($src);

$dest = fopen(__DIR__ . '/build/' . basename($filename, '.php'), 'w');
fwrite($dest, $binary);
fwrite($dest, '__PHP_PKG_SENTINEL__');
fwrite($dest, $code);
fclose($dest);

echo "Packaging complete.\n";
exit(0);
