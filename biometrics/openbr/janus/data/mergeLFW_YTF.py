# Merges the YTF and LFW directories

import shutil, glob, os

ytfSubjectDirectories = glob.glob('ytf/*')

for subject in ytfSubjectDirectories:
	name = os.path.split(subject)[1]
	videos = glob.glob(os.path.join(subject, '*'))
	for video in videos:
		shutil.move(video, os.path.join('lfw/%s' % name, os.path.split(video)[1]))