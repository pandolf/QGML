import numpy as np
#import ROOT
import root_numpy as rn
import pandas as pd
from sklearn import svm


a = rn.root2array('qgMiniTuple_pandas.root', 'qgLite')
A = pd.DataFrame(a)

y = A['isQuark']
X = A.drop(labels='isQuark', axis=1)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=0)


clf = svm.SVC(gamma=0.001, C=100.)

clf.fit(X_train, y_train)

y_pred = clf.predict(X_test)
