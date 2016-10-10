// DefaultMultitexturing2D.frag : Mix a number of Sampler2D Texture.

# version 330

in vec2 uTexCoord;

layout(location = 0) out vec4 fragColor;

uniform uint uTextureCount;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;
uniform sampler2D uTexture8;
uniform sampler2D uTexture9;

void main () 
{
    if ( uTextureCount > uint(0) ) fragColor = texture(uTexture0, uTexCoord);
    if ( uTextureCount > uint(1) ) fragColor = mix( fragColor, texture(uTexture1, uTexCoord), 0.5 );
    if ( uTextureCount > uint(2) ) fragColor = mix( fragColor, texture(uTexture2, uTexCoord), 0.5 );
    if ( uTextureCount > uint(3) ) fragColor = mix( fragColor, texture(uTexture3, uTexCoord), 0.5 );
    if ( uTextureCount > uint(4) ) fragColor = mix( fragColor, texture(uTexture4, uTexCoord), 0.5 );
    if ( uTextureCount > uint(5) ) fragColor = mix( fragColor, texture(uTexture5, uTexCoord), 0.5 );
    if ( uTextureCount > uint(6) ) fragColor = mix( fragColor, texture(uTexture6, uTexCoord), 0.5 );
    if ( uTextureCount > uint(7) ) fragColor = mix( fragColor, texture(uTexture7, uTexCoord), 0.5 );
    if ( uTextureCount > uint(8) ) fragColor = mix( fragColor, texture(uTexture8, uTexCoord), 0.5 );
    if ( uTextureCount > uint(9) ) fragColor = mix( fragColor, texture(uTexture9, uTexCoord), 0.5 );
}

