#include <sapi/embed/php_embed.h>

int main(int argc, char *argv[])
{
    PHP_EMBED_START_BLOCK(argc, argv)

    char *loader = "$fh = fopen(__FILE__, 'r'); \
                    fseek($fh, -4, SEEK_END); \
                    $size = unpack('V', fread($fh, 4))[1]; \
                    fseek($fh, -($size + 4), SEEK_END); \
                    $code = fread($fh, $size); \
                    eval(' /* loader */ ?>' . $code);";

    zend_eval_string(loader, NULL, argv[0]);

    PHP_EMBED_END_BLOCK()

    return 0;
}
