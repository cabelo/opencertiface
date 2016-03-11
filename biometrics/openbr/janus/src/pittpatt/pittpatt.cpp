#include <cstring>
#include <limits>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>

#include <pittpatt_errors.h>
#include <pittpatt_license.h>
#include <pittpatt_sdk.h>

#include "iarpa_janus.h"

using namespace std;

ppr_context_type ppr_context;

struct janus_template_type {
    vector<ppr_face_list_type> ppr_face_lists;
};

struct janus_gallery_type {
    ppr_gallery_type ppr_gallery;
};

static janus_error to_janus_error(ppr_error_type error)
{
    if (error != PPR_SUCCESS)
        printf("PittPatt 5: %s\n", ppr_error_message(error));

    switch (error) {
      case PPR_SUCCESS:                 return JANUS_SUCCESS;
      case PPR_NULL_MODELS_PATH:
      case PPR_INVALID_MODELS_PATH:     return JANUS_INVALID_SDK_PATH;
      case PPR_NULL_IMAGE:
      case PPR_INVALID_RAW_IMAGE:
      case PPR_INCONSISTENT_IMAGE_DIMENSIONS:
                                        return JANUS_INVALID_IMAGE;
      default:                          return JANUS_UNKNOWN_ERROR;
    }
}

#define JANUS_TRY_PPR(PPR_API_CALL)            \
{                                              \
    ppr_error_type ppr_error = (PPR_API_CALL); \
    if (ppr_error != PPR_SUCCESS)              \
        return to_janus_error(ppr_error);      \
}

static ppr_error_type initialize_ppr_context(ppr_context_type *context)
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
    return ppr_initialize_context(settings, context);
}

janus_error janus_initialize(const char *sdk_path, const char *temp_path, const char *model_file)
{
    (void) model_file;
    (void) temp_path;
    const char *models = "/models/";
    const size_t models_path_len = strlen(sdk_path) + strlen(models);
    char *models_path = new char[models_path_len];
    snprintf(models_path, models_path_len, "%s%s", sdk_path, models);

    janus_error error = to_janus_error(ppr_initialize_sdk(models_path, my_license_id, my_license_key));
    free(models_path);

    if (error != JANUS_SUCCESS)
        return error;

    return to_janus_error(initialize_ppr_context(&ppr_context));
}

janus_error janus_finalize()
{
    janus_error error = to_janus_error(ppr_finalize_context(ppr_context));
    ppr_finalize_sdk();

    return error;
}

