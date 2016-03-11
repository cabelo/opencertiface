// This file is designed to have no dependencies outside the C++ Standard Library
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "iarpa_janus_io.h"

using namespace std;

#define ENUM_CASE(X) case JANUS_##X: return #X;
#define ENUM_COMPARE(X,Y) if (!strcmp(#X, Y)) return JANUS_##X;

const char *janus_error_to_string(janus_error error)
{
    switch (error) {
        ENUM_CASE(SUCCESS)
        ENUM_CASE(UNKNOWN_ERROR)
        ENUM_CASE(OUT_OF_MEMORY)
        ENUM_CASE(INVALID_SDK_PATH)
        ENUM_CASE(OPEN_ERROR)
        ENUM_CASE(READ_ERROR)
        ENUM_CASE(WRITE_ERROR)
        ENUM_CASE(PARSE_ERROR)
        ENUM_CASE(INVALID_IMAGE)
        ENUM_CASE(INVALID_VIDEO)
        ENUM_CASE(MISSING_TEMPLATE_ID)
        ENUM_CASE(MISSING_FILE_NAME)
        ENUM_CASE(NULL_ATTRIBUTES)
        ENUM_CASE(MISSING_ATTRIBUTES)
        ENUM_CASE(FAILURE_TO_DETECT)
        ENUM_CASE(FAILURE_TO_ENROLL)
        ENUM_CASE(NUM_ERRORS)
        ENUM_CASE(NOT_IMPLEMENTED)
    }
    return "UNKNOWN_ERROR";
}

janus_error janus_error_from_string(const char *error)
{
    ENUM_COMPARE(SUCCESS, error)
    ENUM_COMPARE(UNKNOWN_ERROR, error)
    ENUM_COMPARE(OUT_OF_MEMORY, error)
    ENUM_COMPARE(INVALID_SDK_PATH, error)
    ENUM_COMPARE(OPEN_ERROR, error)
    ENUM_COMPARE(READ_ERROR, error)
    ENUM_COMPARE(WRITE_ERROR, error)
    ENUM_COMPARE(PARSE_ERROR, error)
    ENUM_COMPARE(INVALID_IMAGE, error)
    ENUM_COMPARE(INVALID_VIDEO, error)
    ENUM_COMPARE(MISSING_TEMPLATE_ID, error)
    ENUM_COMPARE(MISSING_FILE_NAME, error)
    ENUM_COMPARE(NULL_ATTRIBUTES, error)
    ENUM_COMPARE(MISSING_ATTRIBUTES, error)
    ENUM_COMPARE(FAILURE_TO_DETECT, error)
    ENUM_COMPARE(FAILURE_TO_ENROLL, error)
    ENUM_COMPARE(NUM_ERRORS, error)
    ENUM_COMPARE(NOT_IMPLEMENTED, error)
    return JANUS_UNKNOWN_ERROR;
}

// For computing metrics
static vector<double> janus_initialize_template_samples;
static vector<double> janus_detection_samples;
static vector<double> janus_augment_samples;
static vector<double> janus_finalize_template_samples;
static vector<double> janus_finalize_gallery_samples;
static vector<double> janus_read_image_samples;
static vector<double> janus_free_image_samples;
static vector<double> janus_verify_samples;
static vector<double> janus_template_size_samples;
static vector<double> janus_gallery_size_samples;
static vector<double> janus_search_samples;
static int janus_missing_attributes_count = 0;
static int janus_failure_to_detect_count = 0;
static int janus_failure_to_enroll_count = 0;
static int janus_other_errors_count = 0;

static void _janus_add_sample(vector<double> &samples, double sample);

#ifndef JANUS_CUSTOM_ADD_SAMPLE

static void _janus_add_sample(vector<double> &samples, double sample)
{
    samples.push_back(sample);
}

#endif // JANUS_CUSTOM_ADD_SAMPLE

struct Rectangle
{
    double x, y, width, height;

    Rectangle()
        : x(0), y(0), width(0), height(0) {}
    Rectangle(double x_, double y_, double width_, double height_)
        : x(x_), y(y_), width(width_), height(height_) {}
    Rectangle(const Rectangle &other)
        : x(other.x), y(other.y), width(other.width), height(other.height) {}

