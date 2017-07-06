#! /usr/bin/python

import ROOT
import numpy as np
import json

import keras
from keras.models import Sequential




def getROC( h1_q, h1_g ):

  nbins=h1_q.GetXaxis().GetNbins()

  roc = ROOT.TGraph(0)

  for i in range(0,nbins):

    ibin = i+1
    eff_q = h1_q.Integral( nbins-ibin, nbins ) / h1_q.Integral( 1, nbins )
    eff_g = h1_g.Integral( nbins-ibin, nbins ) / h1_g.Integral( 1, nbins )

    roc.SetPoint(i, eff_q, 1.-eff_g )

  return roc



def main():

  setStyle()
  
  weightsfile = 'weights-improvement-10-0.73.hdf5'
  
  qg_rnn = keras.models.load_model(weightsfile)
  
  
  filename = 'qgNumpyRNN.npz'
  
  f = open(filename,'r')
  npzf = np.load(f)
  y = npzf['y']
  qgl = npzf['qgl']
  X = npzf['X']
  X = np.fliplr(X)
  
  n = len(y)
  n = 10000
  print 'Total entries: ' + str(n)
  
  outfile = ROOT.TFile.Open('prova.root', 'recreate')
  
  h1_qgl_q = ROOT.TH1D('qgl_q', '', 50, 0., 1.0001)
  h1_qgl_g = ROOT.TH1D('qgl_g', '', 50, 0., 1.0001)
  
  h1_rnn_q = ROOT.TH1D('rnn_q', '', 50, 0., 1.0001)
  h1_rnn_g = ROOT.TH1D('rnn_g', '', 50, 0., 1.0001)
  
  
  for i in range(n):
  
    if i%10000==0 :
      print 'Entry: %d / %d' %(i,n)
  
    rnn = qg_rnn.predict(X[i].reshape(-1,50,2),verbose=0)
  
    if y[i]==1:
      h1_qgl_q.Fill(qgl[i])
      h1_rnn_q.Fill(rnn)
    else:
      h1_qgl_g.Fill(qgl[i])
      h1_rnn_g.Fill(rnn)
  
  
  
  roc_qgl = getROC( h1_qgl_q, h1_qgl_g )
  roc_rnn = getROC( h1_rnn_q, h1_rnn_g )
  
  roc_qgl.SetMarkerStyle(20)
  roc_rnn.SetMarkerStyle(24)
  
  roc_qgl.SetMarkerSize(2)
  roc_rnn.SetMarkerSize(2)
  
  roc_qgl.SetMarkerColor(48)
  roc_rnn.SetMarkerColor(48)
  
  
  c1 = ROOT.TCanvas('c1', '', 600, 600)
  h2 = ROOT.TH2D('axes', '', 10, 0., 1.0001, 10, 0., 1.0001)
  h2.SetXTitle('Quark Jet Efficiency')
  h2.SetYTitle('Gluon Jet Rejection')
  h2.Draw()
  
  diag = ROOT.TLine(0.,1.,1.,0.)
  diag.Draw('same')
  
  roc_qgl.Draw('psame')
  roc_rnn.Draw('psame')
  
  c1.SaveAs('prova.eps')
  
  h1_qgl_q.Write() 
  h1_qgl_g.Write() 
  
  h1_rnn_q.Write() 
  h1_rnn_g.Write() 
  
  roc_qgl.Write()
  roc_rnn.Write()
  
  outfile.Close()



def setStyle():

  style = ROOT.TStyle("DrawBaseStyle", "");
  style.SetCanvasColor(0);
  style.SetPadColor(0);
  style.SetFrameFillColor(0);
  style.SetStatColor(0);
  style.SetOptStat(0);
  style.SetTitleFillColor(0);
  style.SetCanvasBorderMode(0);
  style.SetCanvasColor(0);
  style.SetCanvasDefH(600); #Height of canvas
  style.SetCanvasDefW(600); #Width of canvas
  style.SetCanvasDefX(0); #POsition on screen
  style.SetCanvasDefY(0);
  style.SetPadBorderMode(0);
  style.SetPadColor(0);
  style.SetPadGridX(False);
  style.SetPadGridY(False);
  style.SetGridColor(0);
  style.SetGridStyle(3);
  style.SetGridWidth(1);
  style.SetFrameBorderMode(0);
  style.SetFrameBorderSize(1);
  style.SetFrameFillColor(0);
  style.SetFrameFillStyle(0);
  style.SetFrameLineColor(1);
  style.SetFrameLineStyle(1);
  style.SetFrameLineWidth(1);
  # Margins:
  style.SetPadTopMargin(0.05);
  style.SetPadBottomMargin(0.15);#0.13);
  style.SetPadLeftMargin(0.15);#0.16);
  style.SetPadRightMargin(0.05);#0.02);
  # For the Global title:
  style.SetOptTitle(0);
  style.SetTitleFont(42);
  style.SetTitleColor(1);
  style.SetTitleTextColor(1);
  style.SetTitleFillColor(10);
  style.SetTitleFontSize(0.05);
  # For the axis titles:
  style.SetTitleColor(1, "XYZ");
  style.SetTitleFont(42, "XYZ");
  style.SetTitleSize(0.05, "XYZ");
  style.SetTitleXOffset(1.15);#0.9);
  style.SetTitleYOffset(1.5); # => 1.15 if exponents
  # For the axis labels:
  style.SetLabelColor(1, "XYZ");
  style.SetLabelFont(42, "XYZ");
  style.SetLabelOffset(0.007, "XYZ");
  style.SetLabelSize(0.045, "XYZ");
  # For the axis:
  style.SetAxisColor(1, "XYZ");
  style.SetStripDecimals(True);
  style.SetTickLength(0.03, "XYZ");
  style.SetNdivisions(510, "XYZ");
  #  style.SetPadTickX(1); # To get tick marks on the opposite side of the frame
  style.SetPadTickY(1);
  # for histograms:
  style.SetHistLineColor(1);
  # for the palette
  style.cd();



if __name__== "__main__":
  main()
