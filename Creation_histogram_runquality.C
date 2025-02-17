// ------------ Creation of pie charts saved in a PDF file showing the MC/data run quality, based on data from goodRuns.txt and badRuns.txt
#include <iostream>
#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TList.h>
#include <TMath.h>
#include <TPie.h>
#include <TStopwatch.h>
#include <TStyle.h>
#include <TSystem.h>
#include <string>
#include <TROOT.h>
#include <fstream>

using namespace std;

void Creation_histogram_runquality(string MC_prod, string period, string apass){
  
  cout << "MC_prod: " << MC_prod << endl;
  cout << "period: " << period << endl;
  cout << "apass: " << apass << endl;
    
  gROOT->SetBatch(kTRUE);
  gStyle->SetOptStat(0);
  
  //----------Input and Output directory - same directory of the config file
  char output_dir_name[1][200] = {"/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/pp900GeV2024_apass1"}; //same directory of the config file
  
  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;

  //---------- Read .txt file for good runs
  ifstream fileGood (Form("%s/goodRuns.txt",output_dir_name[0]));
  
  if(!fileGood){
   cout << "File GoodRuns not Found!" << endl;  
   return;
  }  
  
  int rungood_column = 0, Etagood_column = 0, Phigood_column = 0, Ptgood_column = 0; //file_good_index
  int count_goodruns = 0, count_GoodEtaPhi = 0, count_GoodEtaPhiPt = 0; //Good runs
  
  while(fileGood >> rungood_column >> Etagood_column >> Phigood_column >> Ptgood_column){
    //counters for different conditions  
    count_goodruns++; 
    if(Etagood_column==1 && Phigood_column==1 && Ptgood_column==0) count_GoodEtaPhi++;
    if(Etagood_column==1 && Phigood_column==1 && Ptgood_column==1) count_GoodEtaPhiPt++;
  }
  
  cout << "Tot good runs: " << count_goodruns << " |etaphi: " << count_GoodEtaPhi << " |also pt: " << count_GoodEtaPhiPt << endl;
  double percentages_good[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good[i] = (count_GoodEtaPhi/(double)count_goodruns) *100;
    else percentages_good[i] = (count_GoodEtaPhiPt/ (double)count_goodruns) *100;
   //cout << percentages_good[i] << endl; 
  }
  
  //---- TPie graph for good runs
  const char *labels_good[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good = new TCanvas("c_good", "Grafico Bad Runs", 800, 800);
  c_good->cd();
  TPie *pie_good = new TPie("pie_good", Form("Distribuzione Good Runs %s", period.c_str()), 2, percentages_good, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good->SetEntryLabel(i, "");
   pie_good->SetTextSize(0.03);
   pie_good->SetEntryFillColor(i, colors_good[i]);
  }
  pie_good->SetLabelsOffset(0.02);
  pie_good->Draw("3d");
  TLegend * leg_good = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good->SetTextSize(0.04);   
   leg_good->AddEntry(pie_good->GetSlice(i), Form("%s: %.1f%%", labels_good[i], percentages_good[i]), "f");
  }
  leg_good->Draw();
  
  c_good->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  //---------- Read .txt file for bad runs
  ifstream fileBad (Form("%s/badRuns.txt",output_dir_name[0]));
  
  if(!fileBad){
   cout << "File GoodRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_column = 0, Etabad_column = 0, Phibad_column = 0, Ptbad_column = 0; //file_bad_index
  int count_badruns = 0, count_BadEta = 0, count_BadPhi = 0, count_BadEtaPhi = 0; //Bad runs
  
  while(fileBad >> runbad_column >> Etabad_column >> Phibad_column >> Ptbad_column){
    //counters for different conditions  
    count_badruns++;
    if(Etabad_column==0 && Phibad_column==1) count_BadEta++;
    if(Etabad_column==1 && Phibad_column==0) count_BadPhi++;
    if(Etabad_column==0 && Phibad_column==0) count_BadEtaPhi++;  
  }
  
  cout << "Tot bad runs: " << count_badruns << " |eta: " << count_BadEta << " |phi: " << count_BadPhi << " |etaphi: " << count_BadEtaPhi << endl;
  
  double percentages_bad[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad[i] = (count_BadEta/ (double)count_badruns) *100;
   if(i==1) percentages_bad[i] = (count_BadPhi/ (double)count_badruns) *100;
   if(i==2) percentages_bad[i] = (count_BadEtaPhi/ (double)count_badruns) *100;
   //cout << percentages_bad[i] << endl;
  }
  
  //---- TPie graph for bad runs
  const char *labels_bad[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad = new TCanvas("c_bad", "Grafico Bad Runs", 800, 800);
  c_bad->cd();
  TPie *pie_bad = new TPie("pie_bad", Form("Distribuzione Bad Runs %s", period.c_str()), 3, percentages_bad, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad->SetEntryLabel(i, "");
   pie_bad->SetTextSize(0.03);
   pie_bad->SetEntryFillColor(i, colors_bad[i]);
  }
  pie_bad->SetLabelsOffset(0.02);
  pie_bad->Draw("3d");
  TLegend * leg_bad = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad->SetTextSize(0.04);   
   leg_bad->AddEntry(pie_bad->GetSlice(i), Form("%s: %.1f%%", labels_bad[i], percentages_bad[i]), "f");
  }
  leg_bad->Draw();
  
  c_bad->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_bad->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
 
  
}
