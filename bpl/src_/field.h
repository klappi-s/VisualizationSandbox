#pragma once


class Field_b{
    public:
    std::string field_ID;
    // virtual void get_data(){
    //     std::cout << " aah shit here we go again" << std::endl;

    // }

};


template<typename T, unsigned Dim>
class Field : public Field_b{
  public:
    inline static int counter = 0;
    T data;
    

    Field();
    Field(std::string name, T v) ;

};



  

    template<typename T, unsigned Dim>
    Field<T,Dim>::Field(std::string name, T v)  : data(v){
        ++counter;
        field_ID = name;
            std::cout << "creating field container" << std::endl;
            VisBase::sf_c.push_back(this);      
    }

    // Field<T,Dim>::Field() :  Field("unnamed_"+ std::to_string(counter), 0.0)  { 
    template<typename T, unsigned Dim>
    Field<T,Dim>::Field()   { 
        ++counter;
        field_ID = "Field<"+std::string(typeid(T).name())+","+std::to_string(Dim)+">_"
                + std::to_string(counter)+"_unlabeled";
    }