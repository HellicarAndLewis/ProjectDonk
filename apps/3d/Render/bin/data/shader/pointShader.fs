
varying float alpha;
uniform sampler2D tex;
   
void main (void) {

   vec4 v = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
   if(v.a > 0.0) {
      gl_FragColor = vec4(v.xyz, alpha);
   } else {
	 gl_FragColor = v;
   }

}