janus_error janus_allocate_template(janus_template *template_)
{
    *template_ = new janus_template_type();
    return JANUS_SUCCESS;
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

janus_image crop(const janus_image src, const janus_attributes *attributes)
{
    janus_image dst;
    size_t x = attributes->face_x < 0 ? 0 : attributes->face_x;
    size_t y = attributes->face_y < 0 ? 0 : attributes->face_y;
    dst.width = (x + attributes->face_width > src.width) ? src.width - x : attributes->face_width;
    dst.height = (y + attributes->face_height > src.height) ? src.height - y : attributes->face_height;
    dst.color_space = src.color_space;
    int channels = src.color_space == JANUS_BGR24 ? 3 : 1;

    dst.data = new janus_data[dst.width * dst.height * channels];
    const unsigned long dst_elements_per_row = dst.width * channels;
    const unsigned long src_elements_per_row = src.width * channels;

    for (size_t i=0; i<dst.height; i++)
        memcpy(dst.data + i*dst_elements_per_row, src.data + src_elements_per_row * (y + i) + channels*x, dst_elements_per_row);

    return dst;
}

struct sort_first_greater {
    bool operator()(const std::pair<float,janus_template_id> &left, const std::pair<float,janus_template_id> &right) {
        return left.first > right.first;
    }
    bool operator()(const std::pair<float,ppr_face_attributes_type> &left, const std::pair<float,ppr_face_attributes_type> &right) {
        return left.first > right.first;
    }
};

janus_error janus_detect(const janus_image image, janus_attributes *attributes_array, const size_t num_requested, size_t *num_actual)
{
    ppr_image_type ppr_image;
    JANUS_TRY_PPR(to_ppr_image(image, &ppr_image))

    ppr_face_list_type face_list;
    ppr_detect_faces(ppr_context, ppr_image, &face_list);

    *num_actual = face_list.length;
    if (face_list.length == 0)
        return JANUS_FAILURE_TO_DETECT;

    vector<pair<float, ppr_face_attributes_type> > face_confidences;
    for (size_t i=0; i<*num_actual; i++) {
        ppr_face_type face = face_list.faces[i];
        ppr_face_attributes_type face_attributes;
        JANUS_TRY_PPR(ppr_get_face_attributes(face, &face_attributes))
        face_confidences.push_back(pair<float, ppr_face_attributes_type>(face_attributes.confidence, face_attributes));
    }

    // Sort by confidence, descending
    sort(face_confidences.begin(), face_confidences.end(), sort_first_greater());

    size_t count = 0;
    for (size_t i=0; i<face_confidences.size(); i++) {
        attributes_array->face_x = face_confidences[i].second.position.x - face_confidences[i].second.dimensions.width/2;
        attributes_array->face_y = face_confidences[i].second.position.y - face_confidences[i].second.dimensions.height/2;
        attributes_array->face_width = face_confidences[i].second.dimensions.width;
        attributes_array->face_height = face_confidences[i].second.dimensions.height;
        attributes_array->detection_confidence = face_confidences[i].first;
        attributes_array++;
        if (++count >= num_requested)
            break;
    }

    attributes_array -= count;
    ppr_free_face_list(face_list);
    ppr_free_image(ppr_image);
    return JANUS_SUCCESS;
}

janus_error janus_augment(const janus_image image, janus_attributes *attributes, janus_template template_)
{
    janus_image cropped = crop(image, attributes);
    ppr_image_type ppr_image;
    to_ppr_image(cropped, &ppr_image);

    ppr_face_list_type face_list;
    ppr_detect_faces(ppr_context, ppr_image, &face_list);

    for (int i=0; i<face_list.length; i++) {
        ppr_face_type face = face_list.faces[i];

        int extractable;
        ppr_is_template_extractable(ppr_context, face, &extractable);
        if (extractable) {
            // Only extract a single face template
            ppr_extract_face_template(ppr_context, ppr_image, &face);
            break;
        }
    }

    template_->ppr_face_lists.push_back(face_list);

    free(cropped.data);
    ppr_free_image(ppr_image);

    return JANUS_SUCCESS;
}

janus_error janus_flatten_template(janus_template template_, janus_flat_template flat_template, size_t *bytes)
{
    ppr_flat_data_type flat_data;

    *bytes = 0;

    for (size_t i=0; i<template_->ppr_face_lists.size(); i++) {
        ppr_flatten_face_list(ppr_context, template_->ppr_face_lists[i], &flat_data);

        const size_t templateBytes = flat_data.length;

        if (*bytes + sizeof(size_t) + templateBytes > janus_max_template_size())
            break;

        memcpy(flat_template, &templateBytes, sizeof(templateBytes));
        flat_template += sizeof(templateBytes);
        *bytes += sizeof(templateBytes);

        memcpy(flat_template, flat_data.data, templateBytes);
        flat_template += templateBytes;
        *bytes += templateBytes;

        ppr_free_flat_data(flat_data);
    }

    return JANUS_SUCCESS;
}

janus_error janus_free_template(janus_template template_)
{
    for (size_t i=0; i<template_->ppr_face_lists.size(); i++) ppr_free_face_list(template_->ppr_face_lists[i]);
    template_->ppr_face_lists.clear();
    delete template_;
    return JANUS_SUCCESS;
}

size_t janus_max_template_size()
{
    return 33554432; // 32 MB
}

static int faceID = 0;

void ppr_unflatten(const janus_flat_template template_, const size_t template_bytes, ppr_gallery_type *gallery, const janus_template_id template_id = 0)
{
    janus_flat_template flat_template = template_;
    while (flat_template < template_ + template_bytes) {
        const size_t flat_template_bytes = *reinterpret_cast<size_t*>(flat_template);
        flat_template += sizeof(flat_template_bytes);

        ppr_flat_data_type flat_data;
        ppr_create_flat_data(flat_template_bytes, &flat_data);
        memcpy(flat_data.data, flat_template, flat_template_bytes);

        ppr_face_list_type face_list;
        ppr_unflatten_face_list(ppr_context, flat_data, &face_list);

        for (int i=0; i<face_list.length; i++) {
            ppr_face_type face = face_list.faces[i];
            int has_template;
            ppr_face_has_template(ppr_context, face, &has_template);

            if (!has_template)
                continue;

            ppr_add_face(ppr_context, gallery, face, (int)template_id, faceID++);
        }

        ppr_free_face_list(face_list);
        ppr_free_flat_data(flat_data);

        flat_template += flat_template_bytes;
    }
}

janus_error janus_verify(const janus_flat_template a, const size_t a_bytes, const janus_flat_template b, const size_t b_bytes, float *similarity)
{
    // Set the default similarity score to be a rejection score (for galleries that don't contain faces)
    *similarity = -1.5;

    ppr_gallery_type query_gallery;
    ppr_create_gallery(ppr_context, &query_gallery);

    ppr_unflatten(a, a_bytes, &query_gallery);

    ppr_gallery_type target_gallery;
    ppr_create_gallery(ppr_context, &target_gallery);

    ppr_unflatten(b, b_bytes, &target_gallery);

    ppr_similarity_matrix_type simmat;
    ppr_compare_galleries(ppr_context, query_gallery, target_gallery, &simmat);
    ppr_get_subject_similarity_score(ppr_context, simmat, 0, 0, similarity);

    ppr_free_gallery(query_gallery);
    ppr_free_gallery(target_gallery);
    ppr_free_similarity_matrix(simmat);

    if (*similarity != *similarity) // True for NaN
        return JANUS_UNKNOWN_ERROR;

    return JANUS_SUCCESS;
}

janus_error janus_write_gallery(const janus_flat_template *templates, const size_t *templates_bytes, const janus_template_id *template_ids, const size_t num_templates, janus_gallery_path gallery_path)
{
    std::ofstream file;
    file.open(gallery_path, ios::out | ios::binary);
    for (size_t i=0; i<num_templates; i++) {
        file.write((char*)&template_ids[i], sizeof(janus_template_id));
        file.write((char*)&templates_bytes[i], sizeof(size_t));
        file.write((char*)templates[i], templates_bytes[i]);
    }
    file.close();
    return JANUS_SUCCESS;
}

janus_error janus_open_gallery(janus_gallery_path gallery_path, janus_gallery *gallery)
{
    *gallery = new janus_gallery_type();
    JANUS_TRY_PPR(ppr_create_gallery(ppr_context, &(*gallery)->ppr_gallery))

    ifstream file;
    file.open(gallery_path, ios::in | ios::binary | ios::ate);
    const size_t bytes = file.tellg();
    file.seekg(0, ios::beg);
    janus_data *templates = new janus_data[bytes];
    file.read((char*)templates, bytes);
    file.close();

    janus_data *templates_ = templates;
    while (templates_ < templates + bytes) {
        janus_template_id template_id = *reinterpret_cast<janus_template_id*>(templates_);
        templates_ += sizeof(template_id);
        const size_t template_bytes = *reinterpret_cast<size_t*>(templates_);
        templates_ += sizeof(template_bytes);

        janus_flat_template flat_template = new janus_data[janus_max_template_size()];
        memcpy(flat_template, templates_, template_bytes);
        templates_ += template_bytes;

        ppr_unflatten(flat_template, template_bytes, &(*gallery)->ppr_gallery, template_id);
        delete[] flat_template;
    }
    delete[] templates;
    return JANUS_SUCCESS;
}

janus_error janus_close_gallery(janus_gallery gallery)
{
    ppr_free_gallery(gallery->ppr_gallery);
    delete gallery;
    return JANUS_SUCCESS;
}

janus_error janus_search(const janus_flat_template probe, const size_t probe_bytes, const janus_gallery gallery, size_t num_requested_returns, janus_template_id *template_ids, float *similarities, size_t *num_actual_returns)
{
    ppr_gallery_type probe_gallery;
    ppr_create_gallery(ppr_context, &probe_gallery);

    ppr_unflatten(probe, probe_bytes, &probe_gallery);

    ppr_similarity_matrix_type simmat;
    ppr_compare_galleries(ppr_context, probe_gallery, gallery->ppr_gallery, &simmat);

    ppr_id_list_type id_list;
    ppr_get_subject_id_list(ppr_context, gallery->ppr_gallery, &id_list);

    if (id_list.length < (int)num_requested_returns) *num_actual_returns = id_list.length;
    else                                        *num_actual_returns = num_requested_returns;

    vector<pair<float,janus_template_id> > scores;

    for (int i=0; i<id_list.length; i++) {
        int target_subject_id = id_list.ids[i];
        float score;
        ppr_get_subject_similarity_score(ppr_context, simmat, 0, target_subject_id, &score);
        scores.push_back(pair<float,janus_template_id>(score,target_subject_id));
    }

    ppr_free_id_list(id_list);
    ppr_free_gallery(probe_gallery);
    ppr_free_similarity_matrix(simmat);

    sort(scores.begin(), scores.end(), sort_first_greater());

    for (size_t i=0; i<*num_actual_returns; i++) {
        similarities[i] = scores[i].first;
        template_ids[i] = scores[i].second;
    }

    return JANUS_SUCCESS;
}
