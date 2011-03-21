
uniform sampler2DRect tex;

varying float alpha;
uniform float gamma;
uniform float luminance;
uniform float blendPower;
uniform float height;
uniform int flip;
uniform vec2 offset;

void main (void) {
	vec2 texPos = gl_FragCoord.xy;
	if(flip==1) {
		texPos.y = height - texPos.y;
		
	}
	texPos -= offset;
	vec4 sourceColor = texture2DRect(tex, texPos);

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
