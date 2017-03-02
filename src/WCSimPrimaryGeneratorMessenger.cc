#include "WCSimPrimaryGeneratorMessenger.hh"
#include "WCSimPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"

WCSimPrimaryGeneratorMessenger::WCSimPrimaryGeneratorMessenger(WCSimPrimaryGeneratorAction* pointerToAction)
:myAction(pointerToAction)
{
  mydetDirectory = new G4UIdirectory("/mygen/");
  mydetDirectory->SetGuidance("WCSim detector control commands.");

  genCmd = new G4UIcmdWithAString("/mygen/generator",this);
  genCmd->SetGuidance("Select primary generator.");
  //T. Akiri: Addition of laser
  //M. Scott: Addition of Rootracker input format
  genCmd->SetGuidance(" Available generators : muline, normal, laser, rootracker");
  genCmd->SetParameterName("generator",true);
  genCmd->SetDefaultValue("muline");
  //T. Akiri: Addition of laser
  //M. Scott: Addition of Rootracker input format
  genCmd->SetCandidates("muline normal laser rootracker");

  fileNameCmd = new G4UIcmdWithAString("/mygen/vecfile",this);
  fileNameCmd->SetGuidance("Select the file of vectors.");
  fileNameCmd->SetGuidance(" Enter the file name of the vector file");
  fileNameCmd->SetParameterName("fileName",true);
  fileNameCmd->SetDefaultValue("inputvectorfile");

  genSet = false;
}

WCSimPrimaryGeneratorMessenger::~WCSimPrimaryGeneratorMessenger()
{
  delete genCmd;
  delete mydetDirectory;
}

void WCSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if( command==genCmd )
  {
    genSet = true;
    if (newValue == "muline")
    {
      myAction->SetMulineEvtGenerator(true);
      myAction->SetRootrackerEvtGenerator(false);
      myAction->SetNormalEvtGenerator(false);
      myAction->SetLaserEvtGenerator(false);
    }
    else if ( newValue == "normal")
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetRootrackerEvtGenerator(false);
      myAction->SetNormalEvtGenerator(true);
      myAction->SetLaserEvtGenerator(false);
    }
    else if ( newValue == "laser")   //T. Akiri: Addition of laser
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetRootrackerEvtGenerator(false);
      myAction->SetNormalEvtGenerator(false);
      myAction->SetLaserEvtGenerator(true);
    }
    else if ( newValue == "rootracker")   //M. Scott: Addition of Rootracker events
    {
      myAction->SetMulineEvtGenerator(false);
      myAction->SetRootrackerEvtGenerator(true);
      myAction->SetNormalEvtGenerator(false);
      myAction->SetLaserEvtGenerator(false);
    }
  }

  if( command == fileNameCmd )
  {
    if(genSet){
      if(myAction->IsUsingRootrackerEvtGenerator()){
        myAction->OpenRootrackerFile(newValue);
      }
      else{
        myAction->OpenVectorFile(newValue);
      }
      G4cout << "Input vector file set to " << newValue << G4endl;
    }
    else{
      G4cout << "Generator has not been set, guessing input vector file is NOT in the Rootracker format - this will crash if you are using a Rootracker input file" << G4endl;
      G4cout << "Please put the '/mygen/generator' command above the '/mygen/vecfile' command in the mac file." << G4endl;
    }
  }

}

G4String WCSimPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv;
  
  if( command==genCmd )
  {
    if(myAction->IsUsingMulineEvtGenerator())
      { cv = "muline"; }
    else if(myAction->IsUsingNormalEvtGenerator())
      { cv = "normal"; }
    else if(myAction->IsUsingLaserEvtGenerator())
      { cv = "laser"; }   //T. Akiri: Addition of laser
    else if(myAction->IsUsingRootrackerEvtGenerator())
      { cv = "rootracker"; }   //M. Scott: Addition of Rootracker events
  }
  
  return cv;
}

