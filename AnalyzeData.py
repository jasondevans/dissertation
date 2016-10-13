import sys
import os
import re
import xml.etree.ElementTree as et



# Declare global variables
f = sys.stdout



# Subject Class Definition

class Subject:
	def __init__(self, filestr):
		self.filename = filestr
		self.subjectId = 0
		self.conditionGameInstructions = False
		self.conditionWordset = False
		self.conditionKeymap = False
		self.congrTrials = 0
		self.incongrTrials = 0
		self.totalTrials = 0
		self.congrNr = 0
		self.congrWindow = 0
		self.congrResp = 0
		self.congrEarly = 0
		self.congrLate = 0
		self.congrCorrect = 0
		self.incongrNr = 0
		self.incongrWindow = 0
		self.incongrResp = 0
		self.incongrEarly = 0
		self.incongrLate = 0
		self.incongrCorrect = 0
		self.congrPropCorrect = 0
		self.incongrPropCorrect = 0
		self.totalResp = 0
		self.totalCorrect = 0
		self.totalPropCorrect = 0
		self.propCorrectDifference = 0
		self.overallInWindowProp = 0
		self.aaq = []
		self.aaq_score = 0
		self.bisbas = []
		self.bisbas_bas_drive = 0
		self.bisbas_bas_fun = 0
		self.bisbas_bas_reward = 0
		self.bisbas_bis = 0
		self.grf = []
		self.grf_promotion = 0
		self.grf_prevention = 0
		self.eam_spatial_approach_words = 0
		self.eam_spatial_avoid_words = 0
		self.age = ''
		self.gender = ''
		self.race_ethnicity = ''
		self.handedness = ''
		tree = et.parse(filestr)
		if (tree.find('./condition-randomization/game-instructions').text == '1'):
			self.conditionGameInstructions = True
		else:
			self.conditionGameInstructions = False
		if (tree.find('./condition-randomization/game-wordset').text == '1'):
			self.conditionWordset = True
		else:
			self.conditionWordset = False
		if (tree.find('./condition-randomization/priming-keymap').text == '1'):
			self.conditionKeymap = True
		else:
			self.conditionKeymap = False
		self.subjectId = tree.find('./subject-id').text
		n = 0
		for priming in tree.findall('priming'):
			n = n + 1
			if n >= 7:
				for trial in priming.findall('trial'):
					self.totalTrials += 1
					congruent = False
					if trial.find('prime-spatial-action').text == 'approach' and trial.find('target-valence').text == 'pleasant':
						congruent = True
					elif trial.find('prime-spatial-action').text == 'avoid' and trial.find('target-valence').text == 'unpleasant':
						congruent = True
					inWindow = False
					if trial.find('response-type').text == 'WINDOW':
						inWindow = True
					correct = False
					if trial.find('target-valence').text == trial.find('standard-response').text:
						correct = True
					responded = False
					if (trial.find('response-type').text in ['EARLY', 'LATE', 'WINDOW']):
						responded = True
						self.totalResp += 1
					if congruent:
						self.congrTrials += 1
						if inWindow:
							self.congrWindow += 1
						elif trial.find('response-type').text == 'NR':
							self.congrNr += 1
						elif trial.find('response-type').text == 'EARLY':
							self.congrEarly += 1
						elif trial.find('response-type').text == 'LATE':
							self.congrLate += 1
						if correct:
							self.congrCorrect += 1
							self.totalCorrect += 1
						if responded:
							self.congrResp += 1
					else:
						self.incongrTrials += 1
						if inWindow:
							self.incongrWindow += 1
						elif trial.find('response-type').text == 'NR':
							self.incongrNr += 1
						elif trial.find('response-type').text == 'EARLY':
							self.incongrEarly += 1
						elif trial.find('response-type').text == 'LATE':
							self.incongrLate += 1
						if correct:
							self.incongrCorrect += 1
							self.totalCorrect += 1
						if responded:
							self.incongrResp += 1
		self.congrPropCorrect = self.congrCorrect / self.congrResp
		self.incongrPropCorrect = self.incongrCorrect / self.incongrResp
		self.totalPropCorrect = self.totalCorrect / self.totalResp
		self.propCorrectDifference = self.congrPropCorrect - self.incongrPropCorrect
		self.overallInWindowProp = ((self.congrWindow + self.incongrWindow) / (self.congrTrials + self.incongrTrials))
		for question in tree.findall("./questionnaire[@name='Acceptance and Action Questionnaire']/question"):
			self.aaq.append(int(question.text))
			self.aaq_score += int(question.text)
		for question in tree.findall("./questionnaire[@name='BIS BAS']/question"):
			self.bisbas.append(int(question.text))
		self.bisbas_bas_drive = (4 * 4) - (self.bisbas[3-1] + self.bisbas[9-1] + self.bisbas[12-1] + self.bisbas[21-1])
		self.bisbas_bas_fun =  (4 * 4) - (self.bisbas[5-1] + self.bisbas[10-1] + self.bisbas[15-1] + self.bisbas[20-1])
		self.bisbas_bas_reward = (4 * 5) - (self.bisbas[4-1] + self.bisbas[7-1] + self.bisbas[14-1] + self.bisbas[18-1] + self.bisbas[23-1])
		self.bisbas_bis = (4 * 5) - (self.bisbas[8-1] + self.bisbas[13-1] + self.bisbas[16-1] + self.bisbas[19-1] + self.bisbas[24-1])
		self.bisbas_bis += self.bisbas[2-1] + self.bisbas[22-1]
		for question in tree.findall("./questionnaire[@name='General Regulatory Focus']/question"):
			self.grf.append(int(question.text))
		self.grf_promotion = self.grf[3-1] + self.grf[5-1] + self.grf[6-1] + self.grf[8-1] + self.grf[12-1] + self.grf[14-1]
		self.grf_promotion += self.grf[16-1] + self.grf[17-1] + self.grf[18-1]
		self.grf_prevention = self.grf[1-1] + self.grf[2-1] + self.grf[4-1] + self.grf[7-1] + self.grf[9-1] + self.grf[10-1]
		self.grf_prevention += self.grf[11-1] + self.grf[13-1] + self.grf[15-1]
		for question in tree.findall("./questionnaire[@name='Explicit Attitude Measurement']/question"):
			thisWord = question.get('info')
			thisValue = int(question.text)
			if (self.conditionWordset):
				if (thisWord in ['CHAIR', 'BASKET']):
					self.eam_spatial_approach_words += thisValue
				else:
					self.eam_spatial_avoid_words += thisValue
			else:
				if (thisWord in ['WAGON', 'STATUE']):
					self.eam_spatial_approach_words += thisValue
				else:
					self.eam_spatial_avoid_words += thisValue
		self.age = tree.find('./demographics/age').text
		self.gender = tree.find('./demographics/gender').text
		self.race_ethnicity = tree.find('./demographics/race-ethnicity').text
		self.handedness = tree.find('./demographics/handedness').text

