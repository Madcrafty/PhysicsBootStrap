// Phong shader for simple game lighting
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; // The ambient color of the model's material
uniform vec3 Kd; // The diffuse color of the model's material
uniform vec3 Ks; // The specular color of the model's material
uniform float Ns; // The specular power of the model's material

//Debug
float specularPower;

uniform vec3 AmbientColor; // Colour of abient light
uniform vec3 LightColor; // Colour of light
uniform vec3 LightDirection;

uniform vec3 CameraPosition;

out vec4 FragColor;

void main(){
    //Debug stuffs
    specularPower = 0.0f;
    // Make sure the normal and the light directions have been normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);

    mat3 TBN = mat3(T,B,N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;

    N = TBN * (texNormal * 2 - 1);

    // Now we can calculate the lambert term, negatative the light direction
    float lambertTerm = max(0, min(1,dot(N,-L)));

    // Calculate the view vector and the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    // Determine the value of the specular term
    float specularTerm = pow(max(0, dot(R,V)), Ns);

    // Determine the value of the ambient
    vec3 ambient = AmbientColor * Ka * texDiffuse;

    // Determine the value of the diffuse
    vec3 diffuse = LightColor * Kd * lambertTerm * texDiffuse;

    // Dertermin the value of the specular
    vec3 specular = LightColor * Ks * specularTerm * texSpecular;

    // N = TBN * (texNormal * 2 - 1); -> delete for standard normals
    // Show standard normals
    // FragColor = vec4(N, 1);

    // Show texture normals
    //FragColor = vec4(texNormal, 1);

    // Show normals without texture
    //FragColor = vec4(ambient + lambertTerm + specular, 1);

    //Output final colour 
    FragColor = vec4(ambient + diffuse + specular, 1);
}