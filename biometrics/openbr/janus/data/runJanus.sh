#!/bin/bash
BR="/usr/local"
CS0_DIR="/path/to/CS0/"
GALLERY="/path/to/galleries/"
MATRIX="/path/to/matrices"
RESULTS="/path/to/results"

# enroll protocol A and B
janus_enroll $BR $CS0_DIR $CS0_DIR/protocol/galleryA.csv $GALLERY/galleryA.gal
janus_enroll $BR $CS0_DIR $CS0_DIR/protocol/probeA.csv $GALLERY/probeA.gal
janus_enroll $BR $CS0_DIR $CS0_DIR/protocol/galleryB.csv $GALLERY/galleryB.gal
janus_enroll $BR $CS0_DIR $CS0_DIR/protocol/probeB.csv $GALLERY/probeB.gal

# evaluate protocol A and B
janus_evaluate $BR $GALLERY/galleryA.gal $GALLERY/probeA.gal $CS0_DIR/protocol/galleryA.csv $CS0_DIR/protocol/probeA.csv $MATRIX/protocolA.mtx $MATRIX/protocolA.mask
janus_evaluate $BR $GALLERY/galleryB.gal $GALLERY/probeB.gal $CS0_DIR/protocol/galleryB.csv $CS0_DIR/protocol/probeB.csv $MATRIX/protocolB.mtx $MATRIX/protocolB.mask

# generate ROC/CMC, etc. and plot in openBR
br -eval $MATRIX/protocolA.mtx $MATRIX/protocolA.mask $RESULTS/protocolA.csv
br -eval $MATRIX/protocolB.mtx $MATRIX/protocolB.mask $RESULTS/protocolB.csv

# plot results in openBR
br -plot $RESULTS/protocolA.csv $RESULTS/protocolB.csv $RESULTS/resultsAB.pdf

open $RESULTS/resultsAB.pdf