    Rectangle& operator=(const Rectangle &rhs)
    {
        x = rhs.x;
        y = rhs.y;
        width = rhs.width;
        height = rhs.height;
        return *this;
    }

    Rectangle intersected(const Rectangle &other) const
    {
        double x_min = max(x, other.x);
        double x_max = min(x+width, other.x+other.width);
        if (x_max > x_min) {
            double y_min = max(y, other.y);
            double y_max = min(y+height, other.y+other.height);
            if (y_max > y_min)
                return Rectangle(x_min, y_min, x_max-x_min, y_max-y_min);
        }
        return Rectangle();
    }

    double area() const { return width * height; }
    double overlap (const Rectangle &other) const
    {
        const Rectangle intersection(intersected(other));
        return intersection.area() / (area() + other.area() - intersection.area());
    }
};

struct TemplateData
{
    vector<string> fileNames;
    vector<janus_template_id> templateIDs;
    map<janus_template_id, int> subjectIDLUT;
    vector<janus_attributes*> attributeLists;

    void release()
    {
        fileNames.clear();
        templateIDs.clear();
        subjectIDLUT.clear();
        for (size_t j=0; j<attributeLists.size(); j++)
            delete attributeLists[j];
        attributeLists.clear();
    }
};

struct TemplateIterator : public TemplateData
{
    size_t i;
    bool verbose;

    TemplateIterator(janus_metadata metadata, bool verbose)
        : i(0), verbose(verbose)
    {
        ifstream file(metadata);

        // Parse header
        string line;
        getline(file, line);
        istringstream attributeNames(line);
        string attributeName;
        getline(attributeNames, attributeName, ','); // TEMPLATE_ID
        getline(attributeNames, attributeName, ','); // SUBJECT_ID
        getline(attributeNames, attributeName, ','); // FILE_NAME
        vector<string> header;
        while (getline(attributeNames, attributeName, ','))
            header.push_back(attributeName);

        // Parse rows
        while (getline(file, line)) {
            istringstream attributeValues(line);
            string templateID, subjectID, fileName, attributeValue;
            getline(attributeValues, templateID, ',');
            getline(attributeValues, subjectID, ',');
            getline(attributeValues, fileName, ',');
            templateIDs.push_back(atoi(templateID.c_str()));
            subjectIDLUT.insert(pair<janus_template_id,int>(atoi(templateID.c_str()), atoi(subjectID.c_str())));
            fileNames.push_back(fileName);

            // Construct attribute list
            janus_attributes *attributes = new janus_attributes;
            for (int j=0; getline(attributeValues, attributeValue, ','); j++) {
                double value = attributeValue.empty() ? NAN : atof(attributeValue.c_str());
                if (header[j].compare(string("FRAME_RATE")) == 0)
                    attributes->frame_rate = value;
                else if (header[j].compare(string("FACE_X")) == 0)
                    attributes->face_x = value;
                else if (header[j].compare(string("FACE_Y")) == 0)
                    attributes->face_y = value;
                else if (header[j].compare(string("FACE_WIDTH")) == 0)
                    attributes->face_width = value;
                else if (header[j].compare(string("FACE_HEIGHT")) == 0)
                    attributes->face_height = value;
                else if (header[j].compare(string("RIGHT_EYE_X")) == 0)
                    attributes->right_eye_x = value;
                else if (header[j].compare(string("RIGHT_EYE_Y")) == 0)
                    attributes->right_eye_y = value;
                else if (header[j].compare(string("LEFT_EYE_X")) == 0)
                    attributes->left_eye_x = value;
                else if (header[j].compare(string("LEFT_EYE_Y")) == 0)
                    attributes->left_eye_y = value;
                else if (header[j].compare(string("NOSE_BASE_X")) == 0)
                    attributes->nose_base_x = value;
                else if (header[j].compare(string("NOSE_BASE_Y")) == 0)
                    attributes->nose_base_y = value;
                else if (header[j].compare(string("FACE_YAW")) == 0)
                    attributes->face_yaw = value;
                else if (header[j].compare(string("FOREHEAD_VISIBLE")) == 0)
                    attributes->forehead_visible = value;
                else if (header[j].compare(string("EYES_VISIBLE")) == 0)
                    attributes->eyes_visible = value;
                else if (header[j].compare(string("NOSE_MOUTH_VISIBLE")) == 0)
                    attributes->nose_mouth_visible = value;
                else if (header[j].compare(string("INDOOR")) == 0)
                    attributes->indoor = value;
                else if (header[j].compare(string("GENDER")) == 0)
                    attributes->gender = value;
                else if (header[j].compare(string("SKIN_TONE")) == 0)
                    attributes->skin_tone = value;
                else if (header[j].compare(string("AGE")) == 0)
                    attributes->age = value;
            }
            attributeLists.push_back(attributes);
        }
        if (verbose)
            fprintf(stderr, "\rEnrolling %zu/%zu", i, attributeLists.size());
    }

