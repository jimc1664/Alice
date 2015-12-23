#version 150

#define PI 3.1415926535897932384626433832795

#include "simplex.glsl"
#include "commonDefferedFS.glsl"


in vec2 UV;
in vec3 Pos;
in float Radius;
in vec4 Col;

uniform	sampler2D texture0;

uniform vec3 CamPos;
uniform float LodMod;


void main() {
	vec3 lpn = normalize( Pos );
	vec4 col;
	
	vec3 sp = lpn *25.0;
	float simplex = snoise(sp, 8, 3.0f, 0.7 );
	
	col = texture(texture0, UV );
	
	col *= 0.7+ 0.3*simplex;
	//col *= (0.3+ 0.7*max( dot( nrm, normalize(vec3(0.8,0.5,0.2)) ),0.0) ); 
	
	vec2 uv = (VPos.xy + 1)*vec2(0.5,0.5);
	vec4 lightDat = texture(LightBuff, uv );
	col.rgb *= lightDat.rgb; 
	//	col.xy = uv; col.z = 0;
	
	FragColor = col;
}

/*
void main() {
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
		//nrm = lpn;
	}
	
	col = Col;
	vec3 sp = lpn *25.0;
	float simplex = snoise(sp, 8, 3.0f, 0.7 );
	
	col = texture(texture0, UV );
	
	col *= 0.7+ 0.3*simplex;
	col *= (0.3+ 0.7*max( dot( nrm, normalize(vec3(0.8,0.5,0.2)) ),0.0) ); 
	
	/*
	float dis = length(CamPos - Pos );
		
	
	vec4 hl = col *vec4(0.7,99.0,0.7,1.0);
	vec4 hl2 = col *vec4(0.7,0.7,99.0,1.0);
	hl.g = min( hl.g,1.0);hl2.b = min( hl2.b,1.0);
	if( Col.r > 0.5 ) {
		vec4 hlt = hl; hl = hl2; hl2 = hlt;	
	}
	
	float d1 = LodMod*2.0, d2 = LodMod*1.5f;
	if( dis > d2 ) {
		

		if( dis < d1 ) {
			col += (hl-col) * (dis - d2)/(d1-d2);
		}// else 
		//	col = hl*0.3;
	}
	vec4 hl3  = Col; hl3.r = min( hl3.t,1.0);
	
	col *= Col.g; * /
	//col *= (vec4(1.0,1.0,1.0,1.0) +Col)*0.5f;
		
	//col =  vec4(1.0,1.0,1.0,1.0) * (simplex*0.5 +0.5);	
	//col = vec4(pow2(longatude),pow2(longatude),abs(longatude),1.0);	 
	// vec3 nrm = normalize(Norm);
	//col =  vec4(nrm*0.5 +vec3(0.5,0.5,0.5), 1.0);
	// col = noise2*noise.b;
	FragColor = col;
}
*/