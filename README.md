# ThereMIDIn

> A MIDI enabled Theremin which also is a digital synthesizer

## Introduction
The idea behind this ThereMIDIn project is to control synthesizers and espacially soft-synths just by 
moving your hands. The original Theremin is the oldest known electronic instrument its principle is simple.
Two RF-antennas create two electric fields, which can be manipulated with your own electric field 
(or the field of whatever). In the original design, one antenna controls the pitch of the outcoming sound 
and the other one controls the volume. 
 
But, how cool would it be to control ANY parameter of ANY synthesizer just by moving your hands?
It would be awesome, that's right :)

## Library Updates
### Audio Library
Because the class 'BandPassFilter' inherits from the class 'AudioFilterStateVariable' (which is part of the *Audio* library), 
some private methods in *filter_variable.h* need to be declared as *protected*:

Change from (line 68):
'private:
	void update_fixed(const int16_t *in,
		int16_t *lp, int16_t *bp, int16_t *hp);
	void update_variable(const int16_t *in, const int16_t *ctl,
		int16_t *lp, int16_t *bp, int16_t *hp);'
        
To:
'protected:
	void update_fixed(const int16_t *in,
		int16_t *lp, int16_t *bp, int16_t *hp);
	void update_variable(const int16_t *in, const int16_t *ctl,
		int16_t *lp, int16_t *bp, int16_t *hp);'