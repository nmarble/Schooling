#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;
//Structs for exception handling
struct incomplete_object{
    string value;
    incomplete_object(string str){ value = str;};
};
struct unrec_type{
    string value;
    unrec_type(string str){ value = str;};
};
struct inval_intVal{
    int value;
    inval_intVal(int val_in){ value = val_in;};
};
struct missing_typeVal {
    string value;
    missing_typeVal(string str = ""){ value = str;};
};
struct inval_age{
    int val;
    string obj_type;
    inval_age(string str, int val_in){obj_type = str; val = val_in;};
};

//enum definitions
typedef enum {TIGER_OBJ, CAR_OBJ, HUMAN_OBJ} TypeEnum;

//MyObject abstract class
class MyObject{
public:
    //virtual functions that allow later subtype manipulation
    virtual TypeEnum get_type_enum() const = 0;
    virtual float get_speed() const = 0;
    virtual string get_type_name() const = 0;
    
    virtual string get_name() const = 0;
    virtual int get_age() const = 0;
    virtual float get_diameter() const = 0;
    virtual int year_passed(int years) = 0;
    virtual ~MyObject(){};      //class destructor
    virtual float get_x() const {return 0;}
    virtual float get_y() const {return 0;}
    virtual void set_x(float _x) const {}
    virtual void set_y(float _y) const {}
    virtual float get_dx() const {return 0;}
    virtual float get_dy() const {return 0;}
};//End MyObject class

// MyObject_Imp class implements all the features to create objects with their name,
// diameter and age
class MyObject_Imp : MyObject{
    string obj_name;
    int age;
    float diam;
    
public:
    //Constructor
    MyObject_Imp(const string& _obj_name, float _diameter, int _age){
        obj_name = _obj_name;
        diam = _diameter;
        age = _age ;
    };
    
    //virtual functions that allow later subtype manipulation
    virtual TypeEnum get_type_enum() const = 0;
    virtual float get_speed() const {return -1;};
    virtual string get_type_name() const{return "unknonwn";};
    
    string get_name() const {return obj_name;};
    int get_age() const { return age;};
    float get_diameter() const {return diam;};
    int year_passed(int years){
        
        //checks for a positve value
        if (years > 0) {
            return  age += years;
        }
        else
            throw inval_intVal(years);
    };
    virtual ~MyObject_Imp(){};      //class destructor
};
//Derived classes
class Human : public MyObject_Imp{
    
    string obj_name;
    
public:
    
    //Constructor figures out the proper travel speed based on the passed age
    Human(const string& _obj_name, float _diameter, int _age) : obj_name(_obj_name),
    MyObject_Imp(_obj_name, _diameter, _age){
        if (_age < 0 || _age >= 100) {
            throw inval_age("human", _age);
        }      
    }
    float get_speed() const{
        
        int age = get_age();
        
        if (age < 30) {
            return (age * .5);
        }
        else if ( age >=30 && age < 65){
            return 15;
        }
        else if ( age >= 65 && age < 95){
            return (15 - ((age - 65) * .5)); //subtracts .5 speed per age > 65
        }
        return 0;
    }
    TypeEnum get_type_enum() const { return HUMAN_OBJ; }
    string get_type_name() const{return "Human";};
    virtual ~Human() {}
     
};
class Tiger : public MyObject_Imp{
string obj_name;
public:
    
    //Constructor figures out the proper travel speed based on the passed age
    Tiger(const string& _obj_name, float _diameter, int _age) : obj_name(_obj_name),
    MyObject_Imp(_obj_name, _diameter, _age){   
        if (_age < 0 || _age > 26) {
            throw inval_age("tiger", _age);
        }     
    }  
    float get_speed() const{   
        return  (5 + get_age());    
    }  
    TypeEnum get_type_enum() const { return TIGER_OBJ; }
    string get_type_name() const { return "Tiger"; };
    virtual ~Tiger() {}  
};
class Car : public MyObject_Imp{
    
    string obj_name;
    
public:
    
    //Constructor figures out the proper travel speed based on the passed age
    Car(const string& _obj_name, float _diameter, int _age) : obj_name(_obj_name),
    MyObject_Imp(_obj_name, _diameter, _age){    
        if (_age < 0 || _age > 50) throw inval_age("car", get_age());
    }

    float get_speed() const{ return 50; }
    TypeEnum get_type_enum() const { return CAR_OBJ; }
    string get_type_name() const { return "Car"; }
    virtual ~Car() {}
    
};
//Factory Class that creates and maintains all objects created
class ObjectFactory{
protected:
    
    ObjectFactory() {}
    static ObjectFactory* _instance;
    virtual ~ObjectFactory() {}
    
public:
    vector<MyObject*> objVect;
    static ObjectFactory* get_inst();
    
