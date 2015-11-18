#version 150

#define PI 3.1415926535897932384626433832795

out vec4 FragColor;

in vec2 UV;
in vec3 Norm;
in vec3 Pos;
in float Radius;
in vec4 Col;

uniform	sampler2D texture0;

float atan2(in float y, in float x)
{
    return abs(x) < 0.02 ?  PI/2 : atan(y/x);
} 
float atan3(in float y, in float x)
{
    return (abs(x) > abs(y)) ?  PI/2.0 - atan(x,y) : atan(y,x);
}

float pow2(in float a )
{
	return a *a;
}
void main()
{
	vec3 lpn = normalize( Pos );
	float ang =  atan3(lpn.z,lpn.x)/PI, longatude =  asin(  lpn.y )/(PI/2)	;
	//FragColor = texture(texture0, vec2( ang, 1-lpn.y ));

	vec4 col;
	
	vec4 noise = texture(texture0, vec2( ang, longatude));
	vec4 noise2 = texture(texture0, vec2( ang*2 + (sin(ang*3)/PI + sin(ang*7)/PI)*0.5 , longatude*(0.5+noise.r+noise.b*2 + sin(ang*23)/PI) )*(12) );
	vec4 noise3 = texture(texture0, vec2( ang, longatude) *42 );
	
	vec3 nrm = Norm;
	nrm = normalize(nrm);
	float rad =Radius;
	rad +=  noise3.g*noise3.r*0.01;
	if( rad > 15.0 ) {
		float y = longatude;
		if( y < 0 ) {
			y = -y;
			ang += 7;
		}
		if( y >0.7 + (noise.b + noise2.b*0.2 + noise3.b*0.1  )*0.2  ) {
			col = vec4(0.9,0.9,1.0,1.0);
		} else if( y > 0.2 +0.1*sin(ang*3*PI) +(noise.g*2- noise.g*noise2.g + noise3.g*0.1 )*0.1 )  {
			col = vec4(0.3,0.6,0.19,1.0);
		} else if( y > 0.05 +0.025*sin(ang*3*PI) +(noise.r*2.5- noise.r*noise2.r  + noise3.r*0.1 )*0.075  )  {
			col = vec4(0.99,0.8,0.55,1.0);
		} else {
			col = vec4(0.15,0.55,0.08,1.0);
		}		
	} else {
		col = vec4(0.0,0.0,1.0,1.0);	 
		nrm = lpn;
	}
	
	col *= 0.7+ 0.3*( noise2.b*2 +noise.b*4+noise3.b)/7;
	col *= (0.3+ 0.7*max( dot( nrm, normalize(vec3(0.8,0.5,0.2)) ),0.0) ); 
	//col = vec4(pow2(longatude),pow2(longatude),abs(longatude),1.0);	 
	// vec3 nrm = normalize(Norm);
	// col =  vec4(nrm*0.5 +vec3(0.5,0.5,0.5), 1.0);
	// col = noise2*noise.b;
	FragColor = col;
}
