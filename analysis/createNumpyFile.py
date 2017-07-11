#! /usr/bin/python

import numpy as np
import ROOT

import sys



fileName = 'qgMiniTuple.root'
if len(sys.argv)>1:
  fileName = sys.argv[1]


f = ROOT.TFile.Open(fileName, 'read')
t = f.Get('qgMiniTupleAK4chs/qgMiniTuple')

n = t.GetEntries()
fivePercent = int(float(n)/20.)

qg_array = []
qgl_array = []
pt_array = []
eta_array = []
X_array = []
#ph_array = []

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
    

  x_ch = []


  for i in range(max_features):
      if i<e.nch:
        x_ch.append( [e.pt_ch[i], e.dr_ch[i] ] )
      else:
        x_ch.append( [0.0, 0.0] )





 # x_ph = []


 # for i in range(max_features):
 #     if i<e.nph:
 #       x_ph.append( [e.pt_ph[i], e.dr_ph[i] ] )
 #     else:
 #       x_ph.append( [0.0, 0.0] )


 # x = [ x_ch, x_ph ]

  xnp = np.array(x_ch, dtype=float)
  #xnp = np.array(x, dtype=float)

  X_array.append(xnp)

  qgl_array.append(e.qgl)
  pt_array.append(e.pt)
  eta_array.append(e.eta)




print 'Done looping.'

y = np.array(qg_array)
X = np.array(X_array)
qgl = np.array(qgl_array)
pt = np.array(pt_array)
eta = np.array(eta_array)

filename = 'qgNumpyRNN.npz'
f = open(filename,'w')
np.savez(f,X=X,y=y,qgl=qgl,pt=pt,eta=eta)
f.close()

print 'Saved numpy arrays to file: '+filename
