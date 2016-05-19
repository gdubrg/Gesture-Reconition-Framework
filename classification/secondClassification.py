__author__ = 'Guido Borghi'

import os
import sys
import re
import numpy
import sklearn
from sklearn import tree
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn import svm


def createFeatures(scores):

    #v1
    #featureset = scores

    #v2
    featureset =[]
    ordini = numpy.argsort(scores)
    for valori in ordini:
        featureset.append(valori)

    return featureset, ordini

#clf = tree.DecisionTreeClassifier()
clf = tree.DecisionTreeClassifier(max_depth=None, min_samples_split=4, random_state=5)
#clf = RandomForestClassifier()
#clf = ExtraTreesClassifier()
#clf = tree.DecisionTreeRegressor()
#clf  = AdaBoostClassifier()
#clf = KNeighborsClassifier(n_neighbors=50)
mysvm = svm.SVC(probability=True);
print('culo')
labels = []

#Load all file in folder path
pathTrain = r'train2'
pathTest = r'test2'


data = []
finalTrue = []
finalFalse = []
finalTmp = []

allFinalTrue = [[] for x in range(0,21)] #divisione per classi
allFinalFalse = [[] for x in range(0,21)]

for dir_entry in os.listdir(pathTrain):
    dir_entry_path = os.path.join(pathTrain, dir_entry)
    if os.path.isfile(dir_entry_path):
        with open(dir_entry_path, 'r') as file:

            #FILENAME
            filename = file.readline().rstrip()
            print filename

            #LABEL
            label = file.readline().rstrip()
            labels.append(label)
            #print label

            #PREVIOUS CLASSIFICATION
            bclass = file.readline().rstrip()

            #VECTOR SCORES
            scores = []
            for count in range(0, 20):
                scores.append (-float(file.readline().rstrip()))

            #v1:
            featureset, ordini = createFeatures(scores)
            # featureset = scores

            #\n
            file.readline()

            #Number of frames
            numFrames = file.readline().rstrip()
            numFrames = float(numFrames)

            #NUmber of HMMs
            numHMM = file.readline().rstrip()

            #per ogni hmm
            for nHMM in range(0, int(numHMM)):
                #per ogni stato
                for nS in range(0, 8):
                    line = file.readline().rstrip()
                    val = 0
                    #percentuale di tempo di essere in quello stato
                    for t in line.split():
                        val += float(t)
                    #quando sono qui ho calcolato la somma di tutta una riga
                    #print val/numFrames
                    finalTmp.append(val)
                #quando sono qui ho calcolato le somme di un hmm, decido dove memorizzarlo
                if bclass=="TRUE":
                    if ((nHMM+1) == int(label)):
                        #finalTrue.append(finalTmp)
                        allFinalTrue[int(label)].append(finalTmp)
                        #print "true:", finalTmp

                        #print "true", nHMM+1, traduzioneda0[int(label)]
                    elif ((nHMM+1) != int(label)):
                       # finalFalse.append(finalTmp)
                        allFinalFalse[int(label)].append(finalTmp)
                        #print "false:", finalTmp
                        #print "false", nHMM+1, traduzioneda0[int(label)]

                # if(finalTrue):
                #     allFinalTrue[int(label)].append(finalTrue)
                # if(finalFalse):
                #     allFinalFalse[int(label)].append(finalFalse)
                #finalTrue = []
                #finalFalse = []
                #featureset.append()
                finalTmp = []
                #leggo due \n
                file.readline()
                file.readline()

                #print "finito"

            data.append(featureset)

# quando arrivo qui ho un vettore di labels ed un vettore di dati
#print labels
#print data

print "Totale labels TRAIN:", len(labels)
print "Totale data TRAIN:", len(data)

#proviamo ad addestrare qualcosa
clf.fit(data, labels)

contatore=0
for i in range(0,len(data)):
    #print clf.predict(data[i])[0]
    #print labels[i]
    if (clf.predict(data[i])[0]==labels[i]):
        contatore+=1

print "Test su training:", contatore


#-----------------------------------
# ysvm = []
# for countT in finalTrue:
#     ysvm.append(0)
# for countF in finalFalse:
#     ysvm.append(1)
# unionFeatSVM = finalTrue + finalFalse
# print len(unionFeatSVM), "classi", len(ysvm)
# unionFeatSVM = sklearn.preprocessing.normalize(unionFeatSVM)
# mysvm.fit(unionFeatSVM, ysvm)


allSVM = [svm.SVC(probability=True, kernel='rbf') for x in range(0, 21)]
for i in range(1, len(allSVM)):
    ysvm = []
    for countT in allFinalTrue[i]:
        ysvm.append(0)
    for countF in allFinalFalse[i]:
        ysvm.append(1)
    #print len(allFinalTrue[i]), len(allFinalFalse[i])
    allFeat = allFinalTrue[i] + allFinalFalse[i]
    allFeat = sklearn.preprocessing.normalize(allFeat)
    allSVM[i].fit(allFeat, ysvm)

print "------------------------------------------------------------------------"

# test vero e proprio
data = []
labels = []
labelHMM = []

finalAll = []
finalTmp = []

