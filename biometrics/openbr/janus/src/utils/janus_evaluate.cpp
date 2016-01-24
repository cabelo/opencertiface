#include <stdlib.h>
#include <string.h>

#include "janus.h"
#include "janus_io.h"

const char *get_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

int main(int argc, char *argv[])
{
    if ((argc < 8) || (argc > 9)) {
        printf("Usage: janus_evaluate sdk_path target_gallery query_gallery target_metadata query_metadata simmat mask [algorithm]\n");
        return 1;
    }
    const char *ext1 = get_ext(argv[2]);
    const char *ext2 = get_ext(argv[3]);
    const char *ext3 = get_ext(argv[4]);
    const char *ext4 = get_ext(argv[5]);
    const char *ext5 = get_ext(argv[6]);
    const char *ext6 = get_ext(argv[7]);

    if (strcmp(ext1, "gal") != 0 || strcmp(ext2, "gal") != 0) {
    	printf("Gallery files must be \".gal\" format\n");
    	return 1;
    }

    if (strcmp(ext3, "csv") != 0 || strcmp(ext4, "csv") != 0) {
    	printf("Metadata files must be \".csv\" format\n");
    	return 1;
    }

    if (strcmp(ext5, "mtx") != 0) {
    	printf("Score matrix files must be \".mtx\" format\n");
    	return 1;
    }

    if (strcmp(ext6, "mask") != 0) {
    	printf("Mask matrix files must be \".mask\" format\n");
    	return 1;
    }

    JANUS_ASSERT(janus_initialize(argv[1], argc >= 9 ? argv[8] : ""))
    JANUS_ASSERT(janus_evaluate(argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]))
    JANUS_ASSERT(janus_finalize())

    janus_print_metrics(janus_get_metrics());
    return EXIT_SUCCESS;
}
