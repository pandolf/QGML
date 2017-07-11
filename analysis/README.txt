first of all you need an ntuple produced by QGDev (as an example see lxplus:/afs/cern.ch/work/p/pandolf/public/qgMiniTuple.root)



*** FOR RNN:

- convert the root file into a numpy file:
  
   python createNumpyFile.py  [filename, default is 'qgMiniTuple.root']


- train the RNN (as it is now it runs overnight on 100k events, on a crappy
  laptop CPU)
  
   python trainQG_RNN.py

  the above will save all the weights (also intermediate NNs) to a folder
which has the name of the date of the training, eg 2017-7-11. it will also
save the learning curve info (acc and loss for train and test) in a rootfile
in that directory

- plot the learning curve:

  make drawRNNTrainTest; ./drawRNNTrainTest [trainingName <- this is usually
the date of the training, eg '2017-7-11']

- compare to the QG Lidelihood Discriminator:

  python compareRNNToQGL.py [trainingName, same as above]



*** FOR CNN:

- make some preliminary plots, just for fun (jet images etc):

  make drawQGML; ./drawQGML

- convert the root file into a numpy file:
  
  make createTreeLite; ./createTreeLite

- train the CNN:

  python trainQG_CNN.py
