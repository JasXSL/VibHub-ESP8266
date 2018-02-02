
/*
  VibHub motor data
*/
#ifndef VhPwm_h
#define VhPwm_h

class VhPwm{

  public:
    VhPwm( int pin, int channel, bool _invert=false );
    void setPWM( int duty );
    
  protected:
    int _channel;
    int _duty;
    bool _invert;


};

#endif