    /*Function reads in object variables from a file and ensures proper object creation*/
    int readObjects(){
        
        string data;
        int intCount = 0;
        int flCount = 0;
        int strCount = 0;
        
        int intValue = 0;
        string strValue = "";
        float flValue = 0.0; 
        
        cin >> data;
        while( cin ) {         
            try {            
                if (data == "int") {
                    int iTest;
                    cin >> iTest;   //used since cin changes int value to zero upon failure
               
                    if (cin.fail()){
                        throw missing_typeVal();
                    }               
                    if (iTest < 0) {
                        throw inval_intVal(iTest);
                    }                  
                    intValue = iTest;
                    intCount++;
                }
                else if (data == "string"){
                    cin >> strValue;
                    if (intValue == 0 || flValue == 0) {
                        throw incomplete_object(strValue);
                    }         
                    objVect.push_back(new_object(strValue, flValue, intValue));                 
                    strCount++;
                }
                else if(data == "float"){
                    float flTest;
                    cin >> flTest;
                    
                    if (cin.fail()) {
                        throw missing_typeVal();
                    }
                    flValue = flTest;
                    
                    flCount++;
                }
                else{
                    
                    throw unrec_type(data);
                }              
                cin>>data;
              
                if (data[0] == '[') {
                    for (int i = (data.size() -1) ; i>=0; i--) {
                        cin.putback(data[i]);
                    }
                    return 0;
                }               
                           
            } catch (incomplete_object e) {             
                cerr << "Error: incomplete object encountered: " << e.value << "\n";
                strValue = "";
                cin >> data;              
            } catch (unrec_type e) {             
                cerr << "Error: unrecognized type: " << e.value << "\n";
                cin >> data;             
            }catch (inval_intVal e){             
                cerr << "Error: no integer should be negative: " << e.value << "\n";
                cin >> data;               
            }catch (missing_typeVal e) {               
                cin.clear();
                cin >> data;
                cerr << "Error: missing value for int type: " << data << "\n";              
            }catch (inval_age e){
                cin >> data;
                cerr << "Error: invalid age for " << e.obj_type << ": " << e.val << endl;
            }
        }//END while loop
             
        return 0;
    }//END readObjects function
   
    /*Function is passed the object information and decides which subtype object to create.*/
    virtual MyObject* new_object(string name, float diameter, int age){
        
        char objType = tolower(name[0]);
       
        if (objType == 't') { return (MyObject*) new Tiger(name, diameter, age); }      
        else if (objType == 'c') { return (MyObject*) new Car(name, diameter, age); }      
        else return (MyObject*) new Human(name, diameter, age);
            
    }//END new_object function
      
    //Function simulates the amount of years passed as instructed by the given input.
    //If the object exceeds it's life cycle the object is deleted.
    virtual void time_passed(int year){
        
        for (int i = 0; i < objVect.size(); ++i) {
            objVect[i]->year_passed(year);                 
        }        
        //loop that eliminates any object that is past its life cycle
        for (int j = 0; j < objVect.size(); ++j) {
            switch (objVect[j]->get_type_enum()) {
                case 0: //TIGER
                    if (objVect[j]->get_age() > 26) {
                        delete objVect[j];
                        objVect.erase(objVect.begin() + j);
                    }
                    break;                  
                case 1: //CAR
                    if (objVect[j]->get_age() > 50) {
                        delete objVect[j];
                        objVect.erase(objVect.begin() + j);
                    }
                    break;                   
                case 2: //HUMAN
                    if (objVect[j]->get_age() > 100) {
                        delete objVect[j];
                        objVect.erase(objVect.begin() + j);
                    }
                    break;                              
            }//END Switch
        }//END for loop             
    }//END time_passed function
    
    typedef vector<MyObject*>::const_iterator const_iterator;
    const_iterator begin() const { return objVect.begin(); }
    const_iterator end() const { return objVect.end(); }
          
};//END Factory class

//Moving target class that replaces the standard obj
class MovingTarget : public MyObject{
    
    MyObject* mvobj;
    float x, y, dx, dy;
    vector <class Observer * > views;    
public:
 
    MovingTarget();
    MovingTarget(MyObject* _mvobj, float _x, float _y, float _dx, float _dy){
        mvobj = _mvobj;
        x = _x;
        y = _y;
        dx = _dx;
        dy = _dy;
        
    };
    void attach(Observer *obs) {
        views.push_back(obs);
    }
    
    virtual float get_x() const {return x;}
    virtual float get_y() const {return y;}
    virtual void set_x(float _x) const {x += _x;} 
    virtual void set_y(float _y) const {y += _y;} 
    virtual float get_dx() const {return dx;}
    virtual float get_dy() const {return dy;}
    virtual int get_age() const {return mvobj->get_age();}
    virtual float get_diameter() const {return mvobj->get_diameter();}
    
    TypeEnum get_type_enum() const {return mvobj->get_type_enum();}
    float get_speed() const {return mvobj->get_speed();}
    string get_type_name() const {return mvobj->get_type_name();}
    