# end Subject Class Definition



# printSubject Method

def printSubject(subject):
	f.write('File: ' + subject.filename + '\n')
	f.write('Subject ID: ' + subject.subjectId + '\n')

	f.write('Game instructions condition: ' + str(subject.conditionGameInstructions) + '\n')
	f.write('Wordset condition: ' + str(subject.conditionWordset) + '\n')
	f.write('Keymap condition: ' + str(subject.conditionKeymap) + '\n')
	
	f.write('congr trials: ' + str(subject.congrTrials) + '\n')
	f.write('incongr trials: ' + str(subject.incongrTrials) + '\n')

	f.write('overall proportion within window: ' + '%.3f' % subject.overallInWindowProp + '\n')
	
	f.write('congr nr: ' + str(subject.congrNr) + '\n')
	f.write('congr window: ' + str(subject.congrWindow) + '\n')
	f.write('congr resp: ' + str(subject.congrResp) + '\n')
	f.write('congr early: ' + str(subject.congrEarly) + '\n')
	f.write('congr late: ' + str(subject.congrLate) + '\n')
	f.write('congr correct: ' + str(subject.congrCorrect) + '\n')
	
	f.write('incongr nr: ' + str(subject.incongrNr) + '\n')
	f.write('incongr window: ' + str(subject.incongrWindow) + '\n')
	f.write('incongr resp: ' + str(subject.incongrResp) + '\n')
	f.write('incongr early: ' + str(subject.incongrEarly) + '\n')
	f.write('incongr late: ' + str(subject.incongrLate) + '\n')
	f.write('incongr correct: ' + str(subject.incongrCorrect) + '\n')

	f.write('congr proportion correct: ' + '%.3f' % subject.congrPropCorrect + '\n')
	f.write('incongr proportion correct: ' + '%.3f' % subject.incongrPropCorrect + '\n')
	f.write('total proportion correct: ' + '%.3f' % subject.totalPropCorrect + '\n')
	f.write('congr - incongr proportion correct: ' + '%.3f' % subject.propCorrectDifference + '\n')

	f.write('AAQ: ' + str(subject.aaq_score) + '\n')
	f.write('BIS/BAS BAS DRIVE: ' + str(subject.bisbas_bas_drive) + '\n')
	f.write('BIS/BAS BAS FUNSEEKING: ' + str(subject.bisbas_bas_fun) + '\n')
	f.write('BIS/BAS BAS REWARD RESPONSIVENESS: ' + str(subject.bisbas_bas_reward) + '\n')
	f.write('BIS/BAS BIS: ' + str(subject.bisbas_bis) + '\n')
	f.write('GRF PROMOTION: ' + str(subject.grf_promotion) + '\n')
	f.write('GRF PREVENTION: ' + str(subject.grf_prevention) + '\n')
	f.write('EAM SPATIAL APPROACH WORDS: ' + str(subject.eam_spatial_approach_words) + '\n')
	f.write('EAM SPATIAL AVOID WORDS: ' + str(subject.eam_spatial_avoid_words) + '\n')

	f.write('Age: ' + str(subject.age) + '\n')
	f.write('Gender: ' + str(subject.gender) + '\n')
	f.write('Race / Ethnicity: ' + str(subject.race_ethnicity) + '\n')
	f.write('Handedness: ' + str(subject.handedness) + '\n')

