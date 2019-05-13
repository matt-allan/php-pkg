<?php

$src = fopen(__DIR__ . '/build/embed', 'r');
$binary = fread($src, filesize(__DIR__ . '/build/embed'));
fclose($src);

$src = fopen(__DIR__ . '/code.php', 'r');
$code = fread($src, $codeLength = filesize(__DIR__ . '/code.php'));
fclose($src);

$dest = fopen(__DIR__ . '/build/greeter', 'w');
fwrite($dest, $binary);
fwrite($dest, $code);
fwrite($dest, pack('V', $codeLength));
fclose($dest);