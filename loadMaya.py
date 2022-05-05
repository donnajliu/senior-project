import os
import os.path
import sys
import maya.mel as mel
import maya.cmds as cmds


# path to controlPoints.txt file in repo
file1 = open(os.path.expanduser("~/Desktop/DMDSeniorProject-master/knots_txt_files/controlPoints.txt"), 'r')

# get rid of first 'Knot Step' line
line = file1.readline()

iterNum = 1
endWhile = False

while True:
    # if we reach end of doc
    if endWhile:
        break
    
    # example: curve -d 3 -p 0 0 0 -p 1 1 1 -p 2 2 2 -p 3 3 3 -p 4 4 4 -k 0 -k 0 -k 0 -k 1 -k 2 -k 2 -k 2;
    execution = 'curve -d 3'
    numPoints = 0

    firstPoint = ''
    first = True
    secondPoint = ''
    second = True

    while True:
        # Get next line from file
        line = file1.readline()

        # if line is empty
        # end of file is reached
        if not line:
            endWhile = True
            break

        x = line.strip()

        # if new curve
        if x == 'Knot Step':
            break

        # store first and second points
        if first:
            first = False
            firstPoint = x
        else:
            if second:
                second = False
                secondPoint = x
                
        execution += ' -p ' + x
        numPoints= numPoints + 1

    for x in range(3):
        execution += ' -k 0'
    for x in range(1, numPoints - 3):
        execution += ' -k ' + str(x)
    for x in range(3):
        execution += ' -k ' + str(numPoints - 3)

    mel.eval(execution + ';')

    firstPointArray = [float(x) for x in firstPoint.split()] 
    secondPointArray = [float(x) for x in secondPoint.split()] 
    vector = ''

    for index in range(len(firstPointArray)):
        pp = secondPointArray[index] - firstPointArray[index]
        vector += str(pp) 

    print(firstPoint)
    print(secondPoint)
    print(vector)

    angle = mel.eval('angleBetween -euler -v1 ' + vector + ' -v2 ' + '0.0 1.0 0.0;')
    print(angle)

    mel.eval('polyCylinder -r 1 -h 2 -sx 20 -sy 1 -sz 1 -ax 0 1 0 -rcp 0 -cuv 3 -ch 1;')
    mel.eval('select -r pCylinder' + str(iterNum) + ';')
    mel.eval('scale -r 0.203127 1 0.203127;')
    mel.eval('scale -r 1 0.0997738 1;')
    mel.eval('scale -r 0.454723 1 0.454723;')
    mel.eval('scale -r 1 0.22715 1;')
    mel.eval('move -a ' + firstPoint + ';')
    mel.eval('rotate -r ' + str(angle[0]) + ' ' + str(angle[2]) + ' ' + str(angle[2]) + ';')
    # extrude
    mel.eval('polyExtrudeFacet -constructionHistory 1 -keepFacesTogether 1 -pvx 7.499999991 -pvy -3.499999986 -pvz 1.727336381 -divisions 30 -twist 0 -taper 1 -off 0 -thickness 0 -smoothingAngle 30 -inputCurve curve' + str(iterNum) + '  pCylinder' + str(iterNum) + '.f[20:39];')
    mel.eval('setAttr "polyExtrudeFace' + str(iterNum) + '.divisions" 400;')

    iterNum = iterNum + 1

for x in range(1, iterNum):
    mel.eval('setAttr "pCylinder' + str(x) + '.visibility" 0;')
    mel.eval('if( `getAttr -k "pCylinder' + str(x) + '.v"`||`getAttr -channelBox "pCylinder' + str(x) + '.v"` )setKeyframe "pCylinder' + str(x) + '.v";')

frames = 100

for x in range(1, iterNum - 1):

    #create blend shape
    mel.eval('select -r pCylinder'+ str(x + 1) + ' ;')
    mel.eval('select -tgl pCylinder'+ str(x) + ' ;')
    mel.eval('blendShape -automatic;')
    ## KEYING ##
    mel.eval('currentTime '+ str(1 + frames * (x-1)) + ' ;')
    mel.eval('setAttr "pCylinder' + str(x) + '.visibility" 1;')
    mel.eval('if( `getAttr -k "pCylinder' + str(x) + '.v"`||`getAttr -channelBox "pCylinder' + str(x) + '.v"` )setKeyframe "pCylinder' + str(x) + '.v";')
    mel.eval('setKeyframe { "blendShape' + str(x) + '.w[0]" };')
    
    # mel.eval('select -r pCylinder'+ str(x + 1) + ' ;')
    # mel.eval('setKeyframe -breakdown 0 -preserveCurveShape 0 |pCylinder' + str(x + 1) + '.translate;')
    # mel.eval('setKeyframe -breakdown 0 -preserveCurveShape 0 -hierarchy none -controlPoints 0 -shape 0 {"pCylinder' + str(x + 1) + '"};')


    ## SECOND KEYING ##
    mel.eval('currentTime ' + str(frames * x) + ' ;')
    mel.eval('setAttr "blendShape' + str(x) + '.pCylinder' + str(x + 1) + '" 1;')
    mel.eval('setKeyframe { "blendShape' + str(x) + '.w[0]" };')
    mel.eval('setAttr "pCylinder' + str(x) + '.visibility" 0;')
    mel.eval('if( `getAttr -k "pCylinder' + str(x) + '.v"`||`getAttr -channelBox "pCylinder' + str(x) + '.v"` )setKeyframe "pCylinder' + str(x) + '.v";')

    # move older shape to new blend shape so animation flows (still in second key)
    # mel.eval('currentTime ' + str(320 * x) + ' ;')

    # xyzOne = cmds.objectCenter('pCylinder' + str(x), gl=True)
    # xyzTwo = cmds.objectCenter('pCylinder' + str(x+1), gl=True)

    # diffX = xyzOne[0] - xyzTwo[0]
    # diffY = xyzOne[1] - xyzTwo[1]
    # diffZ = xyzOne[2] - xyzTwo[2]

    # mel.eval('select -r pCylinder'+ str(x + 1) + ' ;')
    # mel.eval('move -r ' + str(diffX) + ' ' + str(diffY) + ' ' + str(diffZ) + ' ;')
    # mel.eval('setKeyframe -breakdown 0 -preserveCurveShape 0 |pCylinder' + str(x + 1) + '.translate;')
    # mel.eval('setKeyframe -breakdown 0 -preserveCurveShape 0 -hierarchy none -controlPoints 0 -shape 0 {"pCylinder' + str(x + 1) + '"};')


file1.close()

for mesh in cmds.ls(type='mesh'):
    cmds.setAttr('{}.displaySmoothMesh'.format(mesh), 0)


