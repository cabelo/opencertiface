#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <pittpatt_errors.h>
#include <pittpatt_license.h>
#include <pittpatt_sdk.h>

#include "janus.h"
#include "janus_aux.h"

static janus_error to_janus_error(ppr_error_type error)
{
    if (error != PPR_SUCCESS)
        printf("PittPatt 5: %s\n", ppr_error_message(error));

    switch (error) {
      case PPR_SUCCESS:                 return JANUS_SUCCESS;
      case PPR_NULL_CONTEXT:
      case PPR_CORRUPT_CONTEXT:
      case PPR_CONTEXT_NOT_INITIALIZED: return JANUS_NULL_CONTEXT;
      case PPR_NULL_MODELS_PATH:
      case PPR_INVALID_MODELS_PATH:     return JANUS_INVALID_SDK_PATH;
      default:                          return JANUS_UNKNOWN_ERROR;
    }
}

#define JANUS_TRY_PPR(PPR_API_CALL)            \
{                                              \
    ppr_error_type ppr_error = (PPR_API_CALL); \
    if (ppr_error != PPR_SUCCESS)              \
        return to_janus_error(ppr_error);      \
}

janus_error janus_initialize(const char *sdk_path, const char *model_file)
{
    (void) model_file;
    const char *models = "/models/";
    const size_t models_path_len = strlen(sdk_path) + strlen(models);
    char *models_path = malloc(models_path_len);
    snprintf(models_path, models_path_len, "%s%s", sdk_path, models);

    janus_error error = to_janus_error(ppr_initialize_sdk(models_path, my_license_id, my_license_key));
    free(models_path);
    return error;
}

void janus_finalize()
{
    ppr_finalize_sdk();
}

static ppr_error_type initialize_ppr_context(ppr_context_type *context, int tracking)
{
    ppr_settings_type settings = ppr_get_default_settings();
    settings.detection.enable = 1;
    settings.detection.min_size = 4;
    settings.detection.max_size = PPR_MAX_MAX_SIZE;
    settings.detection.adaptive_max_size = 1.f;
    settings.detection.adaptive_min_size = 0.01f;
    settings.detection.threshold = 0;
    settings.detection.use_serial_face_detection = 1;
    settings.detection.num_threads = 1;
    settings.detection.search_pruning_aggressiveness = 0;
    settings.detection.detect_best_face_only = 0;
    settings.landmarks.enable = 1;
    settings.landmarks.landmark_range = PPR_LANDMARK_RANGE_COMPREHENSIVE;
    settings.landmarks.manually_detect_landmarks = 0;
    settings.recognition.enable_extraction = 1;
    settings.recognition.enable_comparison = 1;
    settings.recognition.recognizer = PPR_RECOGNIZER_MULTI_POSE;
    settings.recognition.num_comparison_threads = 1;
    settings.recognition.automatically_extract_templates = 0;
    settings.recognition.extract_thumbnails = 0;
    settings.tracking.enable = tracking;
    if (tracking) {
        settings.tracking.cutoff = 0;
        settings.tracking.discard_completed_tracks = 0;
        settings.tracking.enable_shot_boundary_detection = 1;
        settings.tracking.shot_boundary_threshold = 0;
    }
    return ppr_initialize_context(settings, context);
}

janus_error janus_initialize_context(janus_context *context)
{
    if (!context) return JANUS_NULL_CONTEXT;
    ppr_context_type ppr_context;
    ppr_error_type ppr_error = initialize_ppr_context(&ppr_context, 0);
    *context = (janus_context)ppr_context;
    return to_janus_error(ppr_error);
}

void janus_finalize_context(janus_context context)
{
    if (!context) return;
    ppr_finalize_context((ppr_context_type)context);
}

static ppr_error_type to_ppr_image(const janus_image image, ppr_image_type *ppr_image)
{
    ppr_raw_image_type raw_image;
    raw_image.bytes_per_line = (image.color_space == JANUS_BGR24 ? 3 : 1) * image.width;
    raw_image.color_space = (image.color_space == JANUS_BGR24 ? PPR_RAW_IMAGE_BGR24 : PPR_RAW_IMAGE_GRAY8);
    raw_image.data = image.data;
    raw_image.height = image.height;
    raw_image.width = image.width;
    return ppr_create_image(raw_image, ppr_image);
}

