#include <stdlib.h>
#include <string.h>

#include "iarpa_janus.h"
#include "iarpa_janus_io.h"

const char *get_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void printUsage()
{
    printf("Usage: janus_create_templates sdk_path temp_path data_path metadata_file gallery_file [-algorithm <algorithm>] [-verbose]\n");
}

int main(int argc, char *argv[])
{
    int requiredArgs = 6;

    if ((argc < requiredArgs) || (argc > 9)) {
        printUsage();
        return 1;
    }

    const char *ext1 = get_ext(argv[4]);
    const char *ext2 = get_ext(argv[5]);
    if (strcmp(ext1, "csv") != 0) {
        printf("metadata_file must be \".csv\" format.\n");
        return 1;
    } else if (strcmp(ext2, "gal") != 0) {
        printf("gallery_file must be \".gal\" format. \n");
        return 1;
    }

    char *algorithm = NULL;
    int verbose = 0;

    for (int i=0; i<argc-requiredArgs; i++)
        if (strcmp(argv[requiredArgs+i],"-algorithm") == 0)
            algorithm = argv[requiredArgs+(++i)];
        else if (strcmp(argv[requiredArgs+i],"-verbose") == 0)
            verbose = 1;
        else {
            fprintf(stderr, "Unrecognized flag: %s\n", argv[requiredArgs+i]);
            return 1;
        }

    JANUS_ASSERT(janus_initialize(argv[1], argv[2], algorithm))
    JANUS_ASSERT(janus_create_templates(argv[3], argv[4], argv[5], verbose))
    JANUS_ASSERT(janus_finalize())

    janus_print_metrics(janus_get_metrics());

    return EXIT_SUCCESS;
}
