#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;

uniform float height_scale;

uniform bool useHeightMap;
uniform bool useSpecularMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    float height = texture(texture_height, texCoords).r;
    vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    return texCoords - p;
}

void main() {
    // offset texture coordinates with parallax mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    if (useHeightMap) {
        vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
        if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
            //discard;
        }
    }

    // sample textures with new texture coords
    // obtain normal from normal map
    vec3 normal = normalize(texture(texture_normal, texCoords)).rgb;

    // transpose normal vector to range -1, 1
    normal = normalize(normal * 2.0 - 1.0); // tangent space

     // get diffuse color
    vec3 color = texture(texture_diffuse, texCoords).rgb;

    // ambient
    vec3 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular;
    if (useSpecularMap) {
        specular = vec3(0.2) * spec * vec3(texture(texture_specular, fs_in.TexCoords));
    }
    else {
        specular = vec3(0.2) * spec;
    }

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}