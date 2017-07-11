#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "interface/QGMLCommon.h"





int main( int argc, char* argv[] ) {

  if( argc<2 ) {
  
    std::cout << "Please provide training name. (eg. 2017-7-6)" << std::endl;
    exit(1);

  }

  QGMLCommon::setStyle();

  std::string training(argv[1]);

  std::string dir = "trainingsRNN/" + training;

  std::string trainfile = dir + "/train.root";

  TFile* file = TFile::Open(trainfile.c_str(), "read");

  TGraph* gr_acc      = (TGraph*)file->Get("acc"     );
  TGraph* gr_loss     = (TGraph*)file->Get("loss"    );
  TGraph* gr_val_acc  = (TGraph*)file->Get("val_acc" );
  TGraph* gr_val_loss = (TGraph*)file->Get("val_loss");

  gr_acc     ->SetLineColor(38);
  gr_loss    ->SetLineColor(46);
  gr_val_acc ->SetLineColor(38);
  gr_val_loss->SetLineColor(46);

  gr_acc     ->SetLineWidth(3);
  gr_loss    ->SetLineWidth(3);
  gr_val_acc ->SetLineWidth(3);
  gr_val_loss->SetLineWidth(3);

  gr_acc     ->SetLineStyle(2);
  gr_loss    ->SetLineStyle(2);
  gr_val_acc ->SetLineStyle(1);
  gr_val_loss->SetLineStyle(1);

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  int nEpochs = gr_acc->GetN();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., nEpochs, 10, 0., 1.0001 );
  h2_axes->SetXTitle( "Epoch Number" );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.4, 0.72, 0.6, 0.92 );
  legend->SetFillColor(0);
  legend->SetTextSize(0.032);
  legend->AddEntry( gr_acc     , "Train Accuracy", "L" );
  legend->AddEntry( gr_loss    , "Train Loss"    , "L" );
  legend->AddEntry( gr_val_acc , "Test Accuracy" , "L" );
  legend->AddEntry( gr_val_loss, "Test Loss"     , "L" );
  legend->Draw("same");

  TPaveText* label_top = QGMLCommon::get_labelTop();
  TPaveText* label_cms = QGMLCommon::get_labelCMS();
  label_top->Draw("same");
  label_cms->Draw("same");

  gr_acc      ->Draw("L same");
  gr_loss     ->Draw("L same");
  gr_val_acc  ->Draw("L same");
  gr_val_loss ->Draw("L same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/learningCurve.eps", dir.c_str()) ); 
  c1->SaveAs( Form("%s/learningCurve.pdf", dir.c_str()) ); 

  return 0;

}
    
