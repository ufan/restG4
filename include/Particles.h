
#ifndef Particles_h
#define Particles_h 1

#include <G4VPhysicsConstructor.hh>
#include <globals.hh>

//[todo] this class is not used in the framework

class Particles : public G4VPhysicsConstructor {
   public:
    Particles(const G4String& name = "particles");

    virtual ~Particles();

    virtual void ConstructParticle();

    // This method is dummy
    virtual void ConstructProcess(){};
};
#endif
