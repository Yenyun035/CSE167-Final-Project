#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;
uniform vec4 lightpositions[ maximal_allowed_lights ];
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {
        
        // HW3: You will compute the lighting here.
        // Camera coord
        // transform position to camera coord
        vec4 cPosition = modelview * position;

        // transform normal (n vector) to camera coord
        mat3 aInvT = transpose(inverse(mat3(modelview)));
        vec3 n = normalize(aInvT * normal);

        // add emission E first
        fragColor = emision;
        
        for(int i = 0; i < nlights; i++) {
            // transform light position to camera coord
            vec4 cLightPos = view * lightpositions[i];

            // l vector
            vec3 l = normalize(cPosition.w * cLightPos.xyz - cLightPos.w * cPosition.xyz);

            // v vector = -p
            vec3 v = normalize(-1 * cPosition.xyz);

            // h vector
            vec3 h = normalize(v + l);

            fragColor += lightcolors[i] * (ambient + diffuse * max(dot(n,l), 0) + specular * pow(max(dot(n,h), 0), shininess));
        }
    }
}
