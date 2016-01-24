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
    if ((argc < 5) || (argc > 7)) {
        printf("Usage: janus_enroll sdk_path data_path metadata_file gallery_file [[algorithm] verbose]\n");
        return 1;
    }

    const char *ext1 = get_ext(argv[3]);
    const char *ext2 = get_ext(argv[4]);

    if (strcmp(ext1, "csv") != 0) {
    	printf("metadata_file must be \".csv\" format.\n");
    	return 1;
    }

    if (strcmp(ext2, "gal") != 0) {
    	printf("gallery_file must be \".gal\" format. \n");
    	return 1;
    }

    JANUS_ASSERT(janus_initialize(argv[1], argc >= 6 ? argv[5] : ""))
    JANUS_ASSERT(janus_create_gallery(argv[2], argv[3], argv[4], argc >= 7 ? atoi(argv[6]) : 0))
    JANUS_ASSERT(janus_finalize())

    janus_print_metrics(janus_get_metrics());
    return EXIT_SUCCESS;
}
