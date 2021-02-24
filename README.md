This README requires a full revision!

 Survey G4RadioactiveDecay process. See Physics Reference Manual, chapter 36.
 See also http://ie.lbl.gov/decay.html
 	
\section rdecay01_s1 Geometry construction

 It is a simple box which represente an 'infinite' homogeneous medium.
  
\section rdecay01_s2 Physics list

 PhysicsList.cc defines only G4RadioactiveDecay, G4Transportation processes,
 and relevant particle definitions.
 Therefore, once created, particles or ions travel as geantino.
         	
\section rdecay01_s3 Primary generator
 
 Default kinematic is an ion (Ne24), at rest, at coordinate origin.
 Can be changed with particleGun commands.
         	
\section rdecay01_s4 Physics

 As said above, all particles and ions behave as geantino, eg. no energy loss.
     
 A flag:
\verbatim
/rdecay01/fullChain (false or true)
\endverbatim
allows to limit to single decay (default) or full decay chain.
 
 In case of full decay chain, G4TrackStatus of ions is set to fStopButAlive
 in order to force decay at rest.  
   
 At each decay, one counts and plots energy spectrum of created particles and
 ions, and energy-momentum balance of that decay.
 
 Total time of life of decay chain is plotted. Activity is computed.
 
 Few macros are given in example. Debug.mac is to be run in interactive mode. 
 
\section rdecay01_s5 User data files

 Users can redefine RadioactiveDecay and PhotonEvaporation data, via commands:
\verbatim
/grdm/setRadioactiveDecayFile
/grdm/setPhotoEvaporationFile 
\endverbatim
 Examples of such files are given in subdirectory UserData. \n
 Formats are described in readme \n
 Examples in macros Cf238.mac and No252.mac 
  	
\section rdecay01_s6 Visualisation
 
 Visualization Manager is set in the main () (see rdecay01.cc).
 Initialisation of the drawing is done via the commands
 /vis/.. in the macro vis.mac. This macro is automatically read from the main 
 in case of interactive running mode.
 
 - e- red
 - e+ blue
 - nu_e white
 - anti_nu_e white
 - gamma green
 - alpha yellow
 - GenericIon grey
 
\section rdecay01_s7 How to start ?
 
  - Execute rdecay01 in 'batch' mode from macro files
\verbatim
% rdecay01   singleDecay.mac
\endverbatim
 		
  - Execute rdecay01 in 'interactive mode' with visualization
\verbatim
% rdecay01
....
Idle>   ---> type your commands. For instance:
Idle> /control/execute debug.mac 
....
Idle> /run/beamOn 1
....				
Idle> exit
\endverbatim
	
\section rdecay01_s8 Histograms
 
  rdecay01 produces several 1D histograms which are saved as
  rdecay01.root by default.

    - 1 : energy spectrum: e+ e-
    - 2 : energy spectrum: nu_e anti_nu_ev
    - 3 : energy spectrum: gamma
    - 4 : energy spectrum: alpha
    - 5 : energy spectrum: ions
    - 6 : total kinetic energy (Q)
    - 7 : momentum balance
    - 8 : total time of life of decay chain          
                            
   The histograms are managed by G4AnalysiManager and its Messenger. 
   The histos can be individually activated with the command :
\verbatim
/analysis/h1/set id nbBins  valMin valMax unit 
\endverbatim
   where unit is the desired unit for the histo (MeV or keV, deg or mrad, etc..)
   
   One can control the name of the histograms file with the command:
\verbatim
/analysis/setFileName  name  (default rdecay1)
\endverbatim
   
   It is possible to choose the format of the histogram file : root (default),
   xml, csv, by using namespace in HistoManager.hh
   
   It is also possible to print selected histograms on an ascii file:
\verbatim
/analysis/h1/setAscii id
\endverbatim
   All selected histos will be written on a file name.ascii (default rdecay1)

*/
