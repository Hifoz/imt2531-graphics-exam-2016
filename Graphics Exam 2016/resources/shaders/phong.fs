#version 410

const int spotlightCount = 6;

struct DirectionalLight
{
	vec3 dir;
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct SpotLight
{
	vec3 pos;
	vec3 dir;
	vec3 color;
	float arc;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
Material material;

in vec3 frag_color;
in vec3 frag_pos;
in vec3 frag_normal;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight[spotlightCount];
uniform mat4 ViewMatrix;
uniform vec3 viewPos;

out vec4 fragment_color;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);  

void main()
{
	material.diffuse = vec3(1, 1, 1);
	material.specular = vec3(1, 1, 1);
	material.shininess = 5;

	vec3 viewDir = normalize(viewPos - frag_pos);
	vec3 normal = normalize(frag_normal);

	// Calculate Directional light
	vec3 res = CalcDirectionalLight(directionalLight, normal, viewDir);

	// Calculate and add all spotlights
	for(int i = 0; i < spotlightCount; i++)
		res += CalcSpotLight(spotLight[i], normal, viewDir);

	vec3 ambient = frag_color * 0.3;
	fragment_color = vec4(ambient + res, 1);
}


// Calculates a directional light
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	// Diffuse
	vec3 lightDir = normalize(light.dir);
	float diff = clamp(dot(normal, lightDir), 0, 1);
	vec3 diffuse = light.diffuse * diff * light.color * frag_color;

	// Specular
	vec3 reflectDir = reflect(-light.dir, normal);
	float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), material.shininess);
	vec3 specular = light.specular * spec * light.color * frag_color;

	return diffuse + specular;
}

// Calculates a spotlight
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{	
//Calculate direction factor
	vec3 lightPosEye = light.pos;
	vec3 directionToSurfaceEye = normalize(frag_pos - lightPosEye);
	float directionDot = dot(directionToSurfaceEye, normalize(-light.dir));
	float directionFactor = clamp((directionDot - light.arc) / 0.5, 0.0, 1.0);
	
	//Diffuse light
	vec3 distanceToLightEye = lightPosEye - frag_pos;
	vec3 directionToLightEye = normalize(distanceToLightEye);
	float dotProd = dot(directionToLightEye, normal);
	dotProd = max(dotProd, 0.0);
	vec3 diffuse = light.diffuse * dotProd;
	
	//Specular light
	vec3 reflectionEye = reflect(-directionToLightEye, normal);
	vec3 surfaceToViewerEye = normalize(-frag_pos);
	float dotProdSpecular = dot(reflectionEye, viewDir);
	dotProdSpecular = max(dotProdSpecular, 0.0);
	float specularFactor = pow(dotProdSpecular, material.shininess);
	vec3 specular = light.specular * specularFactor;

	float attenuation = 1.0 / max(1, distance(lightPosEye, frag_pos) / 5);
			
	return (diffuse + specular ) * frag_color  * directionFactor; // * attenuation;
} 