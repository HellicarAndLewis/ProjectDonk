	
uniform sampler2D tex;
   
void main (void) {

   gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * gl_Color;
   //gl_FragColor = vec4( gl_TexCoord[0].s, gl_TexCoord[0].t, 0, 1);
}