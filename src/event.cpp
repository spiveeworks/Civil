
typedef FxPtNum<-4, signed long> time;

class Event {
protected:
    time when;
public:
    virtual void execute() =0;
    time getTime () {return time;}
};

class 



