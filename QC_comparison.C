#include <iostream>

#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TLatex.h>
#include <TLine.h>
#include <TLegend.h>
#include <TList.h>
#include <TMath.h>
#include <TMatrix.h>
#include <TVirtualPad.h>
#include <TStopwatch.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>
#include <string>
#include <TROOT.h>

using namespace std;
using namespace o2::quality_control::core;
using namespace o2::quality_control_modules::mid;

void QC_comparison(string period, string run){
  
  gROOT->SetBatch(kTRUE);

  const char pass_name[2][10] = {"apass1","apass4"};
  
  Long_t *dummy1 = 0, *dummy2 = 0, *dummy3 = 0, *dummy4 = 0;
    
  TStopwatch time;
  time.Start();
  
  TFile *file_in[2] = {0x0};
  TH2F *hhLocalBoard[2][4] = {0x0};
  char LocalBoard_name[4][50] = {"hhLocalBoardsMap11","hhLocalBoardsMap12","hhLocalBoardsMap21","hhLocalBoardsMap22"};
  
  TH2F *hhEffRatio[2][4] = {0x0};
  char EffRatio_name[4][50] = {"hhTrackDetRatio44Map11","hhTrackDetRatio44Map12","hhTrackDetRatio44Map21","hhTrackDetRatio44Map22"};
  
  for(int i=0;i<2;i++){
   if(gSystem->GetPathInfo(Form("QC_fullrun_%s_%s_%s_output.root",period.c_str(),pass_name[i],run.c_str()),dummy1,dummy2,dummy3,dummy4) != 0) cout << "File Not Found! Try again" << endl;    
   file_in[i] = new TFile(Form("QC_fullrun_%s_%s_%s_output.root",period.c_str(),pass_name[i],run.c_str()),"read");
   printf("Open File: %s\n",file_in[i]->GetName());
   
   for(int lmap=0;lmap<4;lmap++){
     hhLocalBoard[i][lmap] = (TH2F*) file_in[i]->Get(Form("%s",LocalBoard_name[lmap]));
     hhEffRatio[i][lmap] = (TH2F*) file_in[i]->Get(Form("%s",EffRatio_name[lmap]));
   }
  }
  
  
  //------------- apass1/apass4
  TH2F *hhLocalBoard_ratio[4] = {0x0}; 
  TH2F *hhEffRatio_ratio[4] = {0x0}; 
  
  for(int lmap=0;lmap<4;lmap++){
   hhLocalBoard_ratio[lmap] = (TH2F *) hhLocalBoard[0][lmap]->Clone(Form("%s_ratio_apass1_apass4",LocalBoard_name[lmap]));
   //hhLocalBoard_ratio[lmap]->SetName(Form("%s",LocalBoardOccupancy_title_ratio[lmap]));
   hhLocalBoard_ratio[lmap]->Divide(hhLocalBoard[1][lmap]); 
   
   hhEffRatio_ratio[lmap] = (TH2F *) hhEffRatio[0][lmap]->Clone(Form("%s_ratio_apass1_apass4",EffRatio_name[lmap]));
   hhEffRatio_ratio[lmap]->Divide(hhEffRatio[1][lmap]); 
  }
  
   //------------ Drawing LocalBoardsOccupancy ratio
   gStyle->SetOptStat(0);
   TCanvas *can_lboccupancy_ratio = 0x0;
   TLegend *leg_lboccupancy_ratio;
   TLatex latex_lboccupancy_ratio;
   TH2F *frame_lboccupancy_ratio[4];
   TPad* pad_lboccupancy_ratio[4];
  
   char LocalBoardOccupancy_title_ratio[4][100] = {"Local boards Occupancy MapMT11 apass1/apass4", "Local boards Occupancy MapMT12 apass1/apass4", "Local boards Occupancy MapMT21 apass1/apass4", "Local boards Occupancy MapMT22 apass1/apass4"};
  
   can_lboccupancy_ratio = new TCanvas("can_lboccupancy_ratio","can_lboccupancy_ratio",1600,1600);
   can_lboccupancy_ratio->Divide(2,2); 

   for(int lmap=0;lmap<4;lmap++){
    can_lboccupancy_ratio->cd(lmap+1);
    pad_lboccupancy_ratio[lmap] = new TPad(Form("padlboccupancy_ratio_%i",lmap),"",0,0,1,1);
    pad_lboccupancy_ratio[lmap]->SetMargin(0.1,0.1,0.15,0.1); //sx,dx,basso,alto
    pad_lboccupancy_ratio[lmap]->Draw();
    can_lboccupancy_ratio->cd(lmap+1);
    pad_lboccupancy_ratio[lmap]->cd();
    //pad_lboccupancy[lmap]->SetLogz();
    
    frame_lboccupancy_ratio[lmap] = new TH2F(Form("frame_ratio_%i",lmap),"",1,-7,7,1,0,9); 
    frame_lboccupancy_ratio[lmap]->SetTitle(Form("%s",LocalBoardOccupancy_title_ratio[lmap]));
    frame_lboccupancy_ratio[lmap]->SetTitleSize(0.25);
    frame_lboccupancy_ratio[lmap]->GetXaxis()->SetTitle("Column");
    frame_lboccupancy_ratio[lmap]->GetYaxis()->SetTitle("Line");
    frame_lboccupancy_ratio[lmap]->GetXaxis()->SetTitleSize(0.05);
    frame_lboccupancy_ratio[lmap]->GetYaxis()->SetTitleSize(0.05);
    frame_lboccupancy_ratio[lmap]->GetXaxis()->SetLabelSize(0.05);
    frame_lboccupancy_ratio[lmap]->GetYaxis()->SetLabelSize(0.05);
    frame_lboccupancy_ratio[lmap]->GetXaxis()->SetTitleOffset(1.1); 
    frame_lboccupancy_ratio[lmap]->GetYaxis()->SetTitleOffset(1.1);
    frame_lboccupancy_ratio[lmap]->GetXaxis()->SetRangeUser(-7,7);
    frame_lboccupancy_ratio[lmap]->GetYaxis()->SetRangeUser(0,9);
    frame_lboccupancy_ratio[lmap]->Draw("");
   
    hhLocalBoard_ratio[lmap]->Draw("COLZ");
   }
   
   gSystem->mkdir("ratio-images");
   can_lboccupancy_ratio->SaveAs(Form("ratio-images/can_lboccupancy_ratio_period_%s_run_%s_apass1_apass4.png",period.c_str(),run.c_str()));
   
   
   char EfficiencyRatio_title_ratio[4][100] = {"MT11 Detector Track 44/all Map apass1/apass4", "MT12 Detector Track 44/all Map apass1/apass4", "MT21 Detector Track 44/all Map apass1/apass4", "MT22 Detector Track 44/all Map apass1/apass4"};
   TCanvas *can_effratio_ratio = 0x0;
   TLatex latex_effratio_ratio;
   TH2F *frame_effratio_ratio[4];
   TPad* pad_effratio_ratio[4];
   
   can_effratio_ratio = new TCanvas("can_effratio_ratio","can_effratio_ratio",1600,1600);
   can_effratio_ratio->Divide(2,2); 
   for(int lmap=0;lmap<4;lmap++){
    can_effratio_ratio->cd(lmap+1);
    //can_effratio->cd(lmap+1)->SetMargin(0.10,0.03,0.10,0.05); 
    pad_effratio_ratio[lmap] = new TPad(Form("padeffratio_ratio_%i",lmap),"",0,0,1,1);
    pad_effratio_ratio[lmap]->SetMargin(0.1,0.1,0.15,0.1); //sx,dx,basso,alto
    pad_effratio_ratio[lmap]->Draw();
    can_effratio_ratio->cd(lmap+1);
    pad_effratio_ratio[lmap]->cd();
    //pad_effratio_ratio[lmap]->SetLogz();
    
    frame_effratio_ratio[lmap] = new TH2F(Form("frame_eff_ratio_%i",lmap),"",1,-7,7,1,0,9); 
    frame_effratio_ratio[lmap]->SetTitle(Form("%s",EfficiencyRatio_title_ratio[lmap]));
    frame_effratio_ratio[lmap]->GetXaxis()->SetTitle("Column");
    frame_effratio_ratio[lmap]->GetYaxis()->SetTitle("Line");
    frame_effratio_ratio[lmap]->GetXaxis()->SetTitleSize(0.05);
    frame_effratio_ratio[lmap]->GetYaxis()->SetTitleSize(0.05);
    frame_effratio_ratio[lmap]->GetXaxis()->SetLabelSize(0.05);
    frame_effratio_ratio[lmap]->GetYaxis()->SetLabelSize(0.05);
    frame_effratio_ratio[lmap]->GetXaxis()->SetTitleOffset(1.1); 
    frame_effratio_ratio[lmap]->GetYaxis()->SetTitleOffset(1.1);
    frame_effratio_ratio[lmap]->GetXaxis()->SetRangeUser(-7,7);
    frame_effratio_ratio[lmap]->GetYaxis()->SetRangeUser(0,9);
    frame_effratio_ratio[lmap]->Draw("");
    
    hhEffRatio_ratio[lmap]->Draw("COLZ");
   }
  
   can_effratio_ratio->SaveAs(Form("ratio-images/can_effratio_period_%s_run_%s_apass1_apass4.png",period.c_str(),run.c_str()));
  
  time.Stop();
  time.Print();
    
}
