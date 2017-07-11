#! /usr/bin/python


import numpy as np
import ROOT
import json as simplejson
import os,sys

import datetime

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import LSTM
from keras import optimizers
from keras.callbacks import ModelCheckpoint

from sklearn.model_selection import train_test_split


isDoubleRNN = False
if len(sys.argv) > 1 :
  isDoubleRNN = True




filename = 'qgNumpyRNN.npz'

f = open(filename,'r')
npzf = np.load(f)
y = npzf['y']
X = npzf['X']
X = np.fliplr(X) # RNN needs most important items last
#X = X.reshape(-1,1,60)

print 'Read data from file: ' + filename

#y = np.array(y)
#X = np.array(X)

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


X_train, X_test, y_train, y_test = train_test_split(X, y,test_size=0.2,random_state=42)



#train_frac = 0.8
#X_train = X.loc[:train_frac*Nrows, :]
#y_train = y.loc[:train_frac*Nrows]
#X_test  = X.loc[ train_frac*Nrows:, :]
#y_test  = y.loc[ train_frac*Nrows:]


#model paramters:
units = 200
dropout = 0.2
recurrent_dropout = 0.2
batch_size = 100
epochs = 100


# build the model: a single LSTM
print('Building LSTM...')
print('  units = %d' % units)
print('  dropout = %f' % dropout)
print('  recurrent_dropout = %f' % recurrent_dropout)
print('  batch_size = %d' % batch_size)
print('  epochs = %d' % epochs)


model = Sequential()


if isDoubleRNN:
  #rnn_ch = TimeDistributed(LSTM(units), input_shape=(n_cands, n_feats))(inp)
  #rnn_ph = 
  #  inp = Input(shape=(None, n_cands, n_feats))
  #  embedding1 = TimeDistributed(LSTM(embedding1_size), input_shape=(n_cands, n_feats))(inp)
  #  embedding2 = LSTM(embedding2_size)(embedding1)
  #merged = Merge([rnn_ch, rnn_ph], mode='concat')
  #model.add(merged)
  i=1
else:
  #model.add(LSTM(units,input_shape=(X[:,1].shape[1:]), dropout=dropout, recurrent_dropout=recurrent_dropout))
  model.add(LSTM(units,input_shape=(X.shape[1:]), dropout=dropout, recurrent_dropout=recurrent_dropout))


model.add(Dense(1, activation='sigmoid'))


model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

print('Train...')


folder = ('trainingsRNN/%d-%d-%d' % (datetime.date.today().year,datetime.date.today().month,datetime.date.today().day))

os.system('mkdir -p '+folder)

filepath=folder+"/weights-improvement-{epoch:02d}-{val_acc:.2f}.hdf5"
checkpoint = ModelCheckpoint(filepath, monitor='val_acc', verbose=1, save_best_only=True, mode='max')
callbacks_list = [checkpoint]

history = model.fit(X_train, y_train,
                    batch_size=batch_size,
                    epochs=epochs,
                    callbacks=callbacks_list,
                    validation_data=(X_test, y_test))

#score, acc = model.evaluate(X_test, y_test,
#                            batch_size=batch_size)

#print('Test score:', score)
#print('Test accuracy:', acc)


trainfile = ROOT.TFile.Open(folder+'/train.root', 'recreate')

gr_acc      = ROOT.TGraph(0)
gr_loss     = ROOT.TGraph(0)
gr_val_acc  = ROOT.TGraph(0)
gr_val_loss = ROOT.TGraph(0)

for i in range(epochs):

  gr_acc     .SetPoint(i, i, history.history['acc']     [i])
  gr_loss    .SetPoint(i, i, history.history['loss']    [i])
  gr_val_acc .SetPoint(i, i, history.history['val_acc'] [i])
  gr_val_loss.SetPoint(i, i, history.history['val_loss'][i])

gr_acc     .SetName('acc')
gr_loss    .SetName('loss')
gr_val_acc .SetName('val_acc')
gr_val_loss.SetName('val_loss')

gr_acc     .Write()
gr_loss    .Write()
gr_val_acc .Write()
gr_val_loss.Write()

trainfile.Close()


### saving the model
model_json = model.to_json()
with open("trainingsRNN/testQG_RNN.json", "w") as json_file:
    json_file.write(simplejson.dumps(simplejson.loads(model_json), indent=4))


##saving the training
model.save(folder+'/final_RNN.hdf5')
