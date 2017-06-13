import numpy as np
import root_numpy as rn
import pandas as pd
from sklearn import svm
from sklearn.model_selection import train_test_split


a = rn.root2array('qgMiniTuple_pandas.root', 'qgLite')
A = pd.DataFrame(a)

y = A['isQuark']
X = A.drop(labels='isQuark', axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=0)


clf = svm.SVC(gamma=0.001, C=100.)

clf.fit(X_train, y_train)

y_pred = pd.Series(clf.predict(X_test))

correct = (y_pred == y_test.reset_index()['isQuark'])
totcorr = sum(correct)
acc = totcorr/float(len(y_pred))

print( "Accuracy = %d / %d = %.2f %%" % (totcorr,len(y_test),100.*acc) )
