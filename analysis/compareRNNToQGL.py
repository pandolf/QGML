#! /usr/bin/python

import ROOT
import numpy as np
import json
import os

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

  rnn_training = 'improvement-10-0.73'
  
  weightsfile = 'trainingsRNN/weights-'+rnn_training+'.hdf5'

  figsfolder = 'figures_'+rnn_training

  os.system('mkdir -p '+figsfolder)
  
  qg_rnn = keras.models.load_model(weightsfile)
  
  
  filename = 'qgNumpyRNN.npz'
  
  f = open(filename,'r')
  npzf = np.load(f)
  y = npzf['y']
  qgl = npzf['qgl']
  pt = npzf['pt']
  eta = npzf['eta']
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

    if (pt[i]<300.) : continue
    if (pt[i]>400.) : continue
    if (abs(eta[i])>1.3) : continue
  
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
  
  roc_qgl.SetMarkerSize(1.6)
  roc_rnn.SetMarkerSize(1.6)
  
  roc_qgl.SetMarkerColor(48)
  roc_rnn.SetMarkerColor(48)
  
  
  c1 = ROOT.TCanvas('c1', '', 600, 600)
  h2 = ROOT.TH2D('axes0', '', 10, 0., 1.0001, 10, 0., 1.0001)
  h2.SetXTitle('Quark Jet Efficiency')
  h2.SetYTitle('Gluon Jet Rejection')
  h2.Draw()

  (label_top,label_cms)=getLabels()
  label_top.Draw('same')
  label_cms.Draw('same')
  
  diag = ROOT.TLine(0.,1.,1.,0.)
  diag.Draw('same')

  legend = ROOT.TLegend(0.2, 0.25, 0.5, 0.45)
  legend.SetFillColor(0)
  legend.SetTextSize(0.038)
  legend.SetHeader("300 < p_{T} < 400 GeV, |#eta|<1.3")
  legend.AddEntry( roc_qgl, "QG LD", "P" )
  legend.AddEntry( roc_rnn, "QG RNN", "P" )
  legend.Draw("same")
  
  roc_qgl.Draw('psame')
  roc_rnn.Draw('psame')
  
  c1.SaveAs(figsfolder+'/rnn_vs_qgl_pt300_400_eta0p0_1p3.eps')
  c1.SaveAs(figsfolder+'/rnn_vs_qgl_pt300_400_eta0p0_1p3.pdf')

  drawQvsG( figsfolder, h1_qgl_q, h1_qgl_g, 'qgl', 'QG Likelihood Discriminant' )
  drawQvsG( figsfolder, h1_rnn_q, h1_rnn_g, 'rnn', 'QG RNN' )

  h1_qgl_q.Write() 
  h1_qgl_g.Write() 
  
  h1_rnn_q.Write() 
  h1_rnn_g.Write() 
  
  roc_qgl.Write()
  roc_rnn.Write()
  
  outfile.Close()



def drawQvsG( folder, h1_q, h1_g, savename, name='' ):

  max_g = h1_g.GetMaximum()/h1_g.Integral()
  max_q = h1_q.GetMaximum()/h1_q.Integral()
  max_y = max(max_g,max_q)*1.2

  c1 = ROOT.TCanvas('c2','',600,600)
  c1.cd()

  h2 = ROOT.TH2D('axes','', 10, 0., 1.0001, 10, 0., max_y)
  h2.SetXTitle(name)
  h2.SetYTitle('Normalized to Unity')
  h2.Draw()

  (label_top,label_cms)=getLabels()
  label_top.Draw('same')
  label_cms.Draw('same')

  h1_q.SetFillColor(38)
  h1_g.SetFillColor(46)

  h1_q.SetLineColor(38)
  h1_g.SetLineColor(46)

  h1_q.SetFillStyle(3004)
  h1_g.SetFillStyle(3005)

  h1_q.SetLineWidth(2)
  h1_g.SetLineWidth(2)

  legend = ROOT.TLegend(0.35,0.7,0.63,0.92)
  legend.SetFillColor(0)
  legend.SetTextSize(0.038)
  legend.SetHeader("300 < p_{T} < 400 GeV, |#eta| < 1.3")
  legend.AddEntry(h1_q, 'Quark Jets', 'F')
  legend.AddEntry(h1_g, 'Gluon Jets', 'F')
  legend.Draw('same')

  h1_g.DrawNormalized('same')
  h1_q.DrawNormalized('same')

  c1.SaveAs(folder+'/'+savename+'_pt300_400_eta0p0_1p3.eps')
  c1.SaveAs(folder+'/'+savename+'_pt300_400_eta0p0_1p3.pdf')

   
def getLabels():

  label_top = ROOT.TPaveText(0.4,0.959,0.975,0.963, "brNDC")
  label_top.SetBorderSize(0)
  label_top.SetFillColor(0)
  label_top.SetTextSize(0.038)
  label_top.SetTextAlign(31)
  label_top.SetTextFont(42)
  label_top.AddText("#sqrt{s} = 13 TeV")
  label_top.Draw("same")


  label_cms = ROOT.TPaveText(0.143,0.96,0.27,0.965, "brNDC")
  label_cms.SetBorderSize(0)
  label_cms.SetFillColor(0)
  label_cms.SetTextSize(0.042)
  label_cms.SetTextAlign(11)
  label_cms.SetTextFont(42)
  label_cms.AddText( "CMS Simulation" )
  label_cms.Draw('same')

  return (label_top,label_cms)



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
