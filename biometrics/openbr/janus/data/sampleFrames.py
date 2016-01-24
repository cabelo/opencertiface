import os, sys, csv

csv_file = sys.argv[1]
interval = int(sys.argv[2])

with open(csv_file, 'r') as in_file:
	reader = csv.reader(in_file, dialect='excel')
	header = reader.next()
	with open('output.csv', 'w') as out_file:
		writer = csv.writer(out_file, dialect='excel')
		writer.writerow(header)

		in_file.seek(0)

		for row in reader:
			if row[2] == 'FRAME':
				pass
			elif row[2] == '':
				writer.writerow(row)
			elif row[2] == '0':
				writer.writerow(row)
			elif int(row[2]) % interval == 0:
				writer.writerow(row)