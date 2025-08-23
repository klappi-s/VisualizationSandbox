#pragma once

class VisBase{
    public:
    
    std::string vis_ID;
    virtual int get_size_pb(){return pb_c.size(); }
    virtual int get_size_sf(){return sf_c.size(); }
    
    inline static std::vector<Field_b*>sf_c; 
    inline static std::vector<ParticleBase_b*> pb_c;
    
};










