#! /usr/bin/python

import ROOT
import numpy as np
import json

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import Conv1D, GlobalMaxPooling1D



weightsfile = 'trainings/testQG_CNN.h5'

qg_cnn = keras.models.load_model(weightsfile)


f = ROOT.TFile.Open('qgMiniTuple_lite.root')
t = f.Get('qgLite')

h1_q = ROOT.TH1D('qgl_q', '', 30, 0., 1.0001)
h1_g = ROOT.TH1D('qgl_g', '', 30, 0., 1.0001)


for e in t:

  qgl = e.qgl

  isQuark = e.isQuark

  x = []
  x.append( e.chImageLite_0 ) # yes, i actually did this
  x.append( e.chImageLite_1 )
  x.append( e.chImageLite_2 )
  x.append( e.chImageLite_3 )
  x.append( e.chImageLite_4 )
  x.append( e.chImageLite_5 )
  x.append( e.chImageLite_6 )
  x.append( e.chImageLite_7 )
  x.append( e.chImageLite_8 )
  x.append( e.chImageLite_9 )
  x.append( e.chImageLite_10 )
  x.append( e.chImageLite_11 )
  x.append( e.chImageLite_12 )
  x.append( e.chImageLite_13 )
  x.append( e.chImageLite_14 )
  x.append( e.chImageLite_15 )
  x.append( e.chImageLite_16 )
  x.append( e.chImageLite_17 )
  x.append( e.chImageLite_18 )
  x.append( e.chImageLite_19 )
  x.append( e.chImageLite_20 )
  x.append( e.chImageLite_21 )
  x.append( e.chImageLite_22 )
  x.append( e.chImageLite_23 )
  x.append( e.chImageLite_24 )
  x.append( e.chImageLite_25 )
  x.append( e.chImageLite_26 )
  x.append( e.chImageLite_27 )
  x.append( e.chImageLite_28 )
  x.append( e.chImageLite_29 )
  x.append( e.chImageLite_30 )
  x.append( e.chImageLite_31 )
  x.append( e.chImageLite_32 )
  x.append( e.chImageLite_33 )
  x.append( e.chImageLite_34 )
  x.append( e.chImageLite_35 )
  x.append( e.chImageLite_36 )
  x.append( e.chImageLite_37 )
  x.append( e.chImageLite_38 )
  x.append( e.chImageLite_39 )
  x.append( e.chImageLite_40 )
  x.append( e.chImageLite_41 )
  x.append( e.chImageLite_42 )
  x.append( e.chImageLite_43 )
  x.append( e.chImageLite_44 )
  x.append( e.chImageLite_45 )
  x.append( e.chImageLite_46 )
  x.append( e.chImageLite_47 )
  x.append( e.chImageLite_48 )
  x.append( e.chImageLite_49 )
  x.append( e.chImageLite_50 )
  x.append( e.chImageLite_51 )
  x.append( e.chImageLite_52 )
  x.append( e.chImageLite_53 )
  x.append( e.chImageLite_54 )
  x.append( e.chImageLite_55 )
  x.append( e.chImageLite_56 )
  x.append( e.chImageLite_57 )
  x.append( e.chImageLite_58 )
  x.append( e.chImageLite_59 )

  xnp = np.array([x])

  cnn = qg_cnn.predict(xnp,verbose=0)
  print( cnn )

  if isQuark:
    h1_q.Fill(qgl)
  else:
    h1_g.Fill(qgl)

  
