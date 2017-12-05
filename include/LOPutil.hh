#ifndef LOPutil_H
#define LOPutil_H
#include "g4csv.hh"
#include <string>
#include <iostream>
#include <fstream>
#include "G4Event.hh"
//#include "G4global.hh"

using namespace std;

class LOPutil
{
	public:
		static LOPutil* GetInstance();
        void ReadLOPFile();
        double GetEee(double E, G4String pname);
	
    private:
		LOPutil();
		~LOPutil();
		static LOPutil* _instance;
    
    public:
        //======= Parameters of Light Output funcion =====
        double PLO[431];    // Light Output Data Array
        double Pparam1;     // 
        double Pparam2;     // 
};


#endif
