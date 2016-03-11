/*******************************************************************************
 * Copyright (c) 2013 Noblis, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 ******************************************************************************/

#ifndef IARPA_JANUS_IO_H
#define IARPA_JANUS_IO_H

#include <iarpa_janus.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \defgroup janus_io Janus I/O
 * \brief Media decoding and evaluation harness.
 * \addtogroup janus_io
 *  @{
 */

/*!
 * \brief The \c JANUS_ASSERT macro provides a simple unrecoverable error
 *        handling mechanism.
 * \see JANUS_CHECK
 */
#define JANUS_ASSERT(EXPRESSION)                                     \
{                                                                    \
    const janus_error error = (EXPRESSION);                          \
    if (error != JANUS_SUCCESS) {                                    \
        fprintf(stderr, "Janus error: %s\n\tFile: %s\n\tLine: %d\n", \
                janus_error_to_string(error),                        \
                __FILE__,                                            \
                __LINE__);                                           \
        abort();                                                     \
    }                                                                \
}                                                                    \

/*!
 * \brief The \c JANUS_CHECK macro provides a simple recoverable error handling
 *        mechanism.
 * \see JANUS_ASSERT
 */
#define JANUS_CHECK(EXPRESSION)             \
{                                           \
    const janus_error error = (EXPRESSION); \
    if (error != JANUS_SUCCESS)             \
        return error;                       \
}                                           \

/*!
 * \brief #janus_error to string.
 * \param[in] error Error code to stringify.
 * \note Memory for the return value is static and should not be freed.
 * \remark This function is \ref thread_safe.
 */
JANUS_EXPORT const char *janus_error_to_string(janus_error error);

/*!
 * \brief #janus_error from string.
 * \param[in] error String to decode.
 * \remark This function is \ref thread_safe.
 * \see janus_enum
 */
JANUS_EXPORT janus_error janus_error_from_string(const char *error);

/*!
 * \brief Read an image from disk.
 * \param[in] file_name Path to the image file.
 * \param[out] image Address to store the decoded image.
 * \remark This function is \ref reentrant.
 * \see janus_free_image
 */
JANUS_EXPORT janus_error janus_read_image(const char *file_name,
                                                            janus_image *image);

/*!
 * \brief Frees the memory previously allocated for a #janus_image.
 * \param[in] image #janus_image to free.
 * \remark This function is \ref reentrant.
 * \see janus_allocate_image
 */
JANUS_EXPORT void janus_free_image(janus_image image);

/*!
 * \brief Handle to a private video decoding type.
 */
typedef struct janus_video_type *janus_video;

/*!
 * \brief Returns a video ready for reading.
 * \param[in] file_name Path to image file.
 * \param[out] video Address to store the allocated video.
 * \remark This function is \ref reentrant.
 * \see janus_read_frame janus_close_video
 */
JANUS_EXPORT janus_error janus_open_video(const char *file_name,
                                                            janus_video *video);

/*!
 * \brief Returns the current frame and advances the video to the next frame.
 * \param[in] video Video to decode.
 * \param[out] image Address to store the allocated image.
 * \remark This function is \ref reentrant.
 * \see janus_open_video janus_free_image
 */
JANUS_EXPORT janus_error janus_read_frame(janus_video video, janus_image *image);

/*!
 * \brief Closes a video previously opened by \ref janus_open_video.
 *
 * Call this function to deallocate the memory allocated to decode the video
 * after the desired frames have been read.
 * \param[in] video The video to close.
 * \remark This function is \ref reentrant.
 */
JANUS_EXPORT void janus_close_video(janus_video video);

/*!
 * \brief File name for a Janus Metadata File
 *
 * A *Janus Metadata File* is a *Comma-Separated Value* (CSV) text file with the following format:
 *
\verbatim
TEMPLATE_ID        , SUBJECT_ID, FILE_NAME, MEDIA_ID, FRAME, <janus_attribute>, <janus_attribute>, ..., <janus_attribute>
<janus_template_id>, <int>     , <string> , <int>   , <int>, <double>         , <double>         , ..., <double>
<janus_template_id>, <int>     , <string> , <int>   , <int>, <double>         , <double>         , ..., <double>
...
<janus_template_id>, <int>     , <string> , <int>   , <int>, <double>         , <double>         , ..., <double>
\endverbatim
 *
 * Where:
 * - [TEMPLATE_ID](\ref janus_template_id) is a unique integer identifier indicating rows that belong to the same template.
 * - \c SUBJECT_ID is a unique integer identifier used to establish ground truth match/non-match.
 *      For the purpose of experimentation, multiple \c TEMPLATE_ID may have the same \c SUBJECT_ID.
 * - \c FILE_NAME is a path to the image or video file on disk.
 * - \c MEDIA_ID is a unique integer identifier indicating rows that belong to the same piece of media (image or video clip).
 * - \c FRAME is the video frame number and -1 (or empty string) for still images.
 * - All rows associated with the same \c TEMPLATE_ID occur sequentially.
 * - All rows associated with the same \c TEMPLATE_ID and \c FILE_NAME occur sequentially ordered by \c FRAME.
 * - A cell is empty when no value is available for the specified attribute.
 *
 * \par Examples:
 * - [meds.csv](https://raw.githubusercontent.com/biometrics/janus/master/data/meds.csv)
 * - [Kirchner.csv](https://raw.githubusercontent.com/biometrics/janus/master/data/Kirchner.csv)
 * - [Toledo.csv](https://raw.githubusercontent.com/biometrics/janus/master/data/Toledo.csv)
 */
typedef const char *janus_metadata;

/*!
 * \brief High-level function for enrolling a template from a metadata file.
 * \param [in] data_path Prefix path to files in metadata.
 * \param [in] metadata #janus_metadata file to enroll.
 * \param [out] template_ Constructed template.
 * \param [out] template_id Template ID from metadata.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_create_template(const char *data_path, janus_metadata metadata, janus_template *template_, janus_template_id *template_id);

/*!
 * \brief High-level function for enrolling templates from a metadata file and writing templates to disk.
 * \param [in] data_path Prefix path to files in metadata.
 * \param [in] metadata #janus_metadata to enroll.
 * \param [in] gallery_file File to save the gallery to.
 * \param [in] verbose Print information and warnings during gallery enrollment.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_create_templates(const char *data_path, janus_metadata metadata, const char *gallery_file, int verbose);

/*!
 * \brief High-level function for enrolling a gallery from a metadata file.
 * \param [in] data_path Prefix path to files in metadata.
 * \param [in] metadata #janus_metadata to enroll.
 * \param [in] gallery_path Path to gallery folder.
 * \param [in] verbose Print information and warnings during gallery enrollment.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_create_gallery(const char *data_path, janus_metadata metadata, janus_gallery_path gallery_path, int verbose);

/*!
 * \brief A dense binary 2D matrix file.
 *
 * Can be either the \a similarity or \a mask matrix format described in
 * <a href="http://openbiometrics.org/doxygen/latest/MBGC_file_overview.pdf#page=12">MBGC File Overview</a>.
 * \see janus_write_matrix janus_evaluate_search janus_evaluate_verify janus_verify_pairwise
 */
typedef const char *janus_matrix;

/*!
 * \brief Utility function for creating a \ref janus_matrix.
 * \param[in] data Matrix data.
 * \param[in] rows Matrix rows.
 * \param[in] columns Matrix columns.
 * \param[in] is_mask If non-zero, data is a uint8_t* of mask values, otherwise data is treated as a float* of similarity values.
 * \param[in] target Target gallery file name recorded in the matrix header.
 * \param[in] query Query gallery file name recorded in the matrix header.
 * \param[in] matrix File to write the matrix to.
 * \remark This function is \ref thread_safe.
 */
JANUS_EXPORT janus_error janus_write_matrix(void *data, int rows, int columns, int is_mask, janus_metadata target, janus_metadata query, janus_matrix matrix);

/*!
 * \brief Create similarity and mask matricies from two galleries with calls to janus_search.
 *
 * The \c SUBJECT_ID field is used to determine ground truth match/non-match in the mask.
 * Refer to <a href="http://www.nist.gov/itl/iad/ig/face.cfm"><b>face.nist.gov</b></a> for more information on IJB-A and the ".candidate_lists" format.
 * \param[in] target Path to gallery being seached against on disk.
 * \param[in] query Templates file created fron janus_create_templates to constitute the rows for the matrix.
 * \param[in] target_metadata metadata file for \p target.
 * \param[in] query_metadata metadata file for \p query.
 * \param[in] simmat Similarity matrix file to be created.
 * \param[in] mask Mask matrix file to be created.
 * \param[in] candidate_lists Text file containing template size and score for each search's requested returns.
 * \param[in] num_requested_returns Desired number of returned results for each call to janus_search.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_evaluate_search(janus_gallery_path target, const char *query, janus_metadata target_metadata, janus_metadata query_metadata, janus_matrix simmat, janus_matrix mask, const char *candidate_lists, size_t num_requested_returns);

/*!
 * \brief Create similarity and mask matricies from two galleries with calls to janus_verify.
 *
 * The \c SUBJECT_ID field is used to determine ground truth match/non-match in the mask.
 * \param[in] target Templates file created from janus_create_templates to constitute the columns of the matrix.
 * \param[in] query Templates file created from janus_create_templates to constitute the rows for the matrix.
 * \param[in] target_metadata metadata file for \p target.
 * \param[in] query_metadata metadata file for \p query.
 * \param[in] simmat Similarity matrix file to be created.
 * \param[in] mask Mask matrix file to be created.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_evaluate_verify(const char *target, const char *query, janus_metadata target_metadata, janus_metadata query_metadata, janus_matrix simmat, janus_matrix mask);

/*!
 * \brief High-level function for executing a list of template comparisons.
 *
 * Refer to <a href="http://www.nist.gov/itl/iad/ig/face.cfm"><b>face.nist.gov</b></a> for more information on IJB-A and the ".matches" format.
 * \param[in] comparisons_file File containing the list of template comparisons to make.
 * \param[in] templates_file Templates file created from janus_create_templates containing the comparison templates.
 * \param[in] template_metadata metadata file for comparison templates.
 * \param[in] simmat Similarity matrix file to be created.
 * \param[in] mask Mask matrix file to be created.
 * \param[in] match_scores Output matches file with template size and score recorded for each comparison.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT janus_error janus_verify_pairwise(const char *comparisons_file, const char *templates_file, janus_metadata template_metadata, janus_matrix simmat, janus_matrix mask, const char *match_scores);

/*!
 * \brief A statistic.
 * \see janus_metrics
 */
struct janus_metric
{
    size_t count;  /*!< \brief Number of samples. */
    double mean;   /*!< \brief Sample average. */
    double stddev; /*!< \brief Sample standard deviation. */
};

/*!
 * \brief All statistics.
 * \see janus_get_metrics
 */
struct janus_metrics
{
    struct janus_metric janus_initialize_template_speed; /*!< \brief ms */
    struct janus_metric janus_detection_speed; /*!< \brief ms */
    struct janus_metric janus_augment_speed; /*!< \brief ms */
    struct janus_metric janus_finalize_template_speed; /*!< \brief ms */
    struct janus_metric janus_read_image_speed; /*!< \brief ms */
    struct janus_metric janus_free_image_speed; /*!< \brief ms */
    struct janus_metric janus_verify_speed; /*!< \brief ms */
    struct janus_metric janus_search_speed; /*!< \brief ms */
    struct janus_metric janus_gallery_size_speed; /*!< \brief ms */
    struct janus_metric janus_finalize_gallery_speed; /*!< \brief ms */
    struct janus_metric janus_template_size; /*!< \brief KB */
    int                 janus_missing_attributes_count; /*!< \brief Count of
                                                             \ref JANUS_MISSING_ATTRIBUTES */
    int                 janus_failure_to_detect_count; /*!< \brief Count of
                                                            \ref JANUS_FAILURE_TO_DETECT */
    int                 janus_failure_to_enroll_count; /*!< \brief Count of
                                                            \ref JANUS_FAILURE_TO_ENROLL */
    int                 janus_other_errors_count; /*!< \brief Count of \ref janus_error excluding
                                                       \ref JANUS_MISSING_ATTRIBUTES,
                                                       \ref JANUS_FAILURE_TO_ENROLL, and
                                                       \ref JANUS_SUCCESS */
};

/*!
 * \brief Retrieve and reset performance metrics.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT struct janus_metrics janus_get_metrics();

/*!
 * \brief Print metrics to stdout.
 * \note Will only print metrics with count > 0 occurrences.
 * \remark This function is \ref thread_unsafe.
 */
JANUS_EXPORT void janus_print_metrics(struct janus_metrics metrics);

/*! @}*/

#ifdef __cplusplus
}
#endif

#endif /* IARPA_JANUS_IO_H */
