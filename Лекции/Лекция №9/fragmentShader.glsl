varying vec3 v_normal;
varying vec3 v_eye;
varying vec3 v_ray;

#define SHININESS 5.0

void main()
{
    vec3 reflection = reflect(v_ray, v_normal);
    float diffuse = max(dot(v_normal, v_ray), 0.0);
    float specular = pow(max(dot(reflection, v_eye), 0.0), SHININESS);
    
    vec4 color = gl_FrontMaterial.ambient +
                 diffuse * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse +
                 specular * gl_FrontMaterial.specular * gl_LightSource[0].specular;

	gl_FragColor = vec4(color.rgb, 1.0);
}

