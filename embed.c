#include <sapi/embed/php_embed.h>

int main(int argc, char *argv[])
{
    PHP_EMBED_START_BLOCK(argc, argv)
    zend_eval_string("echo 'hello world!';", NULL,
        "Simple Hello World App");
    PHP_EMBED_END_BLOCK()

    return 0;
}
