#version 150

#if GL_ES

precision mediump float;

#endif

out vec4 fragColor;
uniform vec2      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)


float voronoi(in vec2 uv) {
    vec2 lp = abs(uv)*10.;
    vec2 sp = fract(lp)-.5;
    lp = floor(lp);
    
    float d = 1.;
    
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            
            vec2 mp = vec2(float(x),float(y));
            vec2 p = lp+mp;
            
            d = min(d,length(sp+(cos(p.x+iTime)+cos(p.y+iTime))*.3-mp));
            
        }
    }
    
    return d;
}

void main()
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy - vec2(.5);
    uv.y *= iResolution.y/iResolution.x;
    
    
    float ang = atan(uv.y,uv.x);
    float dst = length(uv);
    float cfade = clamp(dst*40.-3.+cos(ang*1.+cos(ang*6.)*1.+iTime*2.)*.68,0.,1.);
    
    float a = 0.;
    for (int i = 3; i < 6; i++) { 
        float fi = float(i);
        vec2 luv = uv+cos((ang-dst)*fi+iTime+uv+fi)*.2;
    
    	a += voronoi(luv)*(.7+(cos(luv.x*14.234)+cos(luv.y*16.234))*.4);
    }
    vec3 color = vec3(0.16, .57 ,.19);
    
	fragColor = vec4(color*a*cfade,1.);
}