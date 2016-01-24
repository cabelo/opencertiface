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

#ifndef JANUS_H
#define JANUS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \mainpage
 * \section overview Overview
 *
 * Janus is a *C* API consisting of three header files:
 *
 * Header      | Documentation  | Required               | Description
 * ----------- | -------------  | ---------------------- | -----------
 * janus.h     | \ref janus     | **Yes**                | \copybrief janus
 * janus_io.h  | \ref janus_io  | No (Provided)          | \copybrief janus_io
 * janus_aux.h | \ref janus_aux | No (Phases 2 & 3 only) | \copybrief janus_aux
 *
 * - [<b>Source Code</b>](https://github.com/biometrics/janus) [github.com]
 * - [<b>Program Homepage</b>]
 *          (http://www.iarpa.gov/index.php/research-programs/janus) [iarpa.gov]
 *
 * \subsection license License
 * The API is provided under a [BSD-like license](LICENSE.txt) and is
 * *free for academic and commercial use*.
 */

#if defined JANUS_LIBRARY
#  if defined _WIN32 || defined __CYGWIN__
#    define JANUS_EXPORT __declspec(dllexport)
#  else
#    define JANUS_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  if defined _WIN32 || defined __CYGWIN__
#    define JANUS_EXPORT __declspec(dllimport)
#  else
#    define JANUS_EXPORT
#  endif
#endif

#define JANUS_VERSION_MAJOR 0
#define JANUS_VERSION_MINOR 1
#define JANUS_VERSION_PATCH 0

/*!
 * \defgroup janus Janus
 * \brief Mandatory interface for Phase 1.
 *
 * All Janus performers should adhere to this interface.
 *
 * \section Overview
 * A Janus application begins with a call to \ref janus_initialize. New
 * templates are constructed with \ref janus_allocate and provided
 * image data with \ref janus_augment. Templates are freed after use with
 * \ref janus_free.
 *
 * Templates can be used for verification with \ref janus_flatten and
 * \ref janus_verify, and search with \ref janus_enroll and \ref janus_search.
 * Complete similarity matrix construction is offered by \ref janus_compare.
 *
 * Transfer learning is optionally offered by \ref janus_train, whose model file
 * output is then passed to \ref janus_initialize.
 *
 * All Janus applications end with a call to \ref janus_finalize.
 *
 * \section thread_safety Thread Safety
 * All functions are marked one of:
 * - \anchor thread_safe \par thread-safe
 *   Can be called simultaneously from multiple threads, even when the
 *   invocations use shared data.
 * - \anchor reentrant \par reentrant
 *   Can be called simultaneously from multiple threads, but only if each
 *   invocation uses its own data.
 * - \anchor thread_unsafe \par thread-unsafe
 *   Can not be called simultaneously from multiple threads.
 *
 * \section implementer_notes Implementer Notes
 * - Define \c JANUS_LIBRARY during compilation to export Janus symbols and
 *   compile a Unix implementation with \c \-fvisibility=hidden.
 * - Follow the <a href="http://www.pathname.com/fhs/">Filesystem Hierarchy
 *   Standard</a> by organizing the implementation into \c bin, \c include,
 *   \c lib, \c share and \c src sub-folders.
 *
 * \addtogroup janus
 * @{
 */

/*!
 * \brief Return type for functions that indicate an error status.
 *
 * All error values are positive integers, with the exception of #JANUS_SUCCESS
 * = 0 which indicates no error.
 */
typedef enum janus_error
{
    JANUS_SUCCESS         = 0, /*!< No error */
    JANUS_UNKNOWN_ERROR      , /*!< Catch-all error code */
    JANUS_OUT_OF_MEMORY      , /*!< Memorry allocation failed */
    JANUS_INVALID_SDK_PATH   , /*!< Incorrect location provided to
                                    #janus_initialize */
    JANUS_OPEN_ERROR         , /*!< Failed to open a file */
    JANUS_READ_ERROR         , /*!< Failed to read from a file */
    JANUS_WRITE_ERROR        , /*!< Failed to write to a file */
    JANUS_PARSE_ERROR        , /*!< Failed to parse file */
    JANUS_INVALID_IMAGE      , /*!< Could not decode image file */
    JANUS_INVALID_VIDEO      , /*!< Could not decode video file */
    JANUS_MISSING_TEMPLATE_ID, /*!< Expected a missing template ID */
    JANUS_MISSING_FILE_NAME  , /*!< Expected a missing file name */
    JANUS_NULL_ATTRIBUTE_LIST, /*!< Null #janus_attribute_list */
    JANUS_MISSING_ATTRIBUTES , /*!< Not all required attributes were
                                    provided */
    JANUS_FAILURE_TO_ENROLL  , /*!< Could not construct a template
                                    from the provided image and
                                    attributes */
    JANUS_NOT_IMPLEMENTED    , /*!< Optional functions may return this value in
                                    lieu of a meaninful implementation */
    JANUS_NUM_ERRORS           /*!< Idiom to iterate over all errors */
} janus_error;

/*!
 * \brief Data buffer type.
 */
typedef uint8_t janus_data;

/*!
 * \brief Supported image formats.
 */
typedef enum janus_color_space
{
    JANUS_GRAY8, /*!< \brief 1 channel grayscale, 8-bit depth. */
    JANUS_BGR24  /*!< \brief 3 channel color (BGR order), 8-bit depth. */
} janus_color_space;

/*!
 * \brief Common representation for still images and video frames.
 *
 * Pixels are stored \em continuously in the janus_image::data buffer.
 * Pixel layout with respect to decreasing spatial locality is \a channel,
 * \a column, \a row.
 * Thus pixel intensity can be retrieved as follows:
 *
\code
janus_data get_intensity(janus_image image, size_t channel, size_t column,
                                                                     size_t row)
{
    const size_t columnStep = (image.image_format == JANUS_COLOR ? 3 : 1);
    const size_t rowStep = image.columns * columnStep;
    const size_t index = row*rowStep + column*columnStep + channel;
    return image.data[index];
}
\endcode
 *
 * (0, 0) corresponds to the top-left corner of the image.
 * (width-1, height-1) corresponds to the bottom-right corner of the image.
 */
typedef struct janus_image
{
    janus_data *data;              /*!< \brief Data buffer. */
    size_t width;                  /*!< \brief Column count in pixels. */
    size_t height;                 /*!< \brief Row count in pixels. */
    janus_color_space color_space; /*!< \brief Arrangement of #data. */
} janus_image;

/*!
 * \brief A measurement made on a #janus_image.
 *
 * These attributes are provided from manual annotation for Phase 1.
 * Phases 2 and 3 will introduce API calls for automated detection and tracking.
 * Additional attributes and training data will be added over the duration of
 * the program.
 */
typedef enum janus_attribute
{
    JANUS_INVALID_ATTRIBUTE = 0, /*!< Catch-all error code */
    JANUS_FRAME                , /*!< Video frame number, -1 (or not present)
                                      for still images */
    JANUS_FACE_X               , /*!< Horizontal offset to top-left corner of
                                      face (pixels) */
    JANUS_FACE_Y               , /*!< Vertical offset to top-left corner of
                                      face (pixels) */
    JANUS_FACE_WIDTH           , /*!< Face horizontal size (pixels) */
    JANUS_FACE_HEIGHT          , /*!< Face vertical size (pixels) */
    JANUS_RIGHT_EYE_X          , /*!< Face landmark (pixels) */
    JANUS_RIGHT_EYE_Y          , /*!< Face landmark (pixels) */
    JANUS_LEFT_EYE_X           , /*!< Face landmark (pixels) */
    JANUS_LEFT_EYE_Y           , /*!< Face landmark (pixels) */
    JANUS_NOSE_BASE_X          , /*!< Face landmark (pixels) */
    JANUS_NOSE_BASE_Y          , /*!< Face landmark (pixels) */
    JANUS_NUM_ATTRIBUTES         /*!< Idiom to iterate over all attributes */
} janus_attribute;

/*!
 * \brief A list of #janus_attribute and value pairs all belonging to a the same
 *        object in a particular image.
 */
typedef struct janus_attribute_list
{
    size_t size; /*!< \brief Size of #attributes and #values. */
    janus_attribute *attributes; /*!< \brief Array of #janus_attribute. */
    double *values; /*!< \brief Array of corresponding attribute values. */
} janus_attribute_list;

/*!
 * \brief Call once at the start of the application, before making any other
 * calls to the API.
 *
 * \param[in] sdk_path Path to the \em read-only directory containing the
 *                     janus-compliant SDK as provided by the implementer.
 * \param[in] model_file An empty string indicating the default algorithm, an
 *                       implementation-defined string indicating a specific
 *                       algorithm configuration, or the path to a model file
 *                       created by \ref janus_train.
 * \remark This function is \ref thread_unsafe and should only be called once.
 * \see janus_finalize
 */
JANUS_EXPORT janus_error janus_initialize(const char *sdk_path,
                                          const char *model_file);

/*!
 * \brief Call once at the end of the application, after making all other calls
 * to the API.
 * \remark This function is \ref thread_unsafe and should only be called once.
 * \see janus_initialize
 */
JANUS_EXPORT janus_error janus_finalize();

/*!
 * \brief Contains the recognition information for an object.
 *
 * Create a new template with \ref janus_allocate.
 * Add images and videos to the template using \ref janus_augment and
 * \ref janus_track.
 * Finalize the template for comparison with \ref janus_flatten.
 * \see janus_flat_template
 */
typedef struct janus_template_type *janus_template;

/*!
 * \brief Allocate memory for an empty template.
 *
 * Add images to it with \ref janus_augment.
 *
 * \code
 * janus_template template_;
 * janus_error error = janus_allocate(&template_);
 * assert(!error);
 * \endcode
 *
 * \param[in] template_ An uninitialized template.
 * \remark This function is \ref reentrant.
 * \see janus_free
 */
JANUS_EXPORT janus_error janus_allocate(janus_template *template_);

/*!
 * \brief Add an image to the template.
 *
 * For video frames, call \ref janus_track first.
 *
 * Augmented templates can then be passed to \ref janus_flatten for verification
 * or \ref janus_enroll for gallery construction.
 *
 * \param[in] image The image containing the detected object to be recognized.
 * \param[in] attributes Location and metadata associated with the detected
 *                       object to recognize.
 * \param[in,out] template_ The template to contain the object's recognition
 *                          information.
 * \remark This function is \ref reentrant.
 */
JANUS_EXPORT janus_error janus_augment(const janus_image image,
                                       const janus_attribute_list attributes,
                                       janus_template template_);

/*!
 * \brief Enable or disable object tracking for the template.
 *
 * Call this function before the first, and after the last, video frame is added
 * with \ref janus_augment.
 *
 * \param[in] template_ The template to contain the tracked object.
 * \param[in] enabled If true, images provided in subsequent calls to
 *                    \ref janus_augment with template_ are sequential frames in
 *                    a video.
 * \remark This function is \ref reentrant.
 * \see janus_augment
 */
JANUS_EXPORT janus_error janus_track(janus_template template_,
                                     int enabled);

/*!
 * \brief Free memory for a template previously allocated by \ref janus_allocate.
 *
 * Call this function on a template after it is no longer needed.
 * \param[in] template_ The template to deallocate.
 * \remark This function is \ref reentrant.
 * \see janus_allocate
 */
 JANUS_EXPORT janus_error janus_free(janus_template template_);

/*!
 * \brief A finalized representation of a template suitable for comparison.
 * \see janus_template
 */
typedef janus_data *janus_flat_template;

/*!
 * \brief The maximum size of templates generated by \ref janus_flatten.
 *
 * Should be less than or equal to 32 MB.
 * \remark This function is \ref thread_safe.
 */
JANUS_EXPORT size_t janus_max_template_size();

/*!
 * \brief Create a finalized template representation for verification with
 *        \ref janus_verify.
 * \param[in] template_ The recognition information to contruct the
 *                      finalized template from.
 * \param[in,out] flat_template A pre-allocated buffer no smaller than
 *                              \ref janus_max_template_size to contain the
 *                              finalized template.
 * \param[out] bytes Size of the buffer actually used to store the template.
 * \remark This function is \ref reentrant.
 */
JANUS_EXPORT janus_error janus_flatten(janus_template template_,
                                       janus_flat_template flat_template,
                                       size_t *bytes);

/*!
 * \brief Return a similarity score for two templates.
 *
 * Higher scores indicate greater similarity.
 * \param[in] a The first template to compare.
 * \param[in] a_bytes Size of template a.
 * \param[in] b The second template to compare.
 * \param[in] b_bytes Size of template b.
 * \param[out] similarity Higher values indicate greater similarity.
 * \remark This function is \ref thread_safe.
 * \see janus_search janus_compare
 */
JANUS_EXPORT janus_error janus_verify(const janus_flat_template a,
                                      const size_t a_bytes,
                                      const janus_flat_template b,
                                      const size_t b_bytes,
                                      float *similarity);

/*!
 * \brief Unique identifier for a \ref janus_template.
 *
 * Associate a template with a unique id using \ref janus_enroll.
 * Retrieve the unique id from a search using \ref janus_search.
 */
typedef int janus_template_id;

/*!
 * \brief A set of \ref janus_template in a file.
 *
 * Offers persistence and (in phases 2 & 3) sub-linear search time.
 * Add templates to the gallery using \ref janus_enroll.
 */
typedef const char *janus_gallery;

/*!
 * \brief Add a template to the gallery.
 *
 * Use \ref janus_search for searching against the gallery.
 *
 * It is up to the user to provide unique \p template_id values. The
 * implementation may assume that multiple templates with the same
 * \p template_id belong to the same identity.
 *
 * The number of templates in the gallery can be retrieved by
 * \ref janus_gallery_size.
 *
 * \param[in] template_ The template to add.
 * \param[in] template_id A unique identifier for the template.
 * \param[in] gallery The gallery file to take ownership of the template. A new
 *                    file will be created if one does not already exist.
 * \remark This function is \ref reentrant.
 */
JANUS_EXPORT janus_error janus_enroll(const janus_template template_,
                                      const janus_template_id template_id,
                                      janus_gallery gallery);

/*!
 * \brief Ranked search for a template against a gallery.
 *
 * \p template_ids and \p similarities should be pre-allocated buffers large
 * enough to contain \p requested_returns elements. \p actual_returns will be
 * less than or equal to requested_returns, depending on the contents of the
 * gallery.
 * \param [in] template_ Probe to search for.
 * \param [in] gallery Gallery to search against.
 * \param [in] requested_returns The desired number of returned results.
 * \param [out] template_ids Buffer to contain the \ref janus_template_id of the
 *                           top matching gallery templates.
 * \param [out] similarities Buffer to contain the similarity scores of the top
 *                           matching templates.
 * \param [out] actual_returns The number of populated elements in template_ids
 *                             and similarities.
 * \remark This function is \ref thread_safe.
 * \see janus_verify janus_compare
 */
JANUS_EXPORT janus_error janus_search(const janus_template template_,
                                      janus_gallery gallery,
                                      int requested_returns,
                                      janus_template_id *template_ids,
                                      float *similarities,
                                      int *actual_returns);

/*!
 * \brief Train a new model from the provided templates.
 * \note This function is optional and may return \ref JANUS_NOT_IMPLEMENTED.
 * \param[in] templates Training data to generate the model file.
 * \param[in] num_templates Length of \em partial_templates.
 * \param[out] model_file File path to contain the trained model.
 * \remark This function is \ref thread_unsafe.
 * \todo Is the API the right level of abstraction for this function? Should
 *       it be moved to the command line instead or a shell script instead? Is
 *       a common interface impossible?
 */
JANUS_EXPORT janus_error janus_train(const janus_template *templates,
                                     const int num_templates,
                                     const char *model_file);

/*!
 * \brief Retrieve the number of templates in a gallery.
 *
 * Galleries may split or join templates based on their identity
 * information, so the number of templates in the gallery is not necessarily
 * equal to the number of \ref janus_enroll calls.
 *
 * \param[in] gallery The gallery whose templates to count.
 * \param[out] size The number of templates in the gallery.
 * \remark This function is \ref thread_safe.
 */
JANUS_EXPORT janus_error janus_gallery_size(janus_gallery gallery,
                                            size_t *size);

/*!
 * \brief Generate a similarity matrix.
 *
 * Compare all templates in the target gallery are to all templates in the query
 * gallery.
 *
 * Use \ref janus_gallery_size to determine the appropriate length of the
 * pre-allocated buffers.
 * \param[in] target Templates forming the similarity matrix columns.
 * \param[in] query Templates forming the similarity matrix rows.
 * \param[out] similarity_matrix Buffer to contain the similarity scores in
 *                               row-major order.
 * \param[out] target_ids Buffer to contain the target gallery template ids.
 * \param[out] query_ids Buffer to contain the query gallery template ids.
 * \remark This function is \ref thread_safe.
 * \see janus_verify janus_search
 */
JANUS_EXPORT janus_error janus_compare(janus_gallery target,
                                       janus_gallery query,
                                       float *similarity_matrix,
                                       janus_template_id *target_ids,
                                       janus_template_id *query_ids);

/*! @}*/

#ifdef __cplusplus
}
#endif

#endif /* JANUS_H */
