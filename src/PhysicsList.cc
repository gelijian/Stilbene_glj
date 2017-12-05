#include "globals.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4LossTableManager.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4IonConstructor.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4NeutronTrackingCut.hh"


#include "G4HadronInelasticQBBC.hh"   
#include "G4HadronPhysicsQGSP_BERT_HP.hh" 
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronHElasticPhysics.hh"



#include "PhysicsList.hh"
#include "G4UnitsTable.hh"
#include <iomanip>
using namespace std;

PhysicsList::PhysicsList(): G4VUserPhysicsList()
{
	G4LossTableManager::Instance();
	defaultCutValue = 0.01 * mm;
	SetVerboseLevel(1);

	fParticleList = new G4DecayPhysics("decays");
	emPhysList = new G4EmStandardPhysics();
	ConstructHadrPhys();
		
	G4cout<<"Come Phys Initial"<<G4endl;
}

PhysicsList::~PhysicsList()
{
	delete emPhysList;
	delete fParticleList;
	for(unsigned int i = 0; i < hadronPhys.size(); i++) 
	{
		delete hadronPhys[i];
	}
	G4cout<<"Over Phys!"<<G4endl;
}

void PhysicsList::ConstructParticle()
{
 // In this method, static member functions should be called
 // for all particles which you want to use.
 // This ensures that objects of these particle types will be
 // created in the program. 
	fParticleList->ConstructParticle();
    ConstructBosons();
	ConstructLeptons();

	G4Neutron::Neutron();
	G4Proton::Proton();
	G4Alpha::Alpha();
	G4GenericIon::GenericIon();
	G4cout<<"Come Phys Particle Construction"<<G4endl;
}

void PhysicsList::ConstructBosons()
{
	// gamma
	G4Gamma::GammaDefinition();
}

void PhysicsList::ConstructLeptons()
{
	// leptons
	//  e+/-
	G4Electron::ElectronDefinition();
	G4Positron::PositronDefinition();
}


void PhysicsList::ConstructProcess()
{
	AddTransportation();
	G4cout << "Come Phys3-1" << G4endl;
	emPhysList->ConstructProcess();
	G4cout << "Come Phys3-2" << G4endl;
	for(unsigned int i=0; i<hadronPhys.size(); i++)
	{
		hadronPhys[i]->ConstructProcess();
		G4cout<<"Come Phys3-3-"<<(i+1)<<G4endl;
	}
	G4cout << "Come Phys3-3" << G4endl;	
}

void PhysicsList::ConstructHadrPhys()
{
	// Construct Inelastic, Fission and Capture Process for neutron, proton and PiK
	hadronPhys.push_back( new G4HadronPhysicsQGSP_BERT_HP());
	
	// Construct Low Energy Inelastic Process for D, T and Alpha ion
	hadronPhys.push_back( new G4IonPhysics("ion"));
	
	// Construct HP Hadron Elastic model for neutron, proton and ions
	// HPFlag"true" is just used to set energy divider line for neutron
	hadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel));
	
	// Construct SynchrotronRadiation of e+ and e-, GammaNuclear and MuonNuclear reaction
	//hadronPhys.push_back( new G4EmExtraPhysics("extra EM"));
	
	// StoppingPhysics
	hadronPhys.push_back( new G4StoppingPhysics("stopping",verboseLevel));
	
	// TrackingCut of Neutron,  energy: 10keV
	G4NeutronTrackingCut* NeuCut = new G4NeutronTrackingCut("nTackingCut",verboseLevel);
	NeuCut->SetKineticEnergyLimit(1.0E-3*MeV);   // 1keV
    hadronPhys.push_back(NeuCut);
	
}

void PhysicsList::SetCuts()
{
	if (verboseLevel >0)
	{
		G4cout << "PhysicsList::SetCuts:";
		G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
	}

	// set cut values for gamma at first and for e- second and next for e+,
	// because some processes for e+/e- need cut values for gamma
	//
    SetCutsWithDefault();
     
	if (verboseLevel>0) DumpCutValuesTable();
    G4cout << "come Phys Cuts Setting" << G4endl;
}
