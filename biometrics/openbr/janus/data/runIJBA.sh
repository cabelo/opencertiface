#!/bin/bash
: '
This script evaluates the IJB-A 1:1 and 1:n protocols and outputs results to the standard format defined at http://www.nist.gov/itl/iad/ig/facechallenges.cfm 
Utility: Description
janus_create_templates: Enrolls a janus_metadata file, outputs templates to file on disk.  This output is used in janus_evaluate_verify and for the query gallery of janus_evaluate_search.
janus_create_gallery: Enrolls a janus_metadata file, outputs a janus_flat_gallery to file on disk.  This output is used as the target gallery of janus_evaluate_search.
janus_verify_pairwise: Perform a list of 1:1 verifications on a templates gallery.  Output matrices are used to generate ROC/DET curves. Output .matches file is used for submission to NIST.
janus_evaluate_search: Perform 1:n search on two galleries.  Output matrices are used to generate CMC curve. Output .candidate_lists file is used for submission to NIST.
'

# edit these variables
PERFORMER="performer"
SDK_PATH="/usr/local/"
IMG_DIR="/path/to/images/and/frames/"
VERIFY_PROTOCOL="/path/to/IJB-A/1to1/"
SEARCH_PROTOCOL="/path/to/IJB-A/1toN/"
TEMP_PATH=$TMPDIR
RESULTS="./results"
ALGORITHM=""
NUM_RETURNS=20
VERSION=1

VERIFY_DIR=${RESULTS}/IJB-A_11_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`
SEARCH_DIR=${RESULTS}/IJB-A_1N_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`

# Evaluate IJB-A
if [ ! -d $RESULTS ]; then
	mkdir $RESULTS
	mkdir $VERIFY_DIR
	mkdir $SEARCH_DIR
fi

for i in `seq 1 10`;
do
	mkdir ${VERIFY_DIR}/split${i}
	mkdir ${SEARCH_DIR}/split${i}

	# IJB-A 1:1 
	janus_create_templates $SDK_PATH $TEMP_PATH $IMG_DIR ${VERIFY_PROTOCOL}/split${i}/verify_metadata_${i}.csv ${RESULTS}/verify_templates_${i}.gal -algorithm $ALGORITHM
	janus_verify_pairwise $SDK_PATH $TEMP_PATH ${VERIFY_PROTOCOL}/split${i}/verify_comparisons_${i}.csv ${RESULTS}/verify_templates_${i}.gal ${VERIFY_PROTOCOL}/split${i}/verify_metadata_${i}.csv ${RESULTS}/verify_${i}.mtx ${RESULTS}/verify_${i}.mask ${VERIFY_DIR}/split${i}/split${i}.matches -algorithm $ALGORITHM

	# IJB-A 1:N 
	janus_create_templates $SDK_PATH $TEMP_PATH $IMG_DIR ${SEARCH_PROTOCOL}/split${i}/search_probe_${i}.csv ${RESULTS}/search_probe_${i}.gal -algorithm $ALGORITHM
	janus_create_gallery $SDK_PATH $TEMP_PATH $IMG_DIR ${SEARCH_PROTOCOL}/split${i}/search_gallery_${i}.csv ${RESULTS}/search_gallery_${i}.gal -algorithm $ALGORITHM
	janus_evaluate_search $SDK_PATH $TEMP_PATH ${RESULTS}/search_gallery_${i}.gal ${RESULTS}/search_probe_${i}.gal ${SEARCH_PROTOCOL}/split${i}/search_gallery_${i}.csv ${SEARCH_PROTOCOL}/split${i}/search_probe_${i}.csv ${RESULTS}/search_${i}.mtx ${RESULTS}/search_${i}.mask ${SEARCH_DIR}/split${i}/split${i}.candidate_lists ${NUM_RETURNS} -algorithm $ALGORITHM
done

cd $RESULTS
tar -cvzf IJB-A_11_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`.tar.gz IJB-A_11_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`/split*
tar -cvzf IJB-A_1N_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`.tar.gz IJB-A_1N_${PERFORMER}_${ALGORITHM}_${VERSION}_`date +%Y-%m-%d`/split*
