#include <fstream>
#include <iostream>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include "TMath.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "../interface/format.h"
#include "../interface/TH1Info.h"	// Histogram details are defined here
#include "../interface/TH1InfoClass.h"	 
#include "../interface/TH2Info.h"	// Histogram details are defined here
#include "../interface/TH2InfoClass.h"	 
using namespace std;

typedef TH1D TH1_Type;
typedef TH2D TH2_Type;

const string samplePath    = "/afs/cern.ch/work/r/rslu/public/FNAL2013_data";
const string storeRootPath = "../result/root";
const string storePngPath  = "../result/png";

const int Sample_Size = 2;
const string sampleName[Sample_Size] = {
	"tree_34318_",
	"tree_35113_"
};

const int ROC_Size = 8;
const char* index_ROC[ROC_Size] = {
	"ROC_0",
	"ROC_1",
	"ROC_2",
	"ROC_3",
	"ROC_4",
	"ROC_5",
	"ROC_6",
	"ROC_7",
};

const int Row_Size = 80;
const int Col_Size = 52;

void pixelCheck(){

	for( int isample=0; isample<Sample_Size; isample++){
		////= Input and prepare the out files ===========================================================================
		string input 	=    samplePath + "/" + sampleName[isample] + ".root";
		string output	= storeRootPath + "/" + sampleName[isample] + ".root";
		TFile* input_f	= new TFile(input.c_str()); 
		TFile* output_f	= new TFile(output.c_str(),"RECREATE"); 
		cout<<"Success read "<<input<<" !"<<endl;	
	
		////= Regist tree and branch ====================================================================================
		TTree* tree = (TTree*)input_f->Get("tree");
		Branches Hit;
		Hit.Register(tree);	
		cout<<"Success register tree!"<<endl;	

		////= Create Histogram, create sub-directory and initialize variables ===========================================
		int hits[ROC_Size][Row_Size][Col_Size];

		TH1InfoClass<TH1_Type> h1[ROC_Size];
		TH2InfoClass<TH2_Type> h2[ROC_Size];

		for( int index=0; index<ROC_Size; index++){ 
			output_f->mkdir(index_ROC[index]);
			output_f->cd(index_ROC[index]);
				h1[index].Initialize();
				h2[index].Initialize();
				cout<<"Success create TH1 and TH2 in "<<index_ROC[index]<<" !"<<endl;	
			output_f->cd();

			for( int r=0; r<Row_Size; r++){
				for( int c=0; c<Col_Size; c++){
					hits[index][r][c]=0;	
				} // colume
			} // row
			 
		} // roc
	
		TH1D* h_hits = new TH1D("TotalHits", "Total Hits", 9, -1, 8);
		h_hits->GetXaxis()->SetBinLabel(1,"X_X");
		h_hits->GetXaxis()->SetBinLabel(2,index_ROC[0]);
		h_hits->GetXaxis()->SetBinLabel(3,index_ROC[1]);
		h_hits->GetXaxis()->SetBinLabel(4,index_ROC[2]);
		h_hits->GetXaxis()->SetBinLabel(5,index_ROC[3]);
		h_hits->GetXaxis()->SetBinLabel(6,index_ROC[4]);
		h_hits->GetXaxis()->SetBinLabel(7,index_ROC[5]);
		h_hits->GetXaxis()->SetBinLabel(8,index_ROC[6]);
		h_hits->GetXaxis()->SetBinLabel(9,index_ROC[7]);	

		////= Loop for each hit =========================================================================================
		cout<<"Running..."<<endl;	
		for( int hit=0; hit<tree->GetEntries(); hit++){
			tree->GetEntry(hit);			
			h_hits->Fill(Hit.ROCnumber);
			if( Hit.ROCnumber < 0 ) continue; 	// Some Hit.ROCnumber = -1	
			output_f->cd(index_ROC[Hit.ROCnumber]);	// Enter the ROC_number directory
				
				h2[Hit.ROCnumber].GetTH2("HitsMap")->Fill(Hit.col,Hit.row);
				h1[Hit.ROCnumber].GetTH1("ROCnumber")->Fill(Hit.ROCnumber);
				hits[Hit.ROCnumber][Hit.row][Hit.col]++;

			output_f->cd(); // Exit
		} // Hit
		output_f->Write();
		cout<<"Success write into "<<output<<" !"<<endl<<endl;	
	} // sample
}


















