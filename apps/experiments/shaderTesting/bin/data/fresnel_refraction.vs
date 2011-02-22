	uniform vec3 CameraPos;
	uniform mat4 ModelWorld4x4;
	varying vec3 N;
	varying vec3 E;
	
	varying vec3 NTrans;
	varying vec3 ITrans;

	// getting the 3x3 out of the 4x4 matrix
	// maybe there's a cleverer way to do this
	mat3 GetLinearPart( mat4 m )
	{
		mat3 result;
		
		result[0][0] = m[0][0]; 
		result[0][1] = m[0][1]; 
		result[0][2] = m[0][2]; 

		result[1][0] = m[1][0]; 
		result[1][1] = m[1][1]; 
		result[1][2] = m[1][2]; 
		
		result[2][0] = m[2][0]; 
		result[2][1] = m[2][1]; 
		result[2][2] = m[2][2]; 
		
		return result;
	}		
	
	
	void main()
	{

		NTrans = gl_NormalMatrix * gl_Normal;
		ITrans = vec3(gl_ModelViewMatrix * gl_Vertex);

	    // output position
		gl_Position = ftransform();
		
		// Texture coordinates for glossMap. 
		//gl_TexCoord[0] = gl_MultiTexCoord0; // not doing this
		
		gl_FrontColor = gl_Color;
		gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
		
		
		mat3 ModelWorld3x3 = GetLinearPart( ModelWorld4x4 );
		
		// find world space position.
		vec4 WorldPos = ModelWorld4x4 *  gl_Vertex;	
		
		// find world space normal.
		N = ModelWorld3x3 * gl_Normal; 
		
		// find world space eye vector.
		E = WorldPos.xyz - CameraPos.xyz;	
	}