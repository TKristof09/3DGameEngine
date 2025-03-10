#version 330

out vec4 fragColor;

in vec3 worldPos0;
in vec2 textCoord0;
in mat3 TBN0;

struct BaseLight{
	vec3 color;
	float intensity;
};

struct Attenuation{
	// exponent*x^2+linear*x+constant
	float exponent;
	float linear;
	float constant;
};

struct PointLight{
	BaseLight base;
	Attenuation attenuation;
	vec3 position;
	float range;
};

uniform sampler2D diffuse;
uniform sampler2D specular;

uniform sampler2D normalMap;

uniform vec3 eyePos;
uniform float specularExponent;

uniform PointLight pointLight;

uniform vec3 color;

vec3 GetNormalFromMap(){
	vec3 normal = texture(normalMap, textCoord0).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(TBN0 * normal);
	return normal;
}
vec4 CalcLight(BaseLight base,vec3 direction){
	// TODO idk why this is needed but it is 
	direction = normalize(-direction);
	// ambient

    // diffuse 
    vec3 norm = GetNormalFromMap();
    float angle = max(dot(norm, direction), 0.0);
    vec3 diffuse = angle * base.color * base.intensity;
    
    // specular
    vec3 viewDir = normalize(eyePos - worldPos0);
    vec3 reflectDir = reflect(-direction, norm);  
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
    vec3 specular = specularFactor * base.color * texture(specular, textCoord0).rgb;  
        
    vec3 result = diffuse + specular;
    return vec4(result,1.0);
}

vec4 CalcPointLight(PointLight pointLight){
	vec3 lightDir = worldPos0 - pointLight.position;
	float distanceToLight = length(lightDir);

	if(distanceToLight > pointLight.range)
		return vec4(0,0,0,0);
	lightDir = normalize(lightDir);
	vec4 color = CalcLight(pointLight.base, lightDir);

	float attenuation = 1 / (pointLight.attenuation.exponent * distanceToLight * distanceToLight +
						pointLight.attenuation.linear * distanceToLight +
						pointLight.attenuation.constant + 0.00001); 
						// +0.00001 to prevent division with 0

	return color * attenuation;
}

void main()
{
	fragColor = vec4(color, 1) * CalcPointLight(pointLight) * texture(diffuse, textCoord0.xy);
} 