static janus_error to_janus_attribute_list(ppr_face_type face, janus_attribute_list *attribute_list, int media_id)
{
    ppr_face_attributes_type face_attributes;
    ppr_get_face_attributes(face, &face_attributes);

    const int num_face_attributes = 11;
    janus_attribute_list result;
    janus_allocate_attribute_list(num_face_attributes + 2*face_attributes.num_landmarks, &result);
    result->attributes[0] = JANUS_MEDIA_ID;
    result->values[0] = media_id;
    result->attributes[1] = JANUS_FRAME;
    result->values[1] = face_attributes.tracking_info.frame_number;
    result->attributes[2] = JANUS_TRACKING_CONFIDENCE;
    result->values[2] = face_attributes.tracking_info.confidence_level;
    result->attributes[3] = JANUS_FACE_CONFIDENCE;
    result->values[3] = face_attributes.confidence;
    result->attributes[4] = JANUS_FACE_WIDTH;
    result->values[4] = face_attributes.dimensions.width;
    result->attributes[5] = JANUS_FACE_HEIGHT;
    result->values[5] = face_attributes.dimensions.height;
    result->attributes[6] = JANUS_FACE_X;
    result->values[6] = face_attributes.position.x;
    result->attributes[7] = JANUS_FACE_Y;
    result->values[7] = face_attributes.position.y;
    result->attributes[8] = JANUS_FACE_ROLL;
    result->values[8] = face_attributes.rotation.roll;
    result->attributes[9] = JANUS_FACE_PITCH;
    result->values[9] = face_attributes.rotation.pitch;
    result->attributes[10] = JANUS_FACE_YAW;
    result->values[10] = face_attributes.rotation.yaw;

    ppr_landmark_list_type landmark_list;
    ppr_get_face_landmarks(face, &landmark_list);
    for (int j=0; j<face_attributes.num_landmarks; j++) {
        const int index = num_face_attributes + 2*j;
        switch (landmark_list.landmarks[j].category) {
        case PPR_LANDMARK_CATEGORY_LEFT_EYE:
            result->attributes[index] = JANUS_LEFT_EYE_X;
            result->attributes[index+1] = JANUS_LEFT_EYE_Y;
            break;
        case PPR_LANDMARK_CATEGORY_RIGHT_EYE:
            result->attributes[index] = JANUS_RIGHT_EYE_X;
            result->attributes[index+1] = JANUS_RIGHT_EYE_Y;
            break;
        case PPR_LANDMARK_CATEGORY_NOSE_BASE:
            result->attributes[index] = JANUS_NOSE_BASE_X;
            result->attributes[index+1] = JANUS_NOSE_BASE_Y;
            break;
        case PPR_LANDMARK_CATEGORY_NOSE_BRIDGE:
            result->attributes[index] = JANUS_NOSE_BRIDGE_X;
            result->attributes[index+1] = JANUS_NOSE_BRIDGE_Y;
            break;
        case PPR_LANDMARK_CATEGORY_EYE_NOSE:
            result->attributes[index] = JANUS_EYE_NOSE_X;
            result->attributes[index+1] = JANUS_EYE_NOSE_Y;
            break;
        case PPR_LANDMARK_CATEGORY_LEFT_UPPER_CHEEK:
            result->attributes[index] = JANUS_LEFT_UPPER_CHEEK_X;
            result->attributes[index+1] = JANUS_LEFT_UPPER_CHEEK_Y;
            break;
        case PPR_LANDMARK_CATEGORY_LEFT_LOWER_CHEEK:
            result->attributes[index] = JANUS_LEFT_LOWER_CHEEK_X;
            result->attributes[index+1] = JANUS_LEFT_LOWER_CHEEK_Y;
            break;
        case PPR_LANDMARK_CATEGORY_RIGHT_UPPER_CHEEK:
            result->attributes[index] = JANUS_RIGHT_UPPER_CHEEK_X;
            result->attributes[index+1] = JANUS_RIGHT_UPPER_CHEEK_Y;
            break;
        case PPR_LANDMARK_CATEGORY_RIGHT_LOWER_CHEEK:
            result->attributes[index] = JANUS_RIGHT_LOWER_CHEEK_X;
            result->attributes[index+1] = JANUS_RIGHT_LOWER_CHEEK_Y;
            break;
        case PPR_NUM_LANDMARK_CATEGORIES:
            result->attributes[index] = JANUS_INVALID_ATTRIBUTE;
            result->attributes[index+1] = JANUS_INVALID_ATTRIBUTE;
            break;
        }
        result->values[index]   = landmark_list.landmarks[j].position.x;
        result->values[index+1] = landmark_list.landmarks[j].position.y;
    }
    ppr_free_landmark_list(landmark_list);
    *attribute_list = result;
    return JANUS_SUCCESS;
}

