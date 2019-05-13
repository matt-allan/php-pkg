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

int main(int argc, char *argv[])
{
    PHP_EMBED_START_BLOCK(argc, argv)

    FILE * fp = fopen(argv[0], "r");

    if(!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    uint32_t size;

    if (fseek(fp, -sizeof size, SEEK_END) != 0) {
        perror("fseek failed");
        return EXIT_FAILURE;
    }

    if (fread(&size, sizeof size, 1, fp) != 1) {
        perror("fread failed");
        return EXIT_FAILURE;
    }

    if (is_big_endian() == true) {
        size = swap_endianness(size);
    }

    if (fseek(fp, -(size + sizeof size), SEEK_END) != 0) {
        perror("fseek failed");
        return EXIT_FAILURE;
    }

    char code[size];
    if (fread(code, size, 1, fp) != 1) {
        perror("fread failed");
        return EXIT_FAILURE;
    }

    if (fclose(fp) != 0) {
        perror("fclose failed");
        return EXIT_FAILURE;
    }

    if (zend_eval_string(code, NULL, argv[0]) == FAILURE) {
        perror("zend_eval_string failed");
        return EXIT_FAILURE;
    }

    PHP_EMBED_END_BLOCK()

    return 0;
}
