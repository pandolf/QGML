#! /usr/bin/python

import numpy as np
import ROOT
import json as simplejson

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import LSTM
from keras import optimizers

from sklearn.model_selection import train_test_split


filename = 'qgNumpyRNN.npz'

f = open(filename,'r')
npzf = np.load(f)
y = npzf['y']
X = npzf['X']

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




# build the model: a single LSTM
print('Build model...')
model = Sequential()
#model.add(Embedding((X.shape[1],X.shape[2]), 128))
#model.add(LSTM(128, dropout=0.2, recurrent_dropout=0.2))
model.add(LSTM(200,input_shape=(X.shape[1],X.shape[2]), dropout=0.2, recurrent_dropout=0.2))
model.add(Dense(1, activation='sigmoid'))


# try using different optimizers and different optimizer configs
model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

print('Train...')
#for x,y in zip(X_train, y_train):
#  model.train(np.array([x]),[y])

model.fit(X_train, y_train,
          batch_size=100,
          epochs=15,
          validation_data=(X_test, y_test))

score, acc = model.evaluate(X_test, y_test,
                            batch_size=100)
print('Test score:', score)
print('Test accuracy:', acc)




### saving the model
model_json = model.to_json()
with open("trainings/testQG_RNN.json", "w") as json_file:
    json_file.write(simplejson.dumps(simplejson.loads(model_json), indent=4))


##saving the training
model.save("trainings/testQG_RNN.h5")
