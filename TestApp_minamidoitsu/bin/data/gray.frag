uniform sampler2DRect image;

varying vec3 pos;

void main (void)
{
    vec2 texCoord = vec2(pos.x , pos.y);

    vec4 col = texture2DRect(image,texCoord);
    float x = 2.2;
    col.b = pow(col.b, x) * 0.071330;
    col.r = pow(col.r, x) * 0.706655;
    col.g = pow(col.g, x) * 0.222015;
    float y = pow((col.b + col.g + col.r), (1 / x));
    vec4 dst = vec4(y,y,y,y);
    
    gl_FragColor.rgba = dst.rgba;
}