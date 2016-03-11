#!/bin/bash
: '
The purpose of this script is to produce ROC, DET and CMC curves for each split of CS0 using the evaluation utilites under janus/src/utils
Utility: Description
janus_create_templates: Enrolls a janus_metadata file, outputs templates to file on disk.  This output is used in janus_evaluate_verify and for the query gallery of janus_evaluate_search.
janus_create_gallery: Enrolls a janus_metadata file, outputs a janus_flat_gallery to file on disk.  This output is used as the target gallery of janus_evaluate_search.
janus_evaluate_verify: Perform 1:1 verification on two galleries.  Output matrices are used to generate ROC/DET curves.
janus_evaluate_search: Perform 1:n search on two galleries.  Output matrices are used to generate CMC curve. 

Please email results to noblis@libjanus.org
The T&E team will use this output as a reference while reproducing results.
'
# Set output file name (no extension)
PERFORMER="performer_name"
SDK_PATH="/usr/local/"
CS0_DIR="/path/to/CS0/"
TEMP_PATH=$TMPDIR
RESULTS="./results"
ALGORITHM=""

plot () {
	if [ "$1" = "*" ]; then
		SPLIT=""
	else
		SPLIT="split$1"
	fi
	if type br &> /dev/null; then
		for PROTOCOL in A B
		do	
			br -plot $RESULTS/split${1}/verify_${1}_${PROTOCOL}.csv $RESULTS/$SPLIT/verify_${SPLIT}_${PROTOCOL}.pdf
			br -plot $RESULTS/split${1}/search_${1}_${PROTOCOL}.csv $RESULTS/$SPLIT/search_${SPLIT}_${PROTOCOL}.pdf
		done
		br -plot $RESULTS/split${1}/verify_${1}_A.csv $RESULTS/split${1}/verify_${1}_B.csv $RESULTS/${SPLIT}/verify_${SPLIT}.pdf
		br -plot $RESULTS/split$1/search_${1}_A.csv $RESULTS/split${1}/search_${1}_B.csv $RESULTS/${SPLIT}/search_${SPLIT}.pdf
		if type pdftk &> /dev/null; then
			for PROTOCOL in A B
			do
				pdftk A=$RESULTS/$SPLIT/verify_${SPLIT}_${PROTOCOL}.pdf B=$RESULTS/$SPLIT/search_${SPLIT}_${PROTOCOL}.pdf cat A1-5 B6 A7-8 output $RESULTS/$SPLIT/${SPLIT}_${PROTOCOL}_results.pdf
				rm $RESULTS/$SPLIT/verify_${SPLIT}_${PROTOCOL}.pdf
				rm $RESULTS/$SPLIT/search_${SPLIT}_${PROTOCOL}.pdf
			done
			pdftk A=$RESULTS/$SPLIT/verify_${SPLIT}.pdf B=$RESULTS/$SPLIT/search_${SPLIT}.pdf cat A1-5 B6 A7-8 output $RESULTS/${SPLIT}/results_${SPLIT}_AB.pdf
			rm $RESULTS/$SPLIT/verify_${SPLIT}.pdf
			rm $RESULTS/$SPLIT/search_${SPLIT}.pdf
		fi
		rm $RESULTS/${SPLIT}/*.R
	fi
}
# Evaluate CS0
if [ ! -d $RESULTS ]; then
	mkdir $RESULTS
fi
for i in `seq 1 10`;
do
	mkdir ${RESULTS}/split${i}
	for PROTOCOL in A B
	do
		# Enroll galleries using "janus_create_templates" for use in "janus_evaluate_verify"
		echo janus_create_templates probe > $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		janus_create_templates $SDK_PATH $TEMP_PATH $CS0_DIR $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_probe.csv $RESULTS/test_${i}_${PROTOCOL}_probe_templates.gal -algorithm $ALGORITHM >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		echo janus_create_templates gallery >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		janus_create_templates $SDK_PATH $TEMP_PATH $CS0_DIR $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_gal.csv $RESULTS/test_${i}_${PROTOCOL}_gal_templates.gal -algorithm $ALGORITHM >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt

		# Evaluate protocol A verification using "janus_evaluate_verify"
		echo janus_evaluate_verify >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		janus_evaluate_verify $SDK_PATH $TEMP_PATH $RESULTS/test_${i}_${PROTOCOL}_gal_templates.gal $RESULTS/test_${i}_${PROTOCOL}_probe_templates.gal $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_gal.csv $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_probe.csv $RESULTS/split${i}/verify_${i}_${PROTOCOL}.mtx $RESULTS/split${i}/verify_${i}_${PROTOCOL}.mask -algorithm $ALGORITHM >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt

		# Enroll target gallery using "janus_create_gallery" for use in "janus_evaluate_search"
		echo janus_create_gallery >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		janus_create_gallery $SDK_PATH $TEMP_PATH $CS0_DIR $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_gal.csv $RESULTS/test_${i}_${PROTOCOL}_gal_flat.gal -algorithm $ALGORITHM >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt

		#Evaluate protocol A search using "janus_evaluate_search"
		echo janus_evaluate_search >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt
		janus_evaluate_search $SDK_PATH $TEMP_PATH $RESULTS/test_${i}_${PROTOCOL}_gal_flat.gal $RESULTS/test_${i}_${PROTOCOL}_probe_templates.gal $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_gal.csv $CS0_DIR/protocol/split${i}/test_${i}_${PROTOCOL}_probe.csv $RESULTS/split${i}/search_${i}_${PROTOCOL}.mtx $RESULTS/split${i}/search_${i}_${PROTOCOL}.mask 150 -algorithm $ALGORITHM >> $RESULTS/split${i}/split${i}_${PROTOCOL}_log.txt

		# Evaluate output matrices using OpenBR
		if type br &> /dev/null; then
			br -eval $RESULTS/split${i}/verify_${i}_${PROTOCOL}.mtx $RESULTS/split${i}/verify_${i}_${PROTOCOL}.mask $RESULTS/split${i}/verify_${i}_${PROTOCOL}.csv
			br -eval $RESULTS/split${i}/search_${i}_${PROTOCOL}.mtx $RESULTS/split${i}/search_${i}_${PROTOCOL}.mask $RESULTS/split${i}/search_${i}_${PROTOCOL}.csv
		fi
	done
	rm $RESULTS/*.gal

	# Plot results for protocols A & B with OpenBR
	plot ${i}
done

# Plot results across all splits
plot "*"
# Package results
cd $RESULTS
tar -cvzf results_${PERFORMER}_${ALGORITHM}.tar.gz split* *.pdf

# OPTIONAL
# email results with "mutt" http://www.guckes.net/Mutt/install.php3
# apt-get install mutt
# echo "CS0 results" | mutt -a results_${PERFORMER}_${ALGORITHM}.tar.gz -s "CS0 results" -- noblis@libjanus.org