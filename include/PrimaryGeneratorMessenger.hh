
#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class PrimaryGeneratorMessenger: public G4UImessenger
{
public:
	PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
	~PrimaryGeneratorMessenger();

	virtual void SetNewValue(G4UIcommand*, G4String);

private:
	PrimaryGeneratorAction* Action;
	
	G4UIdirectory* GunDir;
	G4UIcmdWithAString* GPSFlagCmd;
	G4UIcmdWithAString* ParticleNameCmd;
	G4UIcmdWithAString* SourceTypeCmd;
	G4UIcmdWithADoubleAndUnit* SourceEnergyCmd;
	G4UIcmdWithADoubleAndUnit* SEconst;
	G4UIcmdWithADoubleAndUnit* SourceRadiusCmd;
	G4UIcmdWithADoubleAndUnit* SourcePosition_zCmd;
	G4UIcmdWithADoubleAndUnit* MomentumThetaRangeCmd;
};

#endif
