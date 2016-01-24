import csv, os, sys, re
# Converts the output csv of janusQuery.sql to proper janus metadata format.  


infile = sys.argv[1] # Output from janusQuery.sql
outfile = open(sys.argv[2], 'wb') # Output .csv

writer = csv.writer(outfile, dialect='excel')
writer.writerow(['TEMPLATE_ID', 'FILE_NAME', 'FRAME', 'RIGHT_EYE_X', 'RIGHT_EYE_Y', 'LEFT_EYE_X', 'LEFT_EYE_Y', 'NOSE_BASE_X', 'NOSE_BASE_Y'])

with open(infile, 'r') as csvfile:
    reader = csv.reader(csvfile, dialect='excel')
    for row in reader:
        template = row[0]
        filename = row[1] + '.' + row[2]
        frame = row[3]
        if frame == '':
            filename = 'DSX_IMAGES/DSX_IMAGES/' + filename
        else:
            filename = 'DSX_FRAMES/' + filename
        if row[5] == '':
            noseX = ''
            noseY = ''
        else:
            s = row[5].split(',')
            noseX = s[0]
            noseX = noseX.replace('(', '')
            noseY = s[1]
            noseY = noseY.replace(')', '')
        if row[7] == '':
            leftX = ''
            leftY = ''
        else:
            s = row[7].split(',')
            leftX = s[0]
            leftX = leftX.replace('(', '')
            leftY = s[1]
            leftY = leftY.replace(')', '')
        if row[6] == '':
            rightX = ''
            rightY = ''
        else:
            s = row[6].split(',')
            rightX = s[0]
            rightX = rightX.replace('(', '')
            rightY = s[1]
            rightY = rightY.replace(')', '')

        writer.writerow([template, filename, frame, rightX, rightY, leftX, leftY, noseX, noseY])

outfile.close()
