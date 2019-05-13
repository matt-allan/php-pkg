<?php

$fh = fopen(__DIR__ . '/build/appended', 'r');
fseek($fh, -4, SEEK_END);
$size = unpack('V', fread($fh, 4))[1];
fseek($fh, -($size + 4), SEEK_END);
$code = fread($fh, $size);
eval(' /* loader */ ?>' . $code);
