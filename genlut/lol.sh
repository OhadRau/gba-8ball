#!/bin/bash

# Yeah, so apparently nin10kit does exactly this lol

#nin10kit --mode=lut --func=sqrt,int.4,0,1000,1 sqrt
nin10kit --mode=lut --func=sin,int.8,0,360,1,1 sin
sed -i '1i#pragma GCC diagnostic ignored "-Wpedantic"' sin.c
#nin10kit --mode=lut --func=cos,short.8,0,360,1,1 cos
