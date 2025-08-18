# VisualizationSandbox

I shortly impemented a basic variant of th ippl layout, "bpl" to be able to try out differents things on how to ideally integrate the visualisation; and get back into the cpp language.




## The Easy way:
Using a base manager class for everything would simlify tha task alot. Ideally the vismanager object is default created by the manager class. And visualisaton initilaize and execut and finalize functions are put into an overwritten run() function of the manager.
Via static member functionality; whenever a field or particle container is created they grant an access point to the VisManager/ automatically subscripe to a registry.

to use an official ideal fromat of the registry design pattern seems likely to be overkill.





private memers cant be directly accessed by childre :D
maybe test out to be sure...

## Field and Particle Registration


The constructor for fields constructor are unlikely to be overwritten by a user (not intended/possible?...). So we can pestablish connection between field and vismanager right there. Further  field has not to many functions we can use for integration.

The constructor fo particle base is very likely to be overwritten/created by the user. But there is the initilalize function. SInce the particleLayout object in tha paricle Base class is private, and child class wont be able to access playout_m directly. Users needs to access this object with the initialize function. Ideally we setup visualisation registration there.

Another function he is guaranteed to use and unlikely to overwirte is the addAttribute function.


So we could technically setup vismanager connection in there somehowhe field is



## Manager Problem:
We have the folloing problems:

IPPL doesnt force users to use the BaseManager.
Eitherwe go the route and say visualsation is only directly enabled with the use of the manager or we build the necessary visualisation call into other parts of ippl.

So The basemanager class is not guaranteed to be used.
Only the particleBase and fields are.


## Functions where visualisation calls could be buried:


Initilize/FInalizer
- force the user to write initialize himself
- "   #include "ippl.h"     "
- ippl:init
- ippl:exit

execute calls
- particlease update()
- Maybe: scatter, gather
- functions of some form of base solver function...
- ...

## type erasure for std::unique<std::vector<type*>>

We nfo the registry we need some form of type erasure,to be able to store different particleBunches and different fields in a neat registry.

Type erasure concepts:
- baseclass
- std::any
- std::variant
- void*


For ParticleBase we can create a higher tempalteless base class particleBaseEmpty, a pointer to this class can point to any template realisation of its children.


For the field its a bit harder since its inherits from barefiel from ippl::expression. Both templated...


We can include all field variants with std::variant, but I am not sure if users are likely to accidentally create fields not included in variant...

ooor we use std::any and additionally save a vector of types to then recast eveything to its original type..


## labels 
also we need Label/ID for fields and particleContainers that can be shown as refernce in the visualisation
