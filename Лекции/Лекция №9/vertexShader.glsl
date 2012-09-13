varying vec3 v_normal;
varying vec3 v_eye;
varying vec3 v_ray;

void main()
{
    vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 normal = vec3(gl_NormalMatrix * gl_Normal);
    vec3 light = vec3(gl_LightSource[0].position);
    
	v_normal = normalize(vertex - normal);
	v_eye = normalize(-vertex);
	v_ray = normalize(vertex - light);
	
	gl_Position = ftransform();
}

