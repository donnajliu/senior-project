#version 330
// ^ Change this to version 130 if you have compatibility issues

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec4 fs_Normal;
in vec4 fs_LightVector;
in vec4 fs_Color;
in vec4 R;
in vec4 V;

uniform vec4 u_lightCol;
uniform float u_nExp;
out vec4 out_Color;

void main() {

    // Material base color (before shading)
    vec4 diffuseColor = fs_Color * u_lightCol;

    // Calculate the diffuse term
    float diffuseTerm = dot(normalize(fs_Normal), normalize(fs_LightVector));

    // Avoid negative lighting values
    diffuseTerm = clamp(diffuseTerm, 0, 1);

    // Specular term calculations
    // R - unit reflected ray
    // V - eye position
    vec4 nR = normalize(R);
    vec4 nV = normalize(V);

    float dotP = dot(nR, nV);
    dotP = clamp(dotP, 0, 1);

    float specularTerm = pow(dotP, 50);

    float ambientTerm = 0.20;
    diffuseTerm = diffuseTerm * 0.75;

    float lightIntensity = diffuseTerm + ambientTerm + specularTerm;
    lightIntensity = clamp(lightIntensity, 0, 1);// + 0.3;

    // Compute final shaded color
    out_Color = vec4(diffuseColor.rgb * lightIntensity + 0.2, diffuseColor.a);
}
