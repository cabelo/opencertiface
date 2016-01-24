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

#ifndef JANUS_AUX_H
#define JANUS_AUX_H

#include <janus.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \defgroup janus_aux Janus Auxiliary
 * \brief Proposed API additions (under construction).
 * \addtogroup janus_aux
 *  @{
 */

/*!
 * \brief A collection of #janus_attribute_list all associated with the same
 *        object.
 */
typedef struct janus_object_type
{
    janus_size size; /*!< \brief Size of #attribute_lists. */
    janus_attribute_list *attribute_lists; /*!< \brief Array of
                                                       #janus_attribute_list. */
} *janus_object;

/*!
 * \brief Allocates memory for a #janus_object capable of storing \em size
 *        attribute lists.
 * \param[in] size Desired value for janus_object::size.
 * \param[out] object Address to store the allocated object.
 * \note Memory will be allocated, but not initialized, for
 *       janus_object::attribute_lists.
 * \see janus_free_object
 */
JANUS_EXPORT janus_error janus_allocate_object(const janus_size size,
                                               janus_object *object);

/*!
 * \brief Frees the memory previously allocated for the object.
 * \param[in] object #janus_object to free.
 * \see janus_allocate_object
 */
JANUS_EXPORT void janus_free_object(janus_object object);

/*!
 * \brief Retrieve the values for an attribute in an object.
 * \param[in] object The object to search.
 * \param[in] attribute The attribute to search for.
 * \param[out] values The values for the requested attribute.
 * \note values should be a pre-allocated buffer of length object->size.
 */
JANUS_EXPORT janus_error janus_get_values(const janus_object object,
                                          const janus_attribute attribute,
                                          janus_value *values);

/*!
 * \brief A list of #janus_object.
 */
typedef struct janus_object_list_type
{
    janus_size size; /*!< \brief Number of elements in #objects. */
    janus_object *objects; /*!< \brief Array of #janus_object. */
} *janus_object_list;

/*!
 * \brief Allocates memory for a #janus_object_list capable of storing \em size
 *        #janus_object.
 * \param[in] size Desired value for janus_object_list::size.
 * \param[out] object_list Address to store the allocated object list.
 * \note Memory will be allocated, but not initialized, for
 *       janus_object_list::objects.
 * \see janus_free_object_list
 */
JANUS_EXPORT janus_error janus_allocate_object_list(const janus_size size,
                                                janus_object_list *object_list);

/*!
 * \brief Frees the memory previously allocated for the object list.
 * \param[in] object_list #janus_object_list to free.
 * \note #janus_free_object will be called for each object in
 *       #janus_object_list::objects. If this behavior is undesired, set
 *       #janus_object_list::size to 0 before calling this function or set
 *       individual elements in #janus_object_list::objects to \c NULL;
 * \see janus_allocate_object_list
 */
JANUS_EXPORT void janus_free_object_list(janus_object_list object_list);

/*!
 * \brief Detect objects in a #janus_image.
 * \see janus_free_object_list
 */
JANUS_EXPORT janus_error janus_detect(const janus_image image,
                                      janus_object_list *object_list);

/*!
 * \brief Contains tracking information for objects in a video.
 */
typedef struct janus_track_type *janus_track;

/*!
 * \brief Create a new track.
 */
JANUS_EXPORT janus_error janus_initialize_track(janus_track *track);

/*!
 * \brief Add a frame to the track.
 */
JANUS_EXPORT janus_error janus_track_frame(const janus_image frame,
                                           janus_track track);

/*!
 * \brief Free the track and compute the detected objects.
 */
JANUS_EXPORT janus_error janus_finalize_track(janus_track track,
                                              janus_object_list *object_list);

/*! @}*/

#ifdef __cplusplus
}
#endif

#endif /* JANUS_AUX_H */
