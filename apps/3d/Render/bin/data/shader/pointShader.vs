attribute float pointSize;
varying float alpha;

void main(void)
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
   gl_Position = gl_ProjectionMatrix * eyeCoord;
   float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
   //float att = 500.0 / dist;
	
   gl_PointSize = clamp(pointSize, 32.0, 48.0);
   gl_FrontColor = gl_Color;
   alpha = pointSize/32.0;
}