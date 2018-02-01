
/*
  VibHub motor data
*/
#ifndef VhPwm_h
#define VhPwm_h

class VhPwm{

  public:
    VhPwm( int pin, int channel );
    void setPWM( int duty );
    
  protected:
    int _channel;
    int _duty;
    


};

#endif

