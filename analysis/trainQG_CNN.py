#! /usr/bin/python

import numpy as np
import ROOT
import json as simplejson

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import Conv1D, GlobalMaxPooling1D

from sklearn.cross_validation import train_test_split



f = ROOT.TFile.Open('qgMiniTupleLite.root', 'read')
t = f.Get('qgLite')

n = t.GetEntries()

qg_array = []
var_array = []

for e in t:

  qg_array.append( e.isQuark )

  #x = []
  #x.append( e.chImageLite_0 ) # yes, i actually did this
  #x.append( e.chImageLite_1 )
  #x.append( e.chImageLite_2 )
  #x.append( e.chImageLite_3 )
  #x.append( e.chImageLite_4 )
  #x.append( e.chImageLite_5 )
  #x.append( e.chImageLite_6 )
  #x.append( e.chImageLite_7 )
  #x.append( e.chImageLite_8 )
  #x.append( e.chImageLite_9 )
  #x.append( e.chImageLite_10 )
  #x.append( e.chImageLite_11 )
  #x.append( e.chImageLite_12 )
  #x.append( e.chImageLite_13 )
  #x.append( e.chImageLite_14 )
  #x.append( e.chImageLite_15 )
  #x.append( e.chImageLite_16 )
  #x.append( e.chImageLite_17 )
  #x.append( e.chImageLite_18 )
  #x.append( e.chImageLite_19 )
  #x.append( e.chImageLite_20 )
  #x.append( e.chImageLite_21 )
  #x.append( e.chImageLite_22 )
  #x.append( e.chImageLite_23 )
  #x.append( e.chImageLite_24 )
  #x.append( e.chImageLite_25 )
  #x.append( e.chImageLite_26 )
  #x.append( e.chImageLite_27 )
  #x.append( e.chImageLite_28 )
  #x.append( e.chImageLite_29 )
  #x.append( e.chImageLite_30 )
  #x.append( e.chImageLite_31 )
  #x.append( e.chImageLite_32 )
  #x.append( e.chImageLite_33 )
  #x.append( e.chImageLite_34 )
  #x.append( e.chImageLite_35 )
  #x.append( e.chImageLite_36 )
  #x.append( e.chImageLite_37 )
  #x.append( e.chImageLite_38 )
  #x.append( e.chImageLite_39 )
  #x.append( e.chImageLite_40 )
  #x.append( e.chImageLite_41 )
  #x.append( e.chImageLite_42 )
  #x.append( e.chImageLite_43 )
  #x.append( e.chImageLite_44 )
  #x.append( e.chImageLite_45 )
  #x.append( e.chImageLite_46 )
  #x.append( e.chImageLite_47 )
  #x.append( e.chImageLite_48 )
  #x.append( e.chImageLite_49 )
  #x.append( e.chImageLite_50 )
  #x.append( e.chImageLite_51 )
  #x.append( e.chImageLite_52 )
  #x.append( e.chImageLite_53 )
  #x.append( e.chImageLite_54 )
  #x.append( e.chImageLite_55 )
  #x.append( e.chImageLite_56 )
  #x.append( e.chImageLite_57 )
  #x.append( e.chImageLite_58 )
  #x.append( e.chImageLite_59 )

  #var_array.append( x )
  var_array.append( e.chImageLite )


y = np.array(qg_array)
X = np.array(var_array)

#seed=13
#np.random.seed(seed)
#np.random.shuffle(y_np)
#np.random.seed(seed)
#np.random.shuffle(X_np)

#y = pd.Series(y_np)
#X = pd.DataFrame(X_np)

#A = pd.DataFrame(a)
#y = A['isQuark']
#X = A.drop(labels='isQuark', axis=1)


(Nrows, Nvars) = X.shape

X_train, X_test, y_train, y_test = train_test_split(X, y,test_size=0.2,random_state=42)



#train_frac = 0.7
#X_train = X.loc[:train_frac*Nrows, :]
#y_train = y.loc[:train_frac*Nrows]
#X_test  = X.loc[ train_frac*Nrows:, :]
#y_test  = y.loc[ train_frac*Nrows:]
#
#y_test = y_test.reset_index()
#y_test = y_test[0]
#
#X_test = X_test.reset_index().drop('index', 1)
#


filters = 30
kernel_size = 3
#hidden_dims = 250
epochs = 1
max_features = Nvars
maxlen = Nvars
embedding_dims = 50

model = Sequential()                                               
print('building model...')
model.add(Embedding(max_features,
                    embedding_dims,
                    input_length=maxlen))
print('added embedding...')
model.add(Dropout(0.2))
print('added dropout...')
model.add(Conv1D(filters=filters, #filters
                 kernel_size=kernel_size, #kernel_size
                 padding='valid',
                 activation='relu',
                 strides=1))
print('added conv1d...')
model.add(GlobalMaxPooling1D())
print('added maxpooling...')




#model.add(Dense(hidden_dims))
#model.add(Dropout(0.2))
#model.add(Activation('relu'))
model.add(Dense(60))
model.add(Dropout(0.2))
model.add(Activation('relu'))

print('added vanilla hidden layer...')
# We project onto a single unit output layer, and squash it with a sigmoid:
model.add(Dense(1))
model.add(Activation('sigmoid'))
print('added last output layer')


model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

model.fit(X_train, y_train,
          #batch_size=32,
          epochs=epochs,
          validation_data=(X_test, y_test))


### saving the model
model_json = model.to_json()
with open("trainings/testQG_CNN.json", "w") as json_file:
    json_file.write(simplejson.dumps(simplejson.loads(model_json), indent=4))


##saving the training
model.save("trainings/testQG_CNN.h5")
