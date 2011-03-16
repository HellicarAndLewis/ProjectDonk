
uniform sampler2DRect tex;

varying float alpha;
uniform float gamma;
uniform float luminance;
uniform float blendPower;


void main (void) {
	vec4 sourceColor = texture2DRect(tex, gl_FragCoord.xy);

	float curve = alpha;
	
	
	curve = (curve < 0.5) ? 
		luminance * pow(2.0*curve, blendPower)
		:
		1.0 - (1.0 - luminance) * pow(2.0*(1.0 - curve), blendPower);
		
	float edge = pow(curve, 1.0/gamma);
	
	sourceColor.r *= edge;
	sourceColor.g *= edge;
	sourceColor.b *= edge;

	gl_FragColor = sourceColor;
	
}
