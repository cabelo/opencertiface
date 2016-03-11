#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "iarpa_janus.h"
#include "iarpa_janus_io.h"
using namespace std;

const char *get_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

void printUsage()
{
    printf("Usage: janus_evaluate_search sdk_path temp_path target_gallery query_gallery target_metadata query_metadata simmat mask candidate_lists num_returns [-algorithm <algorithm>]\n");
}

int main(int argc, char *argv[])
{
    int requiredArgs = 11;

    if ((argc < requiredArgs) || (argc > 13)) {
        printUsage();
        return 1;
    }

    const char *ext1 = get_ext(argv[4]);
    const char *ext2 = get_ext(argv[5]);
    const char *ext3 = get_ext(argv[6]);
    const char *ext4 = get_ext(argv[7]);
    const char *ext5 = get_ext(argv[8]);
    const char *ext6 = get_ext(argv[9]);

    if (strcmp(ext1, "gal") != 0) {
        printf("Query gallery file must be \".gal\" format.\n");
        return 1;
    } else if (strcmp(ext2, "csv") != 0 || strcmp(ext3, "csv") != 0) {
        printf("Metadata files must be \".csv\" format. \n");
        return 1;
    } else if (strcmp(ext4, "mtx") != 0) {
        printf("Similarity matrix files should be \".mtx\" format.\n");
        return 1;
    } else if (strcmp(ext5, "mask") != 0) {
        printf("Mask matrix files should be \".mask\" format.\n");
        return 1;
    } else if (strcmp(ext6, "candidate_lists") != 0) {
        printf("candidate_lists must be \".candidate_lists\" format.\n");
        return 1;
    }

    char *algorithm = NULL;
    for (int i=0; i<argc-requiredArgs; i++)
        if (strcmp(argv[requiredArgs+i], "-algorithm") == 0)
            algorithm = argv[requiredArgs+(++i)];
        else {
            fprintf(stderr, "Unrecognized flag: %s\n", argv[requiredArgs+i]);
            return 1;
        }

    JANUS_ASSERT(janus_initialize(argv[1], argv[2], algorithm))
    size_t num_requested_returns = atoi(argv[10]);

    JANUS_ASSERT(janus_evaluate_search(argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], num_requested_returns))
    JANUS_ASSERT(janus_finalize())

    janus_print_metrics(janus_get_metrics());
    return EXIT_SUCCESS;
}
