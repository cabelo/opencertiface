import csv, os, sys, glob, re

# Script to combine lfw and ytf.  Setting percentage train to 100 will send all output to a single csv
# Subjects that do not have >= numImg and >= numVid are partitioned off into a probe set


# Check parameters
if len(sys.argv) != 6:
    print "lfw_ytf.py lfwRootdir ytfRootdir numImg numVid percentageTrain"
    exit(-1)

# used for natural sorting
def natural_key(string_):
    return [int(s) if s.isdigit() else s for s in re.split(r'(\d+)', string_)]



lfwRootdir = sys.argv[1]
ytfRootdir = sys.argv[2]
numImg = int(sys.argv[3])
numVid = int(sys.argv[4])
pctTrain = int(sys.argv[5])

pctTrain = pctTrain/100.0

train = open("lfw_ytf_train.csv", 'w')
test = open("lfw_ytf_test.csv", 'w')
probe = open("lfw_ytf_probe.csv", 'w')

trainWriter = csv.writer(train, dialect = 'excel')
testWriter = csv.writer(test, dialect = 'excel')
probeWriter = csv.writer(probe, dialect = 'excel')

# Write header row for output files
trainWriter.writerow(['TEMPLATE_ID', 'FILE_NAME', 'FRAME'])
testWriter.writerow(['TEMPLATE_ID', 'FILE_NAME', 'FRAME'])
probeWriter.writerow(['TEMPLATE_ID', 'FILE_NAME', 'FRAME'])

lfwSubjectDirectories = glob.glob(os.path.join(lfwRootdir, '*'))

# Use length of lfw subjects because ytf is a subset of lfw
trainAmount = int(len(lfwSubjectDirectories) * pctTrain)
templateID = 0
count = 0

for lfwSubject in lfwSubjectDirectories:
    # get video directories for subject if there are any
    videoDirectories = glob.glob(os.path.join(ytfRootdir, os.path.split(lfwSubject)[1]))
    if len(videoDirectories) == 0:
        videos = []
    else:
        videos = glob.glob(os.path.join(videoDirectories[0], '*'))
        videos.sort(key = natural_key)

    # get all the subject's images
    images = glob.glob(os.path.join(lfwSubject, '*'))
    images.sort(key = natural_key)

    # Probe: check to see if they meet the image and video requirements
    if (len(images) < numImg) or (len(videos) < numVid):
        for img in images:
            probeWriter.writerow([templateID, img, None])
        for vid in videos:
            frame = 0
            vidFrames = glob.glob(os.path.join(vid, '*'))
            vidFrames.sort(key = natural_key)
            for frm in vidFrames:
                probeWriter.writerow([templateID, frm, frame])
                frame += 1
    # Train: first n subjects based on pctTrain are put into training set
    elif count < trainAmount:
        for img in images:
            trainWriter.writerow([templateID, img, None])
        for vid in videos:
            frame = 0
            vidFrames = glob.glob(os.path.join(vid, '*'))
            vidFrames.sort(key = natural_key)
            for frm in vidFrames:
                trainWriter.writerow([templateID, frm, frame])
                frame += 1
        count += 1
    # Test
    else:
        for img in images:
            testWriter.writerow([templateID, img, None])
            print templateID, os.path.split(img)[1]
        for vid in videos:
            frame = 0
            vidFrames = glob.glob(os.path.join(vid, '*'))
            vidFrames.sort(key = natural_key)
            for frm in vidFrames:
                testWriter.writerow([templateID, frm, frame])
                print templateID, os.path.split(frm)[1]
                frame += 1

    templateID += 1

train.close()
test.close()
probe.close()