    TemplateData next()
    {
        TemplateData templateData;
        if (i >= attributeLists.size()) {
            fprintf(stderr, "\n");
        } else {
            const janus_template_id templateID = templateIDs[i];
            while ((i < attributeLists.size()) && (templateIDs[i] == templateID)) {
                templateData.templateIDs.push_back(templateIDs[i]);
                templateData.fileNames.push_back(fileNames[i]);
                janus_attributes *attributes = new janus_attributes;
                memcpy(attributes, attributeLists[i], sizeof(janus_attributes));
                templateData.attributeLists.push_back(attributes);
                i++;
            }
            if (verbose)
                fprintf(stderr, "\rEnrolling %zu/%zu", i, attributeLists.size());
        }
        return templateData;
    }

    static janus_error create(const char *data_path, const TemplateData templateData, janus_template *template_, janus_template_id *templateID, bool verbose)
    {
        clock_t start = clock();
        JANUS_CHECK(janus_allocate_template(template_))
        _janus_add_sample(janus_initialize_template_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);

        const size_t num_requested_detections = 10;
        for (size_t i=0; i<templateData.templateIDs.size(); i++) {
            janus_attributes *attributes = new janus_attributes;
            memcpy(attributes, templateData.attributeLists[i], sizeof(janus_attributes));

            janus_image image;

            start = clock();
            JANUS_ASSERT(janus_read_image((data_path + templateData.fileNames[i]).c_str(), &image))
            _janus_add_sample(janus_read_image_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);

            size_t num_actual_detections = 0;
            janus_attributes *attributes_array = new janus_attributes[num_requested_detections];

            start = clock();
            janus_error error = janus_detect(image, attributes_array, num_requested_detections, &num_actual_detections);
            _janus_add_sample(janus_detection_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
            if (error == JANUS_FAILURE_TO_DETECT || num_actual_detections == 0) {
                janus_failure_to_detect_count++;
                if (verbose)
                    printf("Failure to detect face in %s\n", templateData.fileNames[i].c_str());
            } else if (error != JANUS_SUCCESS) {
                janus_other_errors_count++;
                printf("Warning: %s on: %s\n", janus_error_to_string(error),templateData.fileNames[i].c_str());
            } else {
                double overlap = 0.5;
                Rectangle truth(attributes->face_x, attributes->face_y, attributes->face_width, attributes->face_height);
                for (size_t j=0; j<min(num_requested_detections, num_actual_detections); j++) {
                    Rectangle detected(attributes_array[j].face_x, attributes_array[j].face_y, attributes_array[j].face_width, attributes_array[j].face_height);
                    if (detected.overlap(truth) > overlap) {
                        overlap = detected.overlap(truth);
                        attributes->face_x = detected.x;
                        attributes->face_y = detected.y;
                        attributes->face_width = detected.width;
                        attributes->face_height = detected.height;
                    }
                }
                if (overlap == 0.5) {
                    janus_failure_to_detect_count++;
                    if (verbose)
                        printf("Failure to detect face in %s\n", templateData.fileNames[i].c_str());
                }
            }
            delete[] attributes_array;

            start = clock();
            error = janus_augment(image, attributes, *template_);
            if (error == JANUS_MISSING_ATTRIBUTES) {
                janus_missing_attributes_count++;
                if (verbose)
                    printf("Missing attributes for: %s\n", templateData.fileNames[i].c_str());
            } else if (error == JANUS_FAILURE_TO_ENROLL) {
                janus_failure_to_enroll_count++;
                if (verbose)
                    printf("Failure to enroll: %s\n", templateData.fileNames[i].c_str());
            } else if (error != JANUS_SUCCESS) {
                janus_other_errors_count++;
                printf("Warning: %s on: %s\n", janus_error_to_string(error),templateData.fileNames[i].c_str());
            }
            _janus_add_sample(janus_augment_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);

            start = clock();
            janus_free_image(image);
            _janus_add_sample(janus_free_image_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);

            delete attributes;
        }

        *templateID = templateData.templateIDs[0];
        return JANUS_SUCCESS;
    }

    ~TemplateIterator() { release(); }
};

janus_error janus_create_template(const char *data_path, janus_metadata metadata, janus_template *template_, janus_template_id *template_id)
{
    TemplateIterator ti(metadata, false);
    return TemplateIterator::create(data_path, ti.next(), template_, template_id, false);
}

#ifndef JANUS_CUSTOM_CREATE_TEMPLATES

janus_error janus_create_templates(const char *data_path, janus_metadata metadata, const char *gallery_file, int verbose)
{
    TemplateIterator ti(metadata, true);
    janus_template template_;
    janus_template_id templateID;
    TemplateData templateData = ti.next();
    std::ofstream file;
    file.open(gallery_file, std::ios::out | std::ios::binary | std::ios::ate);
    while (!templateData.templateIDs.empty()) {
        JANUS_CHECK(TemplateIterator::create(data_path, templateData, &template_, &templateID, verbose))
        janus_flat_template flat_template_ = new janus_data[janus_max_template_size()];
        size_t bytes;
        JANUS_CHECK(janus_flatten_template(template_, flat_template_, &bytes))
        JANUS_CHECK(janus_free_template(template_))
        file.write((char*)&templateID, sizeof(templateID));
        file.write((char*)&bytes, sizeof(bytes));
        file.write((char*)flat_template_, bytes);

        delete[] flat_template_;
        templateData.release();
        templateData = ti.next();
    }
    file.close();
    return JANUS_SUCCESS;
}

#endif // JANUS_CUSTOM_CREATE_TEMPLATES

#ifndef JANUS_CUSTOM_CREATE_GALLERY

janus_error janus_create_gallery(const char *data_path, janus_metadata metadata, janus_gallery_path gallery_path, int verbose)
{
    vector<janus_flat_template> templates;
    vector<size_t> template_bytes;
    vector<janus_template_id> template_ids;
    size_t num_templates = 0;

    TemplateIterator ti(metadata, true);
    janus_template template_;
    janus_template_id templateID;
    TemplateData templateData = ti.next();
    while (!templateData.templateIDs.empty()) {
        JANUS_CHECK(TemplateIterator::create(data_path, templateData, &template_, &templateID, verbose))

        size_t bytes;
        janus_data *buffer = new janus_data[janus_max_template_size()];
        JANUS_ASSERT(janus_flatten_template(template_, buffer, &bytes))
        JANUS_ASSERT(janus_free_template(template_))

        janus_flat_template flat_template;
        flat_template = new janus_data[bytes];
        memcpy(flat_template, buffer, bytes);
        delete[] buffer;

        templates.push_back(flat_template);
        template_bytes.push_back(bytes);
        template_ids.push_back(templateID);
        num_templates++;

        templateData.release();
        templateData = ti.next();
    }
    JANUS_ASSERT(janus_write_gallery(&templates[0], &template_bytes[0], &template_ids[0], num_templates, gallery_path))

    for (size_t i=0; i<templates.size(); i++)
        delete[] templates[i];

    return JANUS_SUCCESS;
}

#endif // JANUS_CUSTOM_CREATE_GALLERY

struct FlatTemplate
{
    struct Data {
        janus_flat_template flat_template;
        size_t bytes, ref_count;
        janus_error error;
    } *data;

