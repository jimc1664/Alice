#version 150

#define PI 3.1415926535897932384626433832795

out vec4 FragColor;

in vec2 UV;
in vec3 Norm;
in vec3 Pos;
in float Radius;
in vec4 Col;
in vec4 WPos;

uniform	sampler2D texture0;
uniform mat4 MVP;
uniform vec3 CamPos;
uniform float LodMod;

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


vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  { 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }
  
  
float snoise(vec3 v, int iter, float fMod, float rough ) {
	float ret = 0.0, mod = 1.0, tot = 0.0;
	for( int i = iter; i-- !=0; ) {
		ret += snoise(v) *mod;
		tot += mod;
		v*= fMod; mod *= rough;		
		
	}
	return ret/tot;
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
	
	col *= Col.g; */
	//col *= (vec4(1.0,1.0,1.0,1.0) +Col)*0.5f;
		
	//col =  vec4(1.0,1.0,1.0,1.0) * (simplex*0.5 +0.5);	
	//col = vec4(pow2(longatude),pow2(longatude),abs(longatude),1.0);	 
	// vec3 nrm = normalize(Norm);
	//col =  vec4(nrm*0.5 +vec3(0.5,0.5,0.5), 1.0);
	// col = noise2*noise.b;
	FragColor = col;
}
