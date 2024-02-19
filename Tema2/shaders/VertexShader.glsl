#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for color
uniform vec3 object_color;
uniform int hp;

// Output value to fragment shader
out vec3 color;



void main()
{
    // use hp to change v_position
   // vec3 deformation = vec3(float(hp) * sin(v_position.x - 1.0f) / 5, 0, - float(hp) * cos(v_position.z + 1.0f));
   // change v_position after a parabolic function
    vec3 deformation = vec3(0.0f, 0.0f, 0.0f);
    color = object_color;
    // change color after hp
    if (hp != 0) {
        color = object_color.rgb * (1.0f - float(hp) / 5);
        //deformation.x = -float(hp) / 10;
        deformation.z = float(hp) / 20;
        //deformation.y = -float(hp) / 5;
	}

    gl_Position = Projection * View * Model * vec4(v_position - deformation, 1.0);
}
