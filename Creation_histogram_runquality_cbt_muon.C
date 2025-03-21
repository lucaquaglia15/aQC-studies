// ------------ Creation of pie charts saved in a PDF file showing the MC/data run quality, based on data from goodRuns_thresholds_sigmas_cbt_muon.txt and badRuns_thresholds_sigmas_cbt_muon.txt
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
  char output_dir_name[1][200] = {"/home/sarapc/Desktop/MID_QC/ComparisonDataMC_A02D/PbPb2024_apass1"}; //same directory of the config file

  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;

  //---------- Read .txt file for good runs
  ifstream fileGood_thresholds_sigmas (Form("%s/goodRuns_thresholds_sigmas_cbt_muon.txt",output_dir_name[0])); //thresholds + sigmas
  
  if(!fileGood_thresholds_sigmas){
   cout << "File Good_thresholds_sigmasRuns not Found!" << endl;  
   return;
  }  
  
  int rungood_thresholds_sigmas_column = 0, Etagood_thresholds_sigmas_column = 0, Phigood_thresholds_sigmas_column = 0, Ptgood_thresholds_sigmas_column = 0; //file_good_thresholds_sigmas_index
  int count_good_thresholds_sigmasruns = 0, count_Good_thresholds_sigmasEtaPhi = 0, count_Good_thresholds_sigmasEtaPhiPt = 0; //Good_thresholds_sigmas runs
  
  while(fileGood_thresholds_sigmas >> rungood_thresholds_sigmas_column >> Etagood_thresholds_sigmas_column >> Phigood_thresholds_sigmas_column >> Ptgood_thresholds_sigmas_column){
    //counters for different conditions  
    count_good_thresholds_sigmasruns++; 
    if(Etagood_thresholds_sigmas_column==1 && Phigood_thresholds_sigmas_column==1 && Ptgood_thresholds_sigmas_column==0) count_Good_thresholds_sigmasEtaPhi++;
    if(Etagood_thresholds_sigmas_column==1 && Phigood_thresholds_sigmas_column==1 && Ptgood_thresholds_sigmas_column==1) count_Good_thresholds_sigmasEtaPhiPt++;
  }
  
  cout << "Tot good_thresholds_sigmas runs: " << count_good_thresholds_sigmasruns << " |etaphi: " << count_Good_thresholds_sigmasEtaPhi << " |also pt: " << count_Good_thresholds_sigmasEtaPhiPt << endl;
  double percentages_good_thresholds_sigmas[2] = {0.,0.};
  
  for(int i=0;i<2;i++){
   if(i==0) percentages_good_thresholds_sigmas[i] = (count_Good_thresholds_sigmasEtaPhi/(double)count_good_thresholds_sigmasruns) *100;
    else percentages_good_thresholds_sigmas[i] = (count_Good_thresholds_sigmasEtaPhiPt/ (double)count_good_thresholds_sigmasruns) *100;
   //cout << percentages_good_thresholds_sigmas[i] << endl; 
  }
  
  //---- TPie graph for good_thresholds_sigmas runs
  const char *labels_good_thresholds_sigmas[2] = {"#eta&&#phi GOOD", "#eta&&#phi&&p_{T} GOOD"};  // labels
  int colors_good_thresholds_sigmas[2] = {kGreen+2,kBlue};
  
  TCanvas *c_good_thresholds_sigmas = new TCanvas("c_good_thresholds_sigmas", "Grafico Good_thresholds_sigmas Runs", 800, 800);
  c_good_thresholds_sigmas->cd();
  TPie *pie_good_thresholds_sigmas = new TPie("pie_good_thresholds_sigmas", Form("Good_thresholds_sigmas Runs %s", period.c_str()), 2, percentages_good_thresholds_sigmas, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_good_thresholds_sigmas->SetEntryLabel(i, "");
   pie_good_thresholds_sigmas->SetTextSize(0.03);
   pie_good_thresholds_sigmas->SetEntryFillColor(i, colors_good_thresholds_sigmas[i]);
  }
  pie_good_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_good_thresholds_sigmas->Draw("3d");
  TLegend * leg_good_thresholds_sigmas = new TLegend(0.25,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_good_thresholds_sigmas->SetTextSize(0.04);   
   leg_good_thresholds_sigmas->AddEntry(pie_good_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_good_thresholds_sigmas[i], percentages_good_thresholds_sigmas[i]), "f");
  }
  leg_good_thresholds_sigmas->Draw();
  
  c_good_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s_cbt_muon.pdf[",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_good_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s_cbt_muon.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
  //---------- Read .txt file for bad runs
  ifstream fileBad_thresholds_sigmas (Form("%s/badRuns_thresholds_sigmas_cbt_muon.txt",output_dir_name[0])); //thresholds + sigmas
  
  if(!fileBad_thresholds_sigmas){
   cout << "File BadRuns not Found!" << endl;  
   return;
  } 
  
  int runbad_thresholds_sigmas_column = 0, Etabad_thresholds_sigmas_column = 0, Phibad_thresholds_sigmas_column = 0, Ptbad_thresholds_sigmas_column = 0; //file_bad_thresholds_sigmas_index
  int count_bad_thresholds_sigmasruns = 0, count_Bad_thresholds_sigmasEta = 0, count_Bad_thresholds_sigmasPhi = 0, count_Bad_thresholds_sigmasEtaPhi = 0; //Bad_thresholds_sigmas runs
  
  while(fileBad_thresholds_sigmas >> runbad_thresholds_sigmas_column >> Etabad_thresholds_sigmas_column >> Phibad_thresholds_sigmas_column >> Ptbad_thresholds_sigmas_column){
    //counters for different conditions  
    count_bad_thresholds_sigmasruns++;
    if(Etabad_thresholds_sigmas_column==0 && Phibad_thresholds_sigmas_column==1) count_Bad_thresholds_sigmasEta++;
    if(Etabad_thresholds_sigmas_column==1 && Phibad_thresholds_sigmas_column==0) count_Bad_thresholds_sigmasPhi++;
    if(Etabad_thresholds_sigmas_column==0 && Phibad_thresholds_sigmas_column==0) count_Bad_thresholds_sigmasEtaPhi++;  
  }
  
  cout << "Tot bad_thresholds_sigmas runs: " << count_bad_thresholds_sigmasruns << " |eta: " << count_Bad_thresholds_sigmasEta << " |phi: " << count_Bad_thresholds_sigmasPhi << " |etaphi: " << count_Bad_thresholds_sigmasEtaPhi << endl;
  
  double percentages_bad_thresholds_sigmas[3] = {0.,0.,0.};
  
  for(int i=0;i<3;i++){
   if(i==0) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasEta/ (double)count_bad_thresholds_sigmasruns) *100;
   if(i==1) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasPhi/ (double)count_bad_thresholds_sigmasruns) *100;
   if(i==2) percentages_bad_thresholds_sigmas[i] = (count_Bad_thresholds_sigmasEtaPhi/ (double)count_bad_thresholds_sigmasruns) *100;
   //cout << percentages_bad_thresholds_sigmas[i] << endl;
  }
  
  //---- TPie graph for bad_thresholds_sigmas runs
  const char *labels_bad_thresholds_sigmas[3] = {"#eta BAD", "#phi BAD", "#eta&&#phi BAD"};
  int colors_bad_thresholds_sigmas[3] = {kOrange+7, kYellow+1, kRed+2};
  
  TCanvas *c_bad_thresholds_sigmas = new TCanvas("c_bad_thresholds_sigmas", "Grafico Bad_thresholds_sigmas Runs", 800, 800);
  c_bad_thresholds_sigmas->cd();
  TPie *pie_bad_thresholds_sigmas = new TPie("pie_bad_thresholds_sigmas", Form("Bad_thresholds_sigmas Runs %s", period.c_str()), 3, percentages_bad_thresholds_sigmas, nullptr);
  for (int i = 0; i < 3; i++) {
   pie_bad_thresholds_sigmas->SetEntryLabel(i, "");
   pie_bad_thresholds_sigmas->SetTextSize(0.03);
   pie_bad_thresholds_sigmas->SetEntryFillColor(i, colors_bad_thresholds_sigmas[i]);
  }
  pie_bad_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_bad_thresholds_sigmas->Draw("3d");
  TLegend * leg_bad_thresholds_sigmas = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 3; i++) {
   leg_bad_thresholds_sigmas->SetTextSize(0.04);   
   leg_bad_thresholds_sigmas->AddEntry(pie_bad_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_bad_thresholds_sigmas[i], percentages_bad_thresholds_sigmas[i]), "f");
  }
  leg_bad_thresholds_sigmas->Draw();
  
  c_bad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s_cbt_muon.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  

  //Total number of runs
  int count_totRuns = count_good_thresholds_sigmasruns + count_bad_thresholds_sigmasruns; //is the same for all configurations

  const char *labels_qualityGeneral[2] = {"GOOD", "BAD"};
  int colors_qualityGeneral[2] = {kGreen+2, kRed+2};

  //Charts to show percentages of good/bad runs
  double percentages_goodBad_thresholds_sigmas[2] = {0.,0.};

  for(int i=0;i<2;i++){
    if(i==0) percentages_goodBad_thresholds_sigmas[i] = (count_good_thresholds_sigmasruns/ (double)count_totRuns) *100;
    if(i==1) percentages_goodBad_thresholds_sigmas[i] = (count_bad_thresholds_sigmasruns/ (double)count_totRuns) *100;
    //cout << percentages_goodBad_chisquare[i] << endl;
  }
  
  //Good/bad thresholds + sigmas
  TCanvas *c_goodbad_thresholds_sigmas = new TCanvas("c_goodbad_thresholds_sigmas", "Grafico Good/Bad Runs thresholds_sigmas", 800, 800);
  c_goodbad_thresholds_sigmas->cd();
  TPie *pie_goodbad_thresholds_sigmas = new TPie("pie_goodbad_thresholds_sigmas", Form("Good Bad Runs_thresholds_sigmas %s", period.c_str()), 2, percentages_goodBad_thresholds_sigmas, nullptr);
  for (int i = 0; i < 2; i++) {
   pie_goodbad_thresholds_sigmas->SetEntryLabel(i, "");
   pie_goodbad_thresholds_sigmas->SetTextSize(0.03);
   pie_goodbad_thresholds_sigmas->SetEntryFillColor(i, colors_qualityGeneral[i]);
  }
  pie_goodbad_thresholds_sigmas->SetLabelsOffset(0.02);
  pie_goodbad_thresholds_sigmas->Draw("3d");
  TLegend * leg_goodbad_thresholds_sigmas = new TLegend(0.45,0.75,0.95,0.9);
  for (int i = 0; i < 2; i++) {
   leg_goodbad_thresholds_sigmas->SetTextSize(0.04);   
   leg_goodbad_thresholds_sigmas->AddEntry(pie_goodbad_thresholds_sigmas->GetSlice(i), Form("%s: %.1f%%", labels_qualityGeneral[i], percentages_goodBad_thresholds_sigmas[i]), "f");
  }
  leg_goodbad_thresholds_sigmas->Draw();
  
  c_goodbad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s_cbt_muon.pdf",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  c_goodbad_thresholds_sigmas->Print(Form("%s/QualityHisto_MC_%s_data_%s_%s_cbt_muon.pdf]",output_dir_name[0],MC_prod.c_str(),period.c_str(),apass.c_str()));
  
  
}
