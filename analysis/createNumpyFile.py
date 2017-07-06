#! /usr/bin/python

import numpy as np
import ROOT


f = ROOT.TFile.Open('qgMiniTuple.root', 'read')
t = f.Get('qgMiniTupleAK4chs/qgMiniTuple')

n = t.GetEntries()
fivePercent = int(float(n)/20.)

qg_array = []
qgl_array = []
word_array = []

print 'Tree has %d entries' % (n)
print 'Looping on tree to build arrays...'

iEv = 0

max_features = 50

for e in t:

  iEv = iEv+1

  #if iEv%10000 == 0:
  #  print "  Entry: %d / %d" % (iEv,n)
  if iEv%fivePercent == 0 and iEv>0:
    print "  %.0f%% complete" % (100.*iEv/(float(n)))
  if iEv>=n : break

  if e.nch<5 : continue
  #if e.nch==0 : continue

  pdgId = e.partonId

  if abs(pdgId)<4 :
    qg_array.append( 1 )
  elif pdgId==21:
    qg_array.append( 0 )
  else:
    continue
    

  x = []


  for i in range(max_features):
      if i<e.nch:
        x.append( [e.pt_ch[i], e.dr_ch[i] ] )
      else:
        x.append( [0.0, 0.0] )


  xnp = np.array(x, dtype=float)

  word_array.append( x )

  qgl_array.append(e.qgl)




print 'Done looping.'

y = np.array(qg_array)
X = np.array(word_array)
qgl = np.array(qgl_array)

filename = 'qgNumpyRNN.npz'
f = open(filename,'w')
np.savez(f,X=X,y=y,qgl=qgl)
f.close()

print 'Saved numpy arrays to file: '+filename
