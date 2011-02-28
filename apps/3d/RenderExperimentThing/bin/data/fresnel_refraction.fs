	
uniform vec3 fresnelValues;
uniform vec3 IoR_Values;
//uniform float opacity;
uniform samplerCube environmentMap;
uniform sampler2D glossMap;
uniform sampler2D baseMap;
varying vec3 N;
varying vec3 E;

varying vec3 NTrans;
varying vec3 ITrans;
// the amount that we're going to fade at the edges
uniform float EdgeFalloff;
	
vec3 refract(vec3 i, vec3 n, float eta)
{
    float cosi = dot(-i, n);
    float cost2 = 1.0 - eta * eta * (1.0 - cosi*cosi);
    vec3 t = eta*i + ((eta*cosi - sqrt(abs(cost2))) * n);
    return t * vec3(cost2 > 0.0);
}

	float fast_fresnel(vec3 I, vec3 N, vec3 fresnelValues)
	{
	    float bias = fresnelValues.x;
	    float power = fresnelValues.y;
	    float scale = 1.0 - bias;
	
	    return bias + pow(1.0 - dot(I, N), power) * scale;
	}


	void main(void)
	{
		vec3 normal = normalize(N);
	     vec3 incident = normalize(E);
		
	   	// make reflection
	   	//
		vec3 reflectionVector = normalize(reflect(incident, normal));
	   	vec3 reflectColor = textureCube(environmentMap, reflectionVector).xyz;
	  	
		//------ Find the refraction
	   	//
	    vec3 refractColor;
	    refractColor.x = textureCube(environmentMap, refract(incident, normal, IoR_Values.x)).x;
	    refractColor.y = textureCube(environmentMap, refract(incident, normal, IoR_Values.y)).y;
	    refractColor.z = textureCube(environmentMap, refract(incident, normal, IoR_Values.z)).z;
		
		vec3 base_color = texture2D(baseMap, gl_TexCoord[0].st).rgb;
		
		// now do gloss colorzz	
		vec3 gloss_color = texture2D(glossMap, gl_TexCoord[0].st).rgb;
		float reflectivity = (gloss_color.r + gloss_color.g + gloss_color.b)/2.0;
		
	    
	   	//------ Find the Fresnel term
	   	//
	    float fresnelTerm = fast_fresnel(-incident, normal, fresnelValues);

    	float opacity = dot(normalize(NTrans), normalize(-ITrans));
		opacity = abs(opacity);
		opacity = 1.0 - pow(opacity, EdgeFalloff);

		vec4 bTint = gl_Color;
		// janky way to get a slight blue...I guess
		bTint.z += abs(EdgeFalloff * 0.15);

		//------ final pixel -
		vec3 color = mix(refractColor, reflectColor, fresnelTerm);
		gl_FragColor = vec4( mix(base_color, color, reflectivity), (opacity * bTint));
}