static int media_id_counter = 0; // TODO: This should be an atomic integer

janus_error janus_detect(const janus_context context, const janus_image image, janus_object_list *object_list)
{
    if (!object_list) return JANUS_UNKNOWN_ERROR;
    *object_list = NULL;

    const int media_id = media_id_counter++;

    ppr_image_type ppr_image;
    JANUS_TRY_PPR(to_ppr_image(image, &ppr_image))

    ppr_face_list_type face_list;
    ppr_detect_faces((ppr_context_type)context, ppr_image, &face_list);

    janus_object_list result;
    janus_allocate_object_list(face_list.length, &result);
    for (janus_size i=0; i<result->size; i++) {
        janus_object object;
        janus_allocate_object(1, &object);
        to_janus_attribute_list(face_list.faces[i], &object->attribute_lists[0], media_id);
        result->objects[i] = object;
    }

    ppr_free_face_list(face_list);
    ppr_free_image(ppr_image);
    *object_list = result;
    return JANUS_SUCCESS;
}
*/

janus_error janus_initialize_track(janus_track *track)
{
    ppr_context_type ppr_context;
    ppr_error_type ppr_error = initialize_ppr_context(&ppr_context, 1);
    *track = (janus_track)ppr_context;
    return to_janus_error(ppr_error);
}

janus_error janus_track_frame(const janus_context context, const janus_image frame, janus_track track)
{
    (void) context;
    ppr_image_type ppr_image;
    JANUS_TRY_PPR(to_ppr_image(frame, &ppr_image))
    ppr_face_list_type face_list;
    ppr_error_type ppr_error = ppr_detect_faces((ppr_context_type)track, ppr_image, &face_list);
    ppr_free_face_list(face_list);
    ppr_free_image(ppr_image);
    return to_janus_error(ppr_error);
}

janus_error janus_finalize_track(janus_track track, janus_object_list *object_list)
{
    if (!object_list) return JANUS_UNKNOWN_ERROR;
    *object_list = NULL;

    const int media_id = media_id_counter++;

    ppr_track_list_type ppr_track_list;
    JANUS_TRY_PPR(ppr_finalize_tracks((ppr_context_type)track))
    JANUS_TRY_PPR(ppr_get_completed_tracks((ppr_context_type)track, &ppr_track_list));
    JANUS_TRY_PPR(ppr_finalize_context((ppr_context_type)track));

    janus_object_list result;
    janus_allocate_object_list(ppr_track_list.length, &result);
    for (janus_size i=0; i<result->size; i++) {
        ppr_track_type ppr_track = ppr_track_list.tracks[i];
        janus_object object;
        janus_allocate_object(ppr_track.tracked_faces.length, &object);
        for (janus_size j=0; j<object->size; j++)
            to_janus_attribute_list(ppr_track.tracked_faces.faces[j], &object->attribute_lists[j], media_id);
        result->objects[i] = object;
    }

    *object_list = result;
    return JANUS_SUCCESS;
}
