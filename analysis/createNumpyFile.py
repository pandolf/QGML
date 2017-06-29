#! /usr/bin/python

import numpy as np
import ROOT


f = ROOT.TFile.Open('qgMiniTuple.root', 'read')
t = f.Get('qgMiniTupleAK4chs/qgMiniTuple')

n = t.GetEntries()
fivePercent = int(float(n)/20.)

qg_array = []
word_array = []

print 'Tree has %d entries' % (n)
print 'Looping on tree to build arrays...'

iEv = 0

max_features = 50

for e in t:

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

  #while len(x)<max_features:
  #  x.append( [0.0, 0.0] )

  xnp = np.array(x, dtype=float)
  #if xnp.dtype!='float64': 
  #  print '\n\n'
  #  print x
  #  print 'dtype: %s' % (xnp.dtype)

  word_array.append( x )

  iEv = iEv+1



print 'Done looping.'

y = np.array(qg_array)
X = np.array(word_array)

filename = 'qgNumpyRNN.npz'
f = open(filename,'w')
np.savez(f,X=X,y=y)
f.close()

print 'Saved numpy arrays to file: '+filename
