#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <getopt.h>


typedef uint32_t word;

const uint32_t r[] = {
        7, 12, 17, 22, 7, 12, 17, 22,
        7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20,
        5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23,
        4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21,
        6, 10, 15, 21, 6, 10, 15, 21
};

const uint32_t k[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf,
        0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122,
        0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d,
        0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905,
        0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44,
        0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039,
        0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3,
        0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82,
        0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


void InspectChunk(const word *chunk) {
    // Prints a Message Chunk in a human readable format
    //
    // Each Message Chunk contains 16 Words; A Word in this context refers to four bytes(8-bit quantity)


    word foo[16];
    for (int i = 0; i < 16; ++i) {
        foo[i] = chunk[i] << 24u | 0xFF0000u & chunk[i] << 8u | 0xFF00u & chunk[i] >> 8u | 0xFFu & chunk[i] >> 24u;
    }

    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║  i       DEC         HEX         BINARY(low-order word first)    ║\n");
    printf("╠════╤════════════╤══════════╤═════════════════════════════════════╣\n");

    char str[33];
    str[32] = '\0';

    for (int i = 0; i < 16; i++) {
        printf("║ %2d │ %10u │ %8X │ ", i, foo[i], foo[i]);

        uint32_t n = foo[i];

        int j;
        for (j = 0; n > 0; j++) {
            str[j] = (char) (n % 2);
            n = n / 2;
        }

        for (int l = 31; l >= 0; l--) {
            printf("%d", str[l]);

            if (l % 8 == 0) printf(" ");
        }
        memset(&str, 0, 32);

        printf("║\n");
    }
    printf("╚════╧════════════╧══════════╧═════════════════════════════════════╝\n");
}

void process(word chunk[16], uint32_t md_buf[4]) {
#ifdef DEBUG
    // If compiled with a debug flag show the individual chunks as they get processed, useful for debugging
    InspectChunk(chunk);
#endif

    uint32_t a = md_buf[0], b = md_buf[1], c = md_buf[2], d = md_buf[3];

    // Applies all 4 founds of 16 operations
    for (uint32_t i = 0; i < 64; i++) {

        uint32_t f, g;

        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | (~d & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | ~d);
            g = 7 * i % 16;
        }

        f += a + k[i] + (uint32_t) chunk[g];
        a = d;
        d = c;
        c = b;
        b += f << r[i] | f >> (32 - r[i]);

    }

    md_buf[0] += a;
    md_buf[1] += b;
    md_buf[2] += c;
    md_buf[3] += d;

}

const char *generate_digest(const uint32_t md_buf[]) {
    // Generates a digest of a message buffer and returns a pointer to a string containing said digest in hexadecimal
    char *digest = malloc(33 * sizeof(char));

    //
#define hex(n) (uint8_t)(md_buf[i]  >> (n))
    for (int i = 0; i < 4; i++) {
        sprintf(digest + 8 * i, "%2.2x%2.2x%2.2x%2.2x", hex(0u), hex(8u), hex(16u), hex(24u));
    }

    return digest;
}


