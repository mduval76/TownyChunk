out vec4 FragColor;

varying vec4 light;
uniform vec4 color;
uniform float time;
uniform vec2 screenSize;

float pseudoNoise(vec2 coord) {
    return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec2 texCoord = (gl_FragCoord.xy / screenSize) * 0.05;
    float noiseValue = pseudoNoise(texCoord + time);

    // Modulate the alpha value with the noise
    float alpha = color.a * noiseValue; 

    FragColor = vec4(color.rgb, alpha);
}