# end printSubject Method



# printSubjectCsv Method

def printSubjectCsv(subject):
	s = str(subject.subjectId)
	s += ',' + (str(1) if subject.conditionGameInstructions else str(0))
	s += ',' + (str(1) if subject.conditionWordset else str(0))
	s += ',' + (str(1) if subject.conditionKeymap else str(0))
	s += ',' + '%.3f' % subject.congrPropCorrect
	s += ',' + '%.3f' % subject.incongrPropCorrect
	s += ',' + '%.3f' % subject.totalPropCorrect
	s += ',' + '%.3f' % subject.propCorrectDifference
	s += ',' + str(subject.aaq_score)
	s += ',' + str(subject.bisbas_bas_drive)
	s += ',' + str(subject.bisbas_bas_fun)
	s += ',' + str(subject.bisbas_bas_reward)
	s += ',' + str(subject.bisbas_bis)
	s += ',' + str(subject.grf_promotion)
	s += ',' + str(subject.grf_prevention)
	s += ',' + str(subject.eam_spatial_approach_words)
	s += ',' + str(subject.eam_spatial_avoid_words)
	s += ',' + subject.age
	s += ',' + subject.gender
	s += ',' + subject.race_ethnicity
	s += ',' + subject.handedness
	f.write(s + '\n')

# end printSubjectCsv Method
	


# Main
		
# Declare variables
validSyntax = False
directoryMode = False
writeToFile = False
filestr = ''
output_filestr = ''

# Process arguments
argIndex = 1
while argIndex < len(sys.argv):
	if sys.argv[argIndex] == '-f':
		validSyntax = True
		directoryMode = False
		filestr = sys.argv[argIndex + 1]
		argIndex += 2
	elif sys.argv[argIndex] == '-d':
		validSyntax = True
		directoryMode = True
		filestr = sys.argv[argIndex + 1]
		argIndex += 2
	elif sys.argv[argIndex] == '-o':
		writeToFile = True
		output_filestr = sys.argv[argIndex + 1]
		f = open(output_filestr, 'w')
		argIndex += 2

# Print usage if invalid syntax
if not validSyntax:
	print("Usage: AnalyzeData.py [-f filename | -d dirname] [-o outputfile]")
	sys.exit()

# Process single file
if (directoryMode == False):
	subject = Subject(filestr)
	printSubject(subject)
	
# Process directory
else:
	subjects = []
	dirlisting = os.listdir(filestr)
	for infile in dirlisting:
		if (re.match(r'ExperimentData.*\.xml', infile)):
			thisSubject = Subject(filestr + os.sep + infile)
			subjects.append(thisSubject)
	for subject in subjects:
		printSubjectCsv(subject)
	n = 0
	crpcTotal = 0
	irpcTotal = 0
	for subject in subjects:
		n += 1
		crpcTotal += subject.congrPropCorrect
		irpcTotal += subject.incongrPropCorrect
	crpcAvg = crpcTotal / n
	irpcAvg = irpcTotal / n
	# f.write('====================' + '\n')
	# f.write('Congruent proportion correct (avg) = ' + '%.3f' % crpcAvg + '\n')
	# f.write('Incongruent proportion correct (avg) = ' + '%.3f' % irpcAvg + '\n')

# Clean up
if writeToFile:
	f.close()

# end Main