char *hash_string(char *str) {
    // Given a pointer to a string break the string into chunks, apply the md5 algorithm and return a message digest

    uint64_t message_size = 0;
    uint8_t offset = 0;

    uint32_t md_buf[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

    word chunk[16];

    size_t read_size;
    // The read one 64byte chunk at a time
    do {
        read_size = (strlen(str) - offset >= sizeof(chunk) ? sizeof(chunk) : strlen(str) % sizeof(chunk));

        memcpy(chunk, str + offset, read_size);
        offset += read_size;

        // Chunks are only processed if they are full
        if (read_size == sizeof(chunk)) {
            process(chunk, md_buf);
        }

        message_size += 8 * read_size;

    } while (read_size == sizeof(chunk));

    // At this stage the chunk left should be less than 64 bytes long
    // However there may be remaining data stored in the data structure from previous cycles of processing

    // All words that were not written to in the last iteration are set to zero
    // Note: Appending the message length will already overwrite the last two words therefore we don't need to zero them here
    for (int i = (int) (read_size / 4) + 1; i < 14; ++i) {
        chunk[i] = 0;
    }

    // The last word written to
    uint64_t final_index = read_size / 4u;
    // Each word contains 4 bytes; final byte indicated what last written to.
    uint64_t final_byte = read_size % 4u;

    chunk[final_index] = final_byte == 0 ? 0 : (chunk[final_index] << 8 * (4 - final_byte)) >> 8 * (4 - final_byte);

    // Append sentential(a single "1" bit)
    chunk[final_index] = (chunk[final_index] << 8 * (3 - final_byte) | 0x80000000) >> 8 * (3 - final_byte);

    // If there is no room to append length, process it in the next chunk
    if (final_index >= 14) {
        process(chunk, md_buf);
        for (int i = 0; i < 16; ++i) {
            chunk[i] = 0;
        }
    }

    // Message size is appended to the final message block in the form of two words
    chunk[14] = (word) message_size;
    chunk[15] = (word) (message_size << 32u);
    process(chunk, md_buf);

    return (char *) generate_digest(md_buf);
}

void test() {
    // Tests to be run
    char testcases[7][100][100] = {
            {"",                                                                                 "d41d8cd98f00b204e9800998ecf8427e"},
            {"a",                                                                                "0cc175b9c0f1b6a831c399e269772661"},
            {"abc",                                                                              "900150983cd24fb0d6963f7d28e17f72"},
            {"message digest",                                                                   "f96b697d7cb7938d525a2f31aaf161d0"},
            {"abcdefghijklmnopqrstuvwxyz",                                                       "c3fcd3d76192e4007dfb496cca67e13b"},
            {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",                   "d174ab98d277d9f5a5611c2c9f419d9f"},
            {"12345678901234567890123456789012345678901234567890123456789012345678901234567890", "57edf4a22be3c955ac49da2e2107b67a"},
    };

    int total_testcases = sizeof(testcases) / sizeof(testcases[0]);
    int passed_counter = 0;
    char *actual_hash;

    for (int i = 0; i < total_testcases; ++i) {
        actual_hash = hash_string(testcases[i][0]);
        printf("Test [%d/%d]:\n", i + 1, total_testcases);
        printf("\tInput string  : \"%s\"\n", testcases[i][0]);
        printf("\tExpected Hash : \"%s\"\n", testcases[i][1]);
        printf("\tActual Hash   : \"%s\"\n", actual_hash);
        printf("\tResult        : ");

        if (strcmp(actual_hash, testcases[i][1]) == 0) {
            printf("Passed\n");
            passed_counter++;
        } else {
            printf("Failed\n");
        }

        printf("\nSummary:\n");
        printf("\t%d tests out of %d PASSED\n", passed_counter, total_testcases);
    }
}

void help() {
    // Brief explanation of how to use the program

    printf("Usage: md5 [option] [string]\n\n");
    printf("Generate and print the MD5 checksum\n");
    printf("\t-h --help\tRun this help menu\n");
    printf("\t-t --test\tTo run a suite of tests\n");
    printf("\t-f --file\tSpecify a file to hash\n");
}

int main(int argc, char *argv[]) {
    char *buffer;
    size_t bufsize = 32;

    int c;

    while (1) {
        static struct option long_options[] =
                {
                        {"test", no_argument,       0, 't'},
                        {"help", no_argument,       0, 'h'},
                        {"file", required_argument, 0, 'f'},
                        {0, 0,                      0, 0}
                };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "t::f:", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {
            case 'h':
                help();
                exit(0);

            case 't':
                test();
                exit(0);

            case 'f':
                printf("option -f with value `%s'\n", optarg);
                exit(0);

            default:
                exit(1);
        }
    }

    // If a word is provided as an are it is hashed otherwise the user is prompted to type an input string
    if (optind < argc) {
        printf("%s%c", hash_string(argv[optind++]), '\n');
    } else {
        buffer = (char *) malloc(bufsize * sizeof(char));
        if (buffer == NULL) {
            perror("Unable to allocate buffer");
            exit(1);
        }

        getline(&buffer, &bufsize, stdin);
        // Get line reads the line from STDIN therefore the trailing new line needs to be removed to generate the correct hash
        buffer[strlen(buffer) - 1] = '\0';

        printf("%s%c", hash_string(buffer), '\n');
    }

    exit(0);
}