    FlatTemplate()
    {
        data = NULL;
    }
    
    FlatTemplate(janus_template template_)
    {
        data = new Data();
        data->ref_count = 1;

        janus_data *buffer = new janus_data[janus_max_template_size()];

        const clock_t start = clock();
        data->error = janus_flatten_template(template_, buffer, &data->bytes);
        JANUS_ASSERT(janus_free_template(template_))
        _janus_add_sample(janus_finalize_template_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
        _janus_add_sample(janus_template_size_samples, data->bytes / 1024.0);

        data->flat_template = new janus_data[data->bytes];
        memcpy(data->flat_template, buffer, data->bytes);
        delete[] buffer;
    }

    FlatTemplate(size_t bytes, janus_flat_template flat_template)
    {
        data = new Data();
        data->ref_count = 1;
        data->bytes = bytes;
        data->flat_template = flat_template;
        data->error = JANUS_SUCCESS;
    }

    FlatTemplate(const FlatTemplate& other)
    {
        *this = other;
    }

    FlatTemplate& operator=(const FlatTemplate& rhs)
    {
        data = rhs.data;
        if (data) data->ref_count++;
        return *this;
    }

    ~FlatTemplate()
    {
        if (data) {
            data->ref_count--;
            if (data->ref_count == 0) {
                delete[] data->flat_template;
                delete data;
            }
        }
    }

    janus_error compareTo(const FlatTemplate &other, float *similarity) const
    {
        const clock_t start = clock();
        janus_error error = janus_verify(data->flat_template, data->bytes, other.data->flat_template, other.data->bytes, similarity);
        _janus_add_sample(janus_verify_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
        return error;
    }
};

janus_error janus_write_matrix(void *data, int rows, int columns, int is_mask, janus_metadata target, janus_metadata query, janus_matrix matrix)
{
    ofstream stream(matrix, ios::out | ios::binary);
    stream << "S2\n"
           << target << '\n'
           << query << '\n'
           << 'M' << (is_mask ? 'B' : 'F') << ' '
           << rows << ' ' << columns << ' ';
    int endian = 0x12345678;
    stream.write((const char*)&endian, 4);
    stream << '\n';
    stream.write((const char*)data, rows * columns * (is_mask ? 1 : 4));
    return JANUS_SUCCESS;
}

janus_data* janus_read_templates(const char *template_file, size_t *bytes)
{
    ifstream file;
    file.open(template_file, ios::in | ios::binary | ios::ate);
    *bytes = file.tellg();
    file.seekg(0, ios::beg);
    janus_data *templates = new janus_data[*bytes];
    file.read((char*)templates, *bytes);
    file.close();
    return templates;
}

janus_error janus_evaluate_search(janus_gallery_path target, const char *query, janus_metadata target_metadata, janus_metadata query_metadata, janus_matrix simmat, janus_matrix mask, const char *candidate_lists, size_t num_requested_returns)
{
    janus_gallery target_gallery;
    JANUS_ASSERT(janus_open_gallery(target, &target_gallery))
    TemplateData targetMetadata = TemplateIterator(target_metadata, false);
    TemplateData queryMetadata = TemplateIterator(query_metadata, false);
    size_t query_size = queryMetadata.templateIDs.size();
    float *similarity_matrix = new float[query_size * num_requested_returns];
    unsigned char *truth = new unsigned char[query_size * num_requested_returns];

    ofstream candidate_lists_stream;
    candidate_lists_stream.open(candidate_lists);
    candidate_lists_stream << "SEARCH_TEMPLATE_ID CANDIDATE_RANK ENROLL_TEMPLATE_ID ENROLL_TEMPLATE_SIZE_BYTES SEARCH_TEMPLATE_SIZE_BYTES RETCODE SIMILARITY_SCORE" << endl;

    // Read in query template file
    size_t query_bytes;
    janus_data *query_templates = janus_read_templates(query, &query_bytes);
    janus_data *q_templates = query_templates;

    int num_queries = 0;
    while (q_templates < query_templates + query_bytes) {
        janus_template_id *template_ids = new janus_template_id[num_requested_returns];
        float *similarities = new float[num_requested_returns];
        size_t num_actual_returns;

        janus_template_id query_template_id = *reinterpret_cast<janus_template_id*>(q_templates);
        q_templates += sizeof(query_template_id);
        const size_t query_template_bytes = *reinterpret_cast<size_t*>(q_templates);
        q_templates += sizeof(query_template_bytes);
        janus_flat_template query_template_flat = new janus_data[janus_max_template_size()];
        memcpy(query_template_flat, q_templates, query_template_bytes);
        q_templates += query_template_bytes;

        clock_t start = clock();
        JANUS_CHECK(janus_search(query_template_flat, query_template_bytes, target_gallery, num_requested_returns, template_ids, similarities, &num_actual_returns))
        _janus_add_sample(janus_search_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
        _janus_add_sample(janus_template_size_samples, query_template_bytes / 1024.0);

        // Write matrix of size num_queries*num_requested returns
        if (num_actual_returns < num_requested_returns) {
            memcpy(similarity_matrix, similarities, sizeof(float)*num_actual_returns);

            for (size_t i=num_actual_returns; i<num_requested_returns; i++) {
                similarity_matrix[i] = -std::numeric_limits<float>::max();
            }
            similarity_matrix += num_requested_returns;
        } else if (num_actual_returns == num_requested_returns) {
            memcpy(similarity_matrix, similarities, sizeof(float)*num_actual_returns);
            similarity_matrix += num_actual_returns;
        } else {
	  std::cerr << "Error: Number of search results returned (" << num_actual_returns << ") is greater than number requested ("
		    << num_requested_returns << "). Likely memory error triggering undefined behavior. Exiting early with error." << std::endl;
	  return JANUS_UNKNOWN_ERROR;
	}

        for (size_t j=0; j<num_requested_returns; j++) {
            if (j<num_actual_returns) {
                truth[num_queries*num_requested_returns+j] = (queryMetadata.subjectIDLUT[query_template_id] == targetMetadata.subjectIDLUT[template_ids[j]] ? 0xff : 0x7f);
                candidate_lists_stream << query_template_id << " " << j << " " << template_ids[j] << " " << 1
                       << " " << query_template_bytes << " " << 0 << " " << similarities[j] << endl;
            } else {
                truth[num_queries*num_requested_returns+j] = 0x00;
                candidate_lists_stream << "0 0 0 0 0 0 0" << endl;
            }
        }
        num_queries++;
        delete[] query_template_flat;
        delete[] template_ids;
        delete[] similarities;
    }
    candidate_lists_stream.close();
    similarity_matrix -= num_queries*num_requested_returns;
    JANUS_CHECK(janus_write_matrix(similarity_matrix, num_queries, num_requested_returns, false, target_metadata, query_metadata, simmat))
    delete[] similarity_matrix;
    JANUS_CHECK(janus_write_matrix(truth, num_queries, num_requested_returns, true, target_metadata, query_metadata, mask))
    delete[] truth;
    return JANUS_SUCCESS;
}

janus_error janus_evaluate_verify(const char *target, const char *query, janus_metadata target_metadata, janus_metadata query_metadata, janus_matrix simmat, janus_matrix mask)
{
    TemplateData targetMetadata = TemplateIterator(target_metadata, false);
    TemplateData queryMetadata = TemplateIterator(query_metadata, false);
    size_t target_size = targetMetadata.templateIDs.size();
    size_t query_size = queryMetadata.templateIDs.size();
    float *similarity_matrix = new float[query_size * target_size];
    unsigned char *truth = new unsigned char[query_size * target_size];

    // Read in query template file
    size_t query_bytes;
    janus_data *query_templates = janus_read_templates(query, &query_bytes);
    janus_data *q_templates = query_templates;

    // Read in target template file
    size_t target_bytes;
    janus_data *target_templates = janus_read_templates(target, &target_bytes);
    janus_data *t_templates = target_templates;

    int num_queries = 0;
    int num_targets = 0;
    while (q_templates < query_templates + query_bytes) {
        janus_template_id query_template_id = *reinterpret_cast<janus_template_id*>(q_templates);
        q_templates += sizeof(query_template_id);
        const size_t query_template_bytes = *reinterpret_cast<size_t*>(q_templates);
        q_templates += sizeof(query_template_bytes);
        janus_flat_template query_template_flat = new janus_data[janus_max_template_size()];
        memcpy(query_template_flat, q_templates, query_template_bytes);
        q_templates += query_template_bytes;
        _janus_add_sample(janus_template_size_samples, query_template_bytes / 1024.0);

        while (t_templates < target_templates + target_bytes) {
            janus_template_id target_template_id = *reinterpret_cast<janus_template_id*>(t_templates);
            t_templates += sizeof(target_template_id);
            const size_t target_template_bytes = *reinterpret_cast<size_t*>(t_templates);
            t_templates += sizeof(target_template_bytes);
            janus_flat_template target_template_flat = new janus_data[janus_max_template_size()];
            memcpy(target_template_flat, t_templates, target_template_bytes);
            t_templates += target_template_bytes;
            if (num_queries == 0)
                _janus_add_sample(janus_template_size_samples, target_template_bytes / 1024.0);

            float similarity;
            clock_t start = clock();
            JANUS_CHECK(janus_verify(query_template_flat, query_template_bytes, target_template_flat, target_template_bytes, &similarity))
            _janus_add_sample(janus_verify_samples, 1000.0 * (clock() - start) / CLOCKS_PER_SEC);
            *similarity_matrix = similarity;
            similarity_matrix += 1;
            *truth = (queryMetadata.subjectIDLUT[query_template_id] == targetMetadata.subjectIDLUT[target_template_id] ? 0xff : 0x7f);
            truth += 1;
            if (num_queries == 0)
                num_targets++;
            delete[] target_template_flat;
        }
    t_templates -= target_bytes;
    delete[] query_template_flat;
    num_queries++;
    }
    similarity_matrix -= num_targets*num_queries;
    truth -= num_targets*num_queries;
    JANUS_CHECK(janus_write_matrix(similarity_matrix, num_queries, num_targets, false, target_metadata, query_metadata, simmat))
    JANUS_CHECK(janus_write_matrix(truth, num_queries, num_targets, true, target_metadata, query_metadata, mask))
    delete[] similarity_matrix;
    delete[] truth;
    delete[] query_templates;
    delete[] target_templates;
    return JANUS_SUCCESS;
}

janus_error janus_verify_pairwise(const char *comparisons_file, const char *templates_file, janus_metadata template_metadata, janus_matrix simmat, janus_matrix mask, const char *match_scores)
{
    TemplateData metadata = TemplateIterator(template_metadata, false);
    vector<float> similarities;
    vector<unsigned char> truth;

    // Read in comparison templates
    size_t bytes;
    janus_data *templates = janus_read_templates(templates_file, &bytes);
    janus_data *templates_ = templates;

    map<janus_template_id, FlatTemplate> template_map;
    while (templates_ < templates + bytes) {
        janus_template_id template_id = *reinterpret_cast<janus_template_id*>(templates_);
        templates_ += sizeof(template_id);
        size_t template_bytes = *reinterpret_cast<size_t*>(templates_);
        templates_ += sizeof(template_bytes);
        janus_flat_template temp = new janus_data[template_bytes];
        memcpy(temp, templates_, template_bytes);
        templates_ += template_bytes;
        _janus_add_sample(janus_template_size_samples, template_bytes / 1024.0);

        template_map.insert(pair<janus_template_id,FlatTemplate>(template_id, FlatTemplate(template_bytes, temp)));
    }

    ofstream output;
    output.open(match_scores);
    output << "ENROLL_TEMPLATE_ID VERIF_TEMPLATE_ID ENROLL_TEMPLATE_SIZE_BYTES VERIF_TEMPLATE_SIZE_BYTES RETCODE SIMILARITY_SCORE" << endl;

    string line;
    ifstream file(comparisons_file);
    while(getline(file, line)) {
        janus_template_id enroll_id, verif_id;
        string value;
        istringstream template_ids(line);
        getline(template_ids, value, ',');
        enroll_id = atoi(value.c_str());
        getline(template_ids, value, ',');
        verif_id = atoi(value.c_str());

        // compare templates and record result
        float similarity;
        janus_error error = template_map[enroll_id].compareTo(template_map[verif_id], &similarity);
        output << enroll_id << " " << verif_id << " " << template_map[enroll_id].data->bytes
               << " " << template_map[verif_id].data->bytes << " " << error << " " << (error ? -1 : similarity) << endl;
        similarities.push_back(similarity);
        truth.push_back(metadata.subjectIDLUT[enroll_id] == metadata.subjectIDLUT[verif_id] ? 0xff : 0x7f);
    }
    output.close();

    JANUS_CHECK(janus_write_matrix(&similarities[0], (int)similarities.size(), 1, false, template_metadata, template_metadata, simmat))
    JANUS_CHECK(janus_write_matrix(&truth[0], (int)truth.size(), 1, true, template_metadata, template_metadata, mask))

    return JANUS_SUCCESS;
}

static janus_metric calculateMetric(const vector<double> &samples)
{
    janus_metric metric;
    metric.count = samples.size();

    if (metric.count > 0) {
        metric.mean = 0;
        for (size_t i=0; i<samples.size(); i++)
            metric.mean += samples[i];
        metric.mean /= samples.size();

        metric.stddev = 0;
        for (size_t i=0; i<samples.size(); i++)
            metric.stddev += pow(samples[i] - metric.mean, 2.0);
        metric.stddev = sqrt(metric.stddev / samples.size());
    } else {
        metric.mean = std::numeric_limits<double>::quiet_NaN();
        metric.stddev = std::numeric_limits<double>::quiet_NaN();
    }

    return metric;
}

janus_metrics janus_get_metrics()
{
    janus_metrics metrics;
    metrics.janus_initialize_template_speed = calculateMetric(janus_initialize_template_samples);
    metrics.janus_detection_speed           = calculateMetric(janus_detection_samples);
    metrics.janus_augment_speed             = calculateMetric(janus_augment_samples);
    metrics.janus_finalize_template_speed   = calculateMetric(janus_finalize_template_samples);
    metrics.janus_read_image_speed          = calculateMetric(janus_read_image_samples);
    metrics.janus_free_image_speed          = calculateMetric(janus_free_image_samples);
    metrics.janus_verify_speed              = calculateMetric(janus_verify_samples);
    metrics.janus_gallery_size_speed        = calculateMetric(janus_gallery_size_samples);
    metrics.janus_finalize_gallery_speed    = calculateMetric(janus_finalize_gallery_samples);
    metrics.janus_search_speed              = calculateMetric(janus_search_samples);
    metrics.janus_template_size             = calculateMetric(janus_template_size_samples);
    metrics.janus_missing_attributes_count  = janus_missing_attributes_count;
    metrics.janus_failure_to_detect_count   = janus_failure_to_detect_count;
    metrics.janus_failure_to_enroll_count   = janus_failure_to_enroll_count;
    metrics.janus_other_errors_count        = janus_other_errors_count;
    return metrics;
}

static void printMetric(const char *name, janus_metric metric, bool speed = true)
{
    if (metric.count > 0)
        printf("%s\t%.2g\t%.2g\t%s\t%.2g\n", name, metric.mean, metric.stddev, speed ? "ms" : "KB", double(metric.count));
}

void janus_print_metrics(janus_metrics metrics)
{
    printf(     "API Symbol               \tMean\tStdDev\tUnits\tCount\n");
    printMetric("janus_initialize_template", metrics.janus_initialize_template_speed);
    printMetric("janus_detect             ", metrics.janus_detection_speed);
    printMetric("janus_augment            ", metrics.janus_augment_speed);
    printMetric("janus_finalize_template  ", metrics.janus_finalize_template_speed);
    printMetric("janus_read_image         ", metrics.janus_read_image_speed);
    printMetric("janus_free_image         ", metrics.janus_free_image_speed);
    printMetric("janus_verify             ", metrics.janus_verify_speed);
    printMetric("janus_gallery_size       ", metrics.janus_gallery_size_speed);
    printMetric("janus_finalize_gallery   ", metrics.janus_finalize_gallery_speed);
    printMetric("janus_search             ", metrics.janus_search_speed);
    printMetric("janus_flat_template      ", metrics.janus_template_size, false);
    printf("\n\n");
    printf("janus_error             \tCount\n");
    printf("JANUS_MISSING_ATTRIBUTES\t%d\n", metrics.janus_missing_attributes_count);
    printf("JANUS_FAILURE_TO_DETECT \t%d\n", metrics.janus_failure_to_detect_count);
    printf("JANUS_FAILURE_TO_ENROLL \t%d\n", metrics.janus_failure_to_enroll_count);
    printf("All other errors        \t%d\n", metrics.janus_other_errors_count);
}
