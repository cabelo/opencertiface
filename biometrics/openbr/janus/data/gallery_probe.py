import csv, os, sys, re, random
# python galler_probe.py csvfile
# Randomly splits the dataset into gallery and probe, gallery has one piece of media per subject, probe has the rest.
# Subjects with only one piece of media are put in the sequestered set.  

csv_file = sys.argv[1]

with open(csv_file, 'r') as file:
	gallery = open('gallery.csv', 'w')
	probe = open('probe.csv', 'w')
	sequester = open('sequester.csv', 'w')

	reader = csv.reader(file, dialect='excel')
	galleryWriter = csv.writer(gallery, dialect='excel')
	probeWriter = csv.writer(probe, dialect='excel')
	sequesterWriter = csv.writer(sequester, dialect='excel')

	header = reader.next()
	galleryWriter.writerow(header)
	probeWriter.writerow(header)
	sequesterWriter.writerow(header)

	template = 0
	flag = True
	media = []
	videos = dict()
	for row in reader:
		if row[0] == str(template):
			if row[2] == '':
				media.append(row)
			elif row[2] == '0':
				media.append(row)
				videos[os.path.split(row[1])[0]] = [row]
			else:
				videos[os.path.split(row[1])[0]].append(row)
		else:
			template += 1
			random.shuffle(media)
			n = len(media)
			if n == 1:
				if media[0][2] == '0':
					for frame in videos[os.path.split(media[i][1])[0]]:
						sequesterWriter.writerow(frame)
				else:
					sequesterWriter.writerow(media[0])
			else:
				for i in xrange(n):
					if i == 0:
						if media[i][2] == '0':
							for frame in videos[os.path.split(media[i][1])[0]]:
								galleryWriter.writerow(frame)
						else:
							galleryWriter.writerow(media[i])
					else:
						if media[i][2] == '0':
							for frame in videos[os.path.split(media[i][1])[0]]:
								probeWriter.writerow(frame)
						else:
							probeWriter.writerow(media[i])

			media = []
			videos.clear()
			if row[2] == '':
				media.append(row)
			elif row[2] == '0':
				media.append(row)
				videos[os.path.split(row[1])[0]] = [row]
			else:
				videos[os.path.split(row[1])[0]].append(row)

	random.shuffle(media)
	n = len(media)
	if n == 1:
		if media[0][2] == '0':
			for frame in videos[os.path.split(media[i][1])[0]]:
				sequesterWriter.writerow(frame)
		else:
			sequesterWriter.writerow(media[0])
	else:
		for i in xrange(n):
			if i == 0:
				if media[i][2] == '0':
					for frame in videos[os.path.split(media[i][1])[0]]:
						galleryWriter.writerow(frame)
				else:
					galleryWriter.writerow(media[i])
			else:
				if media[i][2] == '0':
					for frame in videos[os.path.split(media[i][1])[0]]:
						probeWriter.writerow(frame)
				else:
					probeWriter.writerow(media[i])

