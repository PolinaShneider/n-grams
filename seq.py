import json
import os
import re
import sys
import time

"""This method list of all inputs.
Should be changed as per the input format"""

startTime = time.time()

def buildInput(basePath):
    """Generate list of all possible files in directory
    and subdirectory, with depth 1"""
    pathList = []
    for dir in os.listdir(basePath):
        for file in os.listdir(os.path.join(basePath, dir)):
            pathList.append(os.path.join(basePath, dir, file))
    return pathList


def readFile(path):
    with open(path, 'r', errors='surrogateescape') as inputFile:
        print("Executing for " + inputFile.name)
        text = inputFile.read()
    return text


def writeToFile(path, text):
    with open(path, 'w') as outputFile:
        outputFile.write(text)


def cleanText(text):
    """Convert text to lower case"""
    text = text.lower()
    """Remove special characters + email addresses + alpha numeric entries"""
    text = re.sub(r'\S*@\S*\s?|([^\s\w]|_)+|\w*\d\w*|[^А-Яа-я0-9\s]|^\d+\s|\s\d+\s|\s\d+$', '', text)
    """remove new lines"""
    text = text.replace("\n", " ")
    """Replace more than one tabs with space"""
    text = re.sub('\t+', ' ', text)
    """Finally remove more than one spaces with space"""
    text = re.sub(' +', ' ', text)
    return text


def generateNormalizedTermFrequency(tokens, documentFrequency):
    termFrequency = {}
    tokenCount = len(tokens)
    for token in tokens:
        if token in termFrequency:
            """Division by tokenCount in document will normalize the termfrequency"""
            termFrequency[token] = termFrequency[token] + 1.0 / tokenCount
        else:
            termFrequency[token] = 1.0 / tokenCount

            if token in documentFrequency:
                documentFrequency[token] = documentFrequency[token] + 1
            else:
                documentFrequency[token] = 1
    return termFrequency, documentFrequency


def wordgramGenerator(tokens, n):
    ngrams = []
    for index in range(0, len(tokens)):
        candidate = tokens[index:index + n]
        ngram = ' '.join(candidate)
        ngrams.append(ngram)
    return ngrams


def ngramGenerator(text, ngramRange):
    tokens = text.split(" ")
    ngrams = []
    for i in range(ngramRange[0], ngramRange[1] + 1):
        ngrams.extend(wordgramGenerator(tokens, i))
    return ngrams

"""read input path as command line parameter"""
inputPath = sys.argv[1]
outputPath = sys.argv[2]
ngramS = int(sys.argv[3])
ngramE = int(sys.argv[4])

pathList = None

documentFrequency = {}

if not os.path.exists(outputPath):
    os.makedirs(outputPath)

if not os.path.exists(os.path.join(outputPath, "Sequential")):
    os.makedirs(os.path.join(outputPath, "Sequential"))

pathList = buildInput(inputPath)

"""Calculate term frequency and the relative document frequency
Here first I am reading text from the input file.
Followed by clean up of text file. The cleanText procedure will remove 
characters."""
for file in pathList:
    text = readFile(file)
    text = cleanText(text)
    tokens = ngramGenerator(text, (ngramS, ngramE))
    tokenTermFrequency, documentFrequency = generateNormalizedTermFrequency(tokens, documentFrequency)
    writeToFile(os.path.join(outputPath, "Sequential", os.path.basename(file) + ".json"),
                json.dumps(tokenTermFrequency, indent=2, ensure_ascii=False))

endTime = time.time()
print("Total time taken " + str((endTime - startTime)))
