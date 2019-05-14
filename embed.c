#include <sapi/embed/php_embed.h>
#include <stdio.h>
#include <stdlib.h>

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
    FILE * fp = fopen(argv[0], "r");

    if(!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    const char token[] = "__PHP_PKG__";
    int start_pos = 0;
    int token_pos = 0;
    char last_bytes[12];

    int c;
    while ((c = fgetc(fp)) != EOF) {
        start_pos++;
        if (c == token[token_pos]) {
            token_pos++;
            if (token_pos > 11) {
                // char buf[120];
                // fseek(fp, start_pos - 12, SEEK_SET);
                // fread(buf, 1, 120, fp);
                // printf("POS: [%d]", start_pos);
                // printf("BUF: [%s]", buf);
                break;
            }
        } else {
            token_pos = 0;
        }
    }

    if (start_pos == 0) {
        perror("Corrupt pkg");
        return EXIT_FAILURE;
    }

    fseek(fp, start_pos, SEEK_SET);
    char buf[512];
    fread(buf, 1, 512, fp);
    printf("POS: [%d]", start_pos);
    printf("BUF: [%s]", buf);

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