    string get_name() const {return mvobj->get_name();}
    int year_passed(int years) {return mvobj->year_passed(years);}   
    void notify();   
    virtual ~MovingTarget(){}
    
};//END MovingTarget class
class Evolution : public ObjectFactory{
protected:
    static Evolution* _instance;
    Evolution() {}
    ~Evolution() {}
    
public:
    static Evolution* get_evo_inst();
    virtual void time_passed(int year){
        for (int index = 0; index < objVect.size(); index++) {
            objVect[index]->set_x(objVect[index]->get_speed() * objVect[index]->get_dx());
            objVect[index]->set_y(objVect[index]->get_speed() * objVect[index]->get_dy());   
        }
        for (int outer = 0; outer < objVect.size(); outer++) {
            for (int inner = 0; inner < objVect.size(); inner++) {
                if (outer != inner) {
                    
                    //Check outer with inner
                }
            }
        }
        
    }

    //Function finishes reading the input that contains the object coordinates.
    int readCoordinates(){
        string coord_data;
        float x = 0, y = 0, dx = 0, dy = 0;
        float coordVal;
        
        for (int i = 0; i < objVect.size(); i++){
            
            cin >> coord_data;
            
            //parses the coordinate data to be placed in proper variable
            if (!cin.fail()) {
                x = 0, y = 0, dx = 0, dy = 0;
                stringstream coordParse(coord_data);
                if (coordParse.peek() == '[') {
                    coordParse.ignore();
                }
                coordParse >> coordVal;
                while (coordParse) {
                    if (x == 0)x = coordVal;                 
                    else if (y == 0)y = coordVal;               
                    else if(dx == 0)dx = coordVal;                 
                    else if(dy == 0)dy = coordVal;                 
                    if (coordParse.peek() == ',' || coordParse.peek() == ']') {
                        coordParse.ignore();
                    }                
                    coordParse >> coordVal;
                }
            }            
            MovingTarget* mvTarg = new MovingTarget(objVect[i], x, y, dx, dy);         
            objVect[i] = mvTarg;          
        }//END iterator
        return 0;
    }   
};//END Evolution class
class Observer {
    MovingTarget *model;
    string colObj1;
    string colObj2;
public:
    Observer(MovingTarget *mod, string obj1, string obj2) {
        model = mod;
        colObj1 = obj1;
        colObj2 = obj2;
        model->attach(this);
    }
    virtual void update() = 0;
protected:
    MovingTarget *getMovingTarget() {
        return model;
    }
    string getObj1() {
        return colObj1;
    }
    string getObj2() {
        return colObj2;
    }
};

void MovingTarget::notify() {
    for (int index = 0; index < views.size(); index++) {
        views[index]->update();
    }
}

class ColObserver: public Observer {
public:
    ColObserver(Evolution *mod, string obj1, string obj2): Observer(mod, obj1, obj2){}
    void update() {
        string colObj1 = getEvolution();
    }
};

//operator overload for "<<"
ostream& operator <<(ostream& out, const MovingTarget *obj){
    out << "(" << obj->get_type_name() << "," << obj->get_name() << "," <<
    obj->get_diameter() << "," << obj->get_age() << "," << obj->get_speed() << "):"
    << " [" << obj->get_x() << "," << obj->get_y() << "," << obj->get_dx() << "," <<
    obj->get_dy() << "]";
    
    return out;
}

//Adds the singleton functionality to the factory
ObjectFactory* ObjectFactory::_instance = 0;
ObjectFactory* ObjectFactory::get_inst() {
    
    if (_instance == 0 ) {
        _instance = new ObjectFactory;
    }
    return _instance;
}

//Adds the singleton functionality Evolution class and also creates an instance
//of the object factory
Evolution* Evolution::_instance = 0;
Evolution* Evolution::get_evo_inst(){
    
    if (_instance == 0 ) {
        _instance = new Evolution;
        ObjectFactory::get_inst();
    }
    return _instance;
}

//Function uses the iterator to print the objects
void printObjects(Evolution *_objEvo){
    for (Evolution::const_iterator iter = _objEvo->begin(); iter != _objEvo->end(); ++iter){
        //explicit casting of to moving target for printing
        MovingTarget* mvTarg;
        MyObject* ob = *iter;
        mvTarg = dynamic_cast<MovingTarget*>(ob);
        cout << mvTarg << " ";
    }
    cout << endl;
}

int main() {
    Evolution* evo = Evolution::get_evo_inst();
    
    int yrSimulate = 0;
    
    //reads the integer that determines how many years to simulate. Assumed it's always the
    //first thing read in given string.
    cin >> yrSimulate;
    evo->readObjects();
    evo->readCoordinates();
    
    cout << "beginning:\n";
    printObjects(evo);
    evo->time_passed(yrSimulate);
    cout << "after " << yrSimulate << " years:\n";
    printObjects(evo);

    return 0;
}
