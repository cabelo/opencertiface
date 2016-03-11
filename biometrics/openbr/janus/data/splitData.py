# Splits the dataset into n even datasets by number of subjects for cross validation. 

import csv
import os
import sys
import math

if len(sys.argv) != 4:
	print "splitData.py input.csv outputDir n-folds"

inputDir = sys.argv[1]
outputDir = sys.argv[2]
n = int(sys.argv[3])

dataSet = open(inputDir, 'r')
dataSetReader = csv.reader(dataSet, dialect='excel')

header = dataSetReader.next()

records = []
for row in dataSetReader:
	records.append(row[0])

templates = list(set(records))
numTemplates = len(templates)

print numTemplates
dataSet.seek(0)
dataSetReader.next()

partitionSize = int(math.floor(5749 / int(n)))

for i in range(n):
	dataSet.seek(0)
	dataSetReader.next()

	output = open(outputDir + "/train%d.csv" % i, 'w')
	writer = csv.writer(output, dialect='excel')
	writer.writerow(header)

	if i == (n-1):
		for row in dataSetReader:
			if int(row[0]) >= i*partitionSize:
				writer.writerow(row)

	else:
		for row in dataSetReader:
			if (int(row[0])>= i*partitionSize) & (int(row[0]) < (i+1)*partitionSize):
				writer.writerow(row)

	output.close()


dataSet.close()
