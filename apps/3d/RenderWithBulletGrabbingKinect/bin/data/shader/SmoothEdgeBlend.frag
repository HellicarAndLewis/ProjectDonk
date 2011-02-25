/**
 * Sept 20 2010
 * 
 * This shader is ripped out of a Quartz Composer patch by  Matthias Oostrik
 * from http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/
 * Original Core Image filter converted by James George ( http://www.jamesgeorge.org/ )
 * for ofxProjectorBlend addon for openFrameworks ( http://www.openframeworks.cc )
 *
 * Originally used in the Infiniti MOI project for projector blending ( http://vimeo.com/14588336 )
 *
 * This shader is capable of blending on all sides, but ofxProjectorBlend currently
 * just uses the shader for two projections at once.
 */

uniform sampler2DRect Tex0;
uniform vec2 texCoordOffset;
uniform float width;
uniform float height;
uniform float OverlapLeft;
uniform float OverlapRight; 
uniform float OverlapTop;
uniform float OverlapBottom;
uniform float BlendPower;
uniform float SomeLuminanceControl;
uniform vec3  GammaCorrection;
uniform float SolidEdgeEnable;
uniform vec4  SolidEdgeColor;

vec4 drawSmoothEdge(float loc, float extend, float blePow, float lumCon, vec3 gamma)
{
	vec4 edge;
	float curve;

	curve = loc / extend;
	curve = (curve < 0.5) ? 
		lumCon * pow(2.0 * curve, blePow) 
		: 
		1.0 - (1.0 - lumCon) * pow(2.0 * (1.0 - curve), blePow);
	
	edge = vec4(pow(curve, 1.0 / gamma.r), 
				pow(curve, 1.0 / gamma.g),
				pow(curve, 1.0 / gamma.b),
				1.0);
				 
	return edge;
}


vec4 drawSmoothEdges(in vec4 overlap, in float blePow, in float lumCon, in vec3 gamma, in vec4 color)
{
	// the actual texcoords coming in need to be offset by texCoordOffset when looking at the texture (but not when testing
	// for whether we're at the edges.
	// e.g. gl_TexCoord[0].xy will give you where in the current screen we're drawing
	// and gl_TexCoord[0].xy + texCoordOffset will give you the pixel we're supposed to get from the texture.
	
	// use this for testing coordinates
	vec2 xy = gl_TexCoord[0].xy;
	
	// use this for getting coordinates out of the texture
	vec2 texCoord = xy + texCoordOffset;
	
	vec4 blank = vec4(0.0,0.0,0.0,0.0);	
	
	vec4 sebImage = 
		(overlap.x > xy.x) ? 
			(0.0 > xy.x) ? 	
				blank 							//leftBlankout
			:	(overlap.z > xy.y) ?
					(0.0 > xy.y) ? 
						blank					//leftBottomBlankOut
					: 	texture2DRect(Tex0, texCoord) 	*		//leftBottomBlend
						drawSmoothEdge(xy.x, overlap.x, blePow, lumCon, gamma) * 
						drawSmoothEdge(xy.y, overlap.z, blePow, lumCon, gamma)
				: 	(height - overlap.w < xy.y) ?  
						(height < xy.y) ? 
							blank				//leftTopBlankout
						: 	texture2DRect(Tex0, texCoord) 	*	//LeftTopBlend
							drawSmoothEdge(xy.x, overlap.x, blePow, lumCon, gamma) * 
							drawSmoothEdge(height - xy.y, overlap.w, blePow, lumCon, gamma)					
							:	texture2DRect(Tex0, texCoord) *		//leftBlankout
						drawSmoothEdge(xy.x, overlap.x, blePow, lumCon, gamma)
		:	(width - overlap.y < xy.x) ? 
				(width < xy.x) ? 
					blank						//rightBlankout
				:	(overlap.z > xy.y) ?
						(0.0 > xy.y) ? 
							blank 				//rightBottomBlankout
						: 	texture2DRect(Tex0, texCoord) 	*	//rightBottomBlend
							drawSmoothEdge(width - xy.x, overlap.y, blePow, lumCon, gamma) *
							drawSmoothEdge(xy.y, overlap.z, blePow, lumCon, gamma)
					:	(height - overlap.w < xy.y) ?
							(height < xy.y) ? 
								blank 			//rightTopBlankout
							:	texture2DRect(Tex0, texCoord) * //rightTopBlend
								drawSmoothEdge(width - xy.x, overlap.y, blePow, lumCon, gamma) * 
								drawSmoothEdge(height - xy.y, overlap.w, blePow, lumCon, gamma)
						:	texture2DRect(Tex0, texCoord) * 	//rightBlend
							drawSmoothEdge(width - xy.x, overlap.y, blePow, lumCon, gamma)
			:	(overlap.z > xy.y) ?
					(0.0 > xy.y) ? 
						blank 					//BottomBlankout
					: 	texture2DRect(Tex0, texCoord) * 		//BottomBlend
						drawSmoothEdge(xy.y, overlap.z, blePow, lumCon, gamma)
				:	(height - overlap.w < xy.y) ?
						(height < xy.y) ? 
							blank 				//TopBlankout
						:	texture2DRect(Tex0, texCoord) * 	//TopBlend
							drawSmoothEdge(height - xy.y, overlap.w, blePow, lumCon, gamma)					
							:	texture2DRect(Tex0, texCoord);	
	
	
	
	
	
	
	return sebImage;
}

void main (void) {
	float SLC = SomeLuminanceControl + 0.5;
	vec4 overlap = vec4 (OverlapLeft, OverlapRight, OverlapBottom, OverlapTop);
	gl_FragData[0] = drawSmoothEdges(overlap, BlendPower, SLC, GammaCorrection, SolidEdgeColor);
}
