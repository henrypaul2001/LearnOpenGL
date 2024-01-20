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

const float minLayers = 8.0;
const float maxLayers = 32.0;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    // number of depth layers
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));;

    // calculate size of each layer
    float layerDepth = 1.0 / numLayers;

    // depth of current layerDepth
    float currentLayerDepth = 0.0;

    // amount to shift the texture coordinates per layer
    vec2 P = viewDir.xy * height_scale;
    vec2 deltaTexCoords = P / numLayers;

    // get initial values
    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(texture_height, currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;

        // get depth map value at current coords
        currentDepthMapValue = texture(texture_height, currentTexCoords).r;

        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture_height, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main() {
    // offset texture coordinates with parallax mapping
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;

    if (useHeightMap) {
        texCoords = ParallaxMapping(texCoords, viewDir);
        if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
            discard;
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
        specular = vec3(0.2) * spec * vec3(texture(texture_specular, texCoords));
    }
    else {
        specular = vec3(0.2) * spec;
    }

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}