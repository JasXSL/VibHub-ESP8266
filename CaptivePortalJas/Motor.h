
/*
  VibHub motor data
*/
#ifndef Motor_h
#define Motor_h

class Motor{

  public:
    Motor( int pin, int channel );
    void setPWM( int duty );
    
  private:
    int _channel;
    int _duty;
    


};

#endif
