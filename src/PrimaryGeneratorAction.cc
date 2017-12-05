//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
	G4int nofParticles = 1;
	fParticleGun = new G4ParticleGun(nofParticles);
    gunMessenger = new PrimaryGeneratorMessenger(this);
    
    GPSFlag = "self";
	SourceType = "mono";
	ParticleName = "neutron";
	SourceRadius = 0 * cm;
	SourcePosition_z = 0 * cm;
	SourceEnergy = 0 * MeV;
	EnergyConst = 0 * MeV;
	MomentumThetaRange = 0 * deg;
	G4cout << "Come Primary Initial" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
    delete gunMessenger;
    G4cout << "Come Over PrimaryGenerator" << G4endl;
}

void PrimaryGeneratorAction::SelfDefineGun()
{
    //-------- Position of source
	G4double SourcePosition_x = 0.0 * cm;
	G4double SourcePosition_y = 0.0 * cm;
    if (SourceRadius == 0)
    {
        SourcePosition_x = 0.0 * cm;
        SourcePosition_y = 0.0 * cm;
    }
    else
    {
        G4double theta = G4UniformRand() * twopi;
        G4double r = SourceRadius * sqrt(G4UniformRand());
        SourcePosition_x = r * cos(theta);
        SourcePosition_y = r * sin(theta);
    }
	//-------- Energy of source
	G4double energy, Emean, EFHWM, sigma, prob1, prob2, angtheta, angphi;
	if (SourceType == "mono")
	{	energy = SourceEnergy;	}
	
	if (SourceType == "gauss")
	{	Emean = SourceEnergy / MeV;  // MeV
		EFHWM = EnergyConst / MeV;  // MeV
		sigma = EFHWM / 2.355;
		energy = (G4RandGauss::shoot(Emean, sigma)) * MeV;
		if(energy < 0.001) {  energy = 0.001 * MeV;  }
	}
	
	if (SourceType == "rect")
	{
		energy = SourceEnergy + EnergyConst * (G4UniformRand() - 0.5);
	}
	
	if (SourceType == "flat")
	{
		energy = EnergyConst * G4UniformRand();
	}
	
	if (SourceType == "Cf252")
	{
		energy = 0.5 + G4UniformRand() * 9.5;
		prob1 = G4UniformRand() * 0.54;
		prob2 = sqrt(energy) * exp(-energy / 1.565);
		while(prob1 > prob2)
		{
			energy = 0.5 + G4UniformRand() * 9.5;
			prob1 = G4UniformRand() * 0.54;
			prob2 = sqrt(energy) * exp(-energy / 1.565);
		}
		angtheta = G4UniformRand() * twopi;
		angphi = G4UniformRand() * twopi * 0.5;	
		//energy = 0.0-EnergyConst*log(G4UniformRand());
	}
	
	if (SourceType == "expdecay")
	{
		energy = 0.0-EnergyConst*log(G4UniformRand());
	}
	//-------- Momentum direction of source
	G4double vx = 0.0;
	G4double vy = 0.0;
	G4double vz = 0.0;
	G4double vr = 1.0;
	G4double MomentumTheta = G4UniformRand() * MomentumThetaRange ;
	G4double phi = 360 * G4UniformRand() * deg;
	vx = vr * sin(MomentumTheta) * cos(phi);
	vy = vr * sin(MomentumTheta) * sin(phi);
	vz = vr * cos(MomentumTheta);

	//--------define the particle gun
	G4ParticleTable* fParticleTable = G4ParticleTable::GetParticleTable();
	fParticleGun->SetParticleDefinition(fParticleTable->FindParticle(ParticleName));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(vx, vy, vz));
	fParticleGun->SetParticlePosition(G4ThreeVector(SourcePosition_x, SourcePosition_y,SourcePosition_z));
	fParticleGun->SetParticleEnergy(energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::ReadEnergyDist()
{} 

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	if(GPSFlag == "self")
	{
		SelfDefineGun();
	}
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