for dir_entry in os.listdir(pathTest):
    dir_entry_path = os.path.join(pathTest, dir_entry)
    if os.path.isfile(dir_entry_path):
        with open(dir_entry_path, 'r') as file:

            #FILENAME
            filename = file.readline().rstrip()
            print filename

            #LABEL
            label = file.readline().rstrip()
            labels.append(label)

            #PREVIOUS CLASSIFICATION
            bclass = file.readline()

            #VECTOR SCORES
            scores = []
            for count in range(0, 20):
                scores.append (-float(file.readline().rstrip()))

            #v1:
            featureset, ordini = createFeatures(scores)
            #featureset = scores

            #\n
            file.readline()

            #Number of frames
            numFrames = file.readline().rstrip()
            numFrames = float(numFrames)

            #NUmber of HMMs
            numHMM = file.readline().rstrip()

            nov = []
            #per ogni hmm
            for nHMM in range(0, int(numHMM)):
                for nS in range(0, 8):
                    line = file.readline().rstrip()
                    val = 0
                    for t in line.split():
                        val += float(t)
                    #quando sono qui ho calcolato la somma di tutta una riga
                    finalTmp.append(val)
                #quando sono qui ho calcolato le somme di un hmm
                finalAll.append(finalTmp)
                nov.append(finalTmp)

                finalTmp = []
                #leggo due \n
                file.readline()
                file.readline()

                #print "finito"

            data.append(featureset)

            labelHMM.append(ordini[0])
            #print labelHMM

# quando arrivo qui ho un vettore di labels ed un vettore di dati

print labels
print data
print labelHMM

print "Totale labels TEST:", len(labels)
print "Totale data TEST:", len(data)
print "Totale finalALL:", len(finalAll)

contatore = 0
contP = 0
contM = 0
contSVM = 0

totTested = len(data)
totOkHMM = 0.
totOkRT = 0.
totOkSVM = 0.
totSVMTested = 0

for i in range(0, len(data)):
    position = i*20+labelHMM[i]
    #print position, labelHMM[i]
    #print finalAll[position]

    #valutare se normalizzare
    finalAll[position] = sklearn.preprocessing.normalize(finalAll[position])

    #print finalAll[position]
    sicurezza = allSVM[int(labelHMM[i])+1].predict_proba(finalAll[position])[0][0]
    print "GT", int(labels[i]), "\tClass. HMM:", int(labelHMM[i])+1, "\tsicurezza:", sicurezza, "\tNostro class.", clf.predict(data[i])[0],
    # for k in range(1, len(allSVM)):
    #     print allSVM[k].predict_proba(finalAll[position])[0][0],
    #print "GT", int(labels[i]), "\tClass. HMM:", traduzione[labelHMM[i]], "\tsicurezza:", mysvm.predict_proba(finalAll[position])[0][0], "\tNostro class.", clf.predict(data[i])[0],
    #print

    if (int(labels[i]) == int(labelHMM[i])+1):
        print "\tHMM TRUE",
        totOkHMM += 1
    else:
        print "\tHMM FALSE",

    if (int(clf.predict(data[i])[0]) == int(labels[i])):
        print "\tCLASS TRUE",
        totOkRT += 1
    else:
        print "\tCLASS FALSE",

    if (int(clf.predict(data[i])[0])==int(labels[i]) and int(labels[i]) != int(labelHMM[i])+1):
        #print "meglio!",
        contM+=1
    if (int(clf.predict(data[i])[0])!=int(labels[i]) and int(labels[i]) == int(labelHMM[i])+1):
        #print "peggio!",
        contP+=1

    #print "\n"

    soglia = 0.1
    if(sicurezza<soglia):
        #ricerco un nuovo candidato testando tutti gli SVM trainati
        totSVMTested += 1
        bestProb = []
        for k in range(1, 20):
            bestProb.append(allSVM[k].predict_proba(finalAll[position])[0][0])
        nuovaClass = (numpy.argmax(bestProb)+1)
        print "\t Potrebbe essere:", nuovaClass,
        if (nuovaClass == int(labels[i])):
            print "esatto",
            contSVM += 1
            totOkSVM += 1
        else:
            print "errore",

    print "\n"

# for i in range(0,len(data)):
#     if (int(clf.predict(data[i])[0])==int(labels[i]) and int(labels[i]) != int(labelHMM[i])+1):
#         print "meglio!"
#         contM+=1
#
#     if (int(clf.predict(data[i])[0])!=int(labels[i]) and int(labels[i]) == int(labelHMM[i])+1):
#         print "peggio!"
#         contP+=1

    # print "--------"
    # print "GT:", labels[i]
    # print "tree:", int(clf.predict(data[i])[0])
    # print "HMM:", traduzione[labelHMM[i]]
    if (int(clf.predict(data[i])[0])==int(labels[i])):
         contatore+=1
    #     print "Corretta"
    #     print
    # else:
    #     print "Sbagliata"

print "Corrette:", contatore
print "Totale:", len(data)
print "meglio:", contM
print "peggio:", contP
print "SVM:", contSVM
print "Class. HMM:", (totOkHMM/totTested)
print "Class. RT:", (totOkRT/totTested)
print "Class. SVM:", (totOkSVM/totSVMTested)







