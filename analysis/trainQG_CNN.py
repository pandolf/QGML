import numpy as np
import root_numpy as rn
import pandas as pd

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Embedding
from keras.layers import Conv1D, GlobalMaxPooling1D



a = rn.root2array('qgMiniTuple_pandas.root', 'qgLite')
np.random.seed(14)
np.random.shuffle(a)
A = pd.DataFrame(a)

y = A['isQuark']
X = A.drop(labels='isQuark', axis=1)

train_frac = 0.7

(Nrows, Nvars) = X.shape

X_train = X.loc[:train_frac*Nrows]
y_train = y.loc[:train_frac*Nrows]
X_test  = X.loc[ train_frac*Nrows:]
y_test  = y.loc[ train_frac*Nrows:]



filters = 30
kernel_size = 3
#hidden_dims = 250
#epochs = 2
max_features = Nvars
maxlen = Nvars
embedding_dims = 50

model = Sequential()                                               
model.add(Embedding(max_features,
                    embedding_dims,
                    input_length=maxlen))
model.add(Dropout(0.2))
model.add(Conv1D(filters=filters, #filters
                 kernel_size=kernel_size, #kernel_size
                 padding='valid',
                 activation='relu',
                 strides=2))
model.add(GlobalMaxPooling1D())

#model.add(Dense(hidden_dims))
#model.add(Dropout(0.2))
#model.add(Activation('relu'))

model.compile(loss='binary_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

model.fit(X_train, y_train,
          batch_size=32,
          epochs=2,
          validation_data=(X_test, y_test))

#y_pred = pd.Series(clf.predict(X_test))
#
#correct = (y_pred == y_test.reset_index()['isQuark'])
#totcorr = sum(correct)
#acc = totcorr/float(len(y_pred))
#
#print( "Accuracy = %d / %d = %.2f %%" % (totcorr,len(y_test),100.*acc) )
