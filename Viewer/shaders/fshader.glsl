#version 330 core
#define MAX_LIGHTS_SIZE 20

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    sampler2D texture_map;
    sampler2D normal_map;
};

struct Light
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 pos;
};

struct Lights
{
    Light list[MAX_LIGHTS_SIZE];
    int size;
};

uniform Material material;
uniform Lights lights;
uniform vec4 camera_pos;
uniform bool apply_toon_shading;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 origfragPos;
out vec4 fragColor;

void main()
{
    vec3 color = vec3(0.0f, 0.0f, 0.0f);
    vec3 texture_color = vec3(texture(material.texture_map, fragTexCoords));
    vec3 n = normalize(fragNormal);
    vec3 v = normalize(camera_pos.xyz - fragPos);

    for (int i = 0; i < lights.size; i++)
    {
        vec3 l = normalize(lights.list[i].pos.xyz - fragPos);
        vec3 r = normalize(2.0f * n * dot(l, n) - l);

        vec3 Ia = material.ambient.xyz * lights.list[i].ambient.xyz;
        vec3 Id = material.diffuse.xyz * lights.list[i].diffuse.xyz * max(dot(l, n), 0.0);
        vec3 Is = material.specular.xyz * lights.list[i].specular.xyz * pow(max(dot(v, r), 0.0), material.shininess);

        color += (Ia + Id + Is);

        if (apply_toon_shading)
        {
            vec3 toon_shading_color = vec3(1.0f, 1.0f, 1.0f);
	        float intensity = dot(l, fragNormal);
	        if (intensity > 0.85f)
		        toon_shading_color = vec3(0.8f, 0.8f, 0.8f);
	        else if (intensity > 0.5f)
		        toon_shading_color = vec3(0.6f, 0.6f, 0.6f);
	        else if (intensity > 0.15f)
		        toon_shading_color = vec3(0.4f ,0.4f, 0.4f);
	        else
		        toon_shading_color = vec3(0.2f, 0.2f, 0.2f);

            color = color * toon_shading_color;
        }
    }
    
    color.x = min(1.0f, color.x);
    color.y = min(1.0f, color.y);
    color.z = min(1.0f, color.z);
    fragColor = vec4(color, 1.0f);
}