#include "LOPutil.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "Randomize.hh"
LOPutil* LOPutil::_instance = NULL;

LOPutil* LOPutil::GetInstance()
{
    if(_instance == NULL)
    {
        _instance = new LOPutil();
    }
    return _instance;
}

LOPutil::LOPutil()
{
    ReadLOPFile();
}

LOPutil::~LOPutil()
{
    delete _instance;
}

void LOPutil::ReadLOPFile()
{
	G4String temp;
	int i;
    
    // PLO function ratio
    //double reduceRatio = 0.95;
    
	ifstream infile("Stilbene_LOP.dat",ios::in);
	if(!infile)
	{
		G4cout<<"Can't Open Light Output Data File!"<<G4endl;	
		return;
	}
	for(i = 0; i < 6; i++)
	{	
        getline(infile, temp);	
    }
	infile >> Pparam1 >> Pparam2;
	for(i = 6;i < 11; i++)
	{
        getline(infile,temp);
    }
	for(i = 0;i < 431; i++)
	{	
        infile >> PLO[i];	
    }
	infile.close();
    /*for (int j = 0; j < 431; j++)
    {
        PLO[j] = PLO[j] * reduceRatio;
    }*/
}

double LOPutil::GetEee(double E,G4String pname)
{
    double Eion = E / 1000.0;     //in unit MeV
    int Eindex;
    double PHLight = 0;
    
    if(pname!= "proton" && pname!= "alpha" && pname!= "deuteron" &&
	   pname!= "Be9[0.0]" && pname!= "C12[0.0]" &&pname!= "B12[0.0]" &&
	   pname!= "e-" && pname!= "e+" && pname == "C12" && pname == "C13")
	{
		G4cout<<"No Light Output Data for Element "<<pname<<" exists!!!"<<G4endl;
		return 0;
	}
// For proton and deuteron
    if(pname== "proton" || pname== "deuteron")
    {
        if(pname == "deuteron")
        {
            Eion = Eion/2;
        }
        if(Eion < 0.6)
        {
            Eindex = floor(Eion / 0.01);
// Linear Interpolation
            PHLight = PLO[Eindex] + (PLO[Eindex+1] - PLO[Eindex]) * (Eion - 0.01 * Eindex) / 0.01;
        }
        if(Eion >= 0.6 && Eion < 8)
        {
            Eindex = floor((Eion - 0.6) / 0.02) + 60;
// Linear Interpolation
            PHLight = PLO[Eindex] + (PLO[Eindex + 1] - PLO[Eindex]) * (Eion - 0.02 * Eindex + 0.6) / 0.02;
        }
        if(Eion >= 8)
        {
            PHLight = Pparam2 * Eion + Pparam1;
        }
        
        if(pname == "deuteron")
        {
            PHLight = PHLight * 2;
        }
        
        return PHLight * 1000;   //return in keV
    }
//For Alpha particle   
    if(pname == "alpha")
    {
        if(Eion <= 6.76)
        {
            PHLight = 0.021 * pow(Eion, 1.871);
            return PHLight * 1000;   //return in keV
        }
        else
        {
            PHLight = 0.20755 * Eion - 0.65314;
            return PHLight * 1000;   //return in keV
        }
    }
//For Be9 ions
    if(pname == "Be9[0.0]")
    {   
        return 0.01 * Eion * 1000;   //return in keV
    }
//For C12 and B12 ions
    if(pname== "C12[0.0]" || pname == "C12" || pname== "B12[0.0]" || pname == "C13")
    {   
        return 0.0097 * Eion * 1000;   //return in keV  
    }
//For electron and positron
    if(pname== "e-" || pname== "e+")
    {
        return Eion * 1000;     // in keV
    }
    G4cout << pname << endl;
    G4cout << "Warning: Do noting in function 'GetEee(double,G4String)'"<< G4endl;
	return 0;
}
