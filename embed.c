#include <sapi/embed/php_embed.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_big_endian()
{
    int n = 1;

    return *(char *)&n != 1;
}

uint32_t swap_endianness(uint32_t value)
{
    uint32_t result = 0;
    result |= (value & 0x000000FF) << 24;
    result |= (value & 0x0000FF00) << 8;
    result |= (value & 0x00FF0000) >> 8;
    result |= (value & 0xFF000000) >> 24;
    return result;
}

static size_t embed_stream_reader(void *handle, char *buf, size_t len)
{
    return fread(buf, 1, len, (FILE*)handle);
}

static void embed_stream_closer(void *handle)
{
    if (handle && (FILE*)handle != stdin) {
        fclose((FILE*)handle);
    }
}

static size_t embed_stream_fsizer(void *handle)
{
    // TODO: actually implement this
    return 0;
}

int main(int argc, char *argv[])
{
    const char token[] = "__PHP_PKG_SENTINEL__";

    FILE * fp = fopen(argv[0], "r");

    if(!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // TODO: seek until we find the sentinel, then stop

    php_embed_module.php_ini_ignore = 1;

    PHP_EMBED_START_BLOCK(argc, argv)

    zend_file_handle file_handle;
    file_handle.type = ZEND_HANDLE_STREAM;
    file_handle.filename = argv[0];
    file_handle.opened_path = NULL;
    file_handle.free_filename = 0;
    file_handle.handle.stream.handle = fp;
    file_handle.handle.stream.reader  = embed_stream_reader;
    file_handle.handle.stream.closer  = embed_stream_closer;
    file_handle.handle.stream.fsizer  = embed_stream_fsizer;
    file_handle.handle.stream.isatty  = 0;

    if (php_execute_script(&file_handle) == FAILURE) {
        perror("Script execution failed");
        return EXIT_FAILURE;
    }

    PHP_EMBED_END_BLOCK()

    return 0;
}
