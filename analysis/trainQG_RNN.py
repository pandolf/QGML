#! /usr/bin/python


import numpy as np
import ROOT
import json as simplejson
import os

import datetime

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import LSTM
from keras import optimizers
from keras.callbacks import ModelCheckpoint

from sklearn.model_selection import train_test_split


filename = 'qgNumpyRNN.npz'

f = open(filename,'r')
npzf = np.load(f)
y = npzf['y']
X = npzf['X']
X = np.fliplr(X)
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
model.add(LSTM(units,input_shape=(X.shape[1:]), dropout=dropout, recurrent_dropout=recurrent_dropout))
model.add(Dense(1, activation='sigmoid'))


model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

print('Train...')


folder = ('%d-%d-%d' % (datetime.date.today().year,datetime.date.today().month,datetime.date.today().day))

os.system('mkdir -p '+folder)

filepath="trainingsRNN/"+folder+"/weights-improvement-{epoch:02d}-{val_acc:.2f}.hdf5"
checkpoint = ModelCheckpoint(filepath, monitor='val_acc', verbose=1, save_best_only=True, mode='max')
callbacks_list = [checkpoint]

model.fit(X_train, y_train,
          batch_size=batch_size,
          epochs=epochs,
          callbacks=callbacks_list,
          validation_data=(X_test, y_test))

score, acc = model.evaluate(X_test, y_test,
                            batch_size=batch_size)
print('Test score:', score)
print('Test accuracy:', acc)




### saving the model
model_json = model.to_json()
with open("trainingsRNN/testQG_RNN.json", "w") as json_file:
    json_file.write(simplejson.dumps(simplejson.loads(model_json), indent=4))


##saving the training
model.save('trainingsRNN/'+folder+'/final_RNN.hdf5')
