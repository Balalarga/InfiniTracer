#include <iostream>
#include <SDL_syswm.h>

#include "OpenGL/Core/Scene.h"
#include "WindowSystem/OpenglWindow.h"

using namespace std;


std::string vShaderCode = R"(#version 330

layout(location = 0)in vec3 iVert;
layout(location = 1)in vec4 iVertColor;

out vec4 vertColor;

void main()
{
    gl_Position = vec4(iVert, 1.0);
    vertColor = iVertColor;
}
)";
std::string fShaderCode = R"(#version 330

in vec4 vertColor;
out vec4 fragColor;

void main()
{
    fragColor = vertColor;
}
)";

ShaderPart vShader(ShaderPart::Type::Vertex, vShaderCode);
ShaderPart fShader(ShaderPart::Type::Fragment, fShaderCode);
Shader shader(&vShader, &fShader);

void BaseInput(ISdlWindow& window)
{
    InputSystem::Get().Add(SDL_SCANCODE_ESCAPE,
        [&window](KeyState state)
        {
            if (state == KeyState::Pressed)
                window.Close();
        });
}

void BaseObjects(Scene& scene)
{
    if (!shader.Compile(true))
    {
        std::cout<<"Shader compilation error\n";
        return;
    }
    
    struct {
        float x, y, z;
        float r, g, b, a;
    } triangle[] {
        {-0.1f, -0.1f, 0.f, 1.f, 0.f, 0.f, 1.f},
        {-0.1f,  0.1f, 0.f, 1.f, 1.f, 0.f, 1.f},
        { 0.0f,  0.0f, 0.f, 1.f, 0.f, 0.f, 1.f},
    };
    Buffer buffer(DataPtr(triangle, sizeof(triangle)/sizeof(triangle[0]), sizeof(triangle[0])), BufferLayout().Float(3).Float(4));
    auto& Obj = scene.AddObject(new IRenderable(buffer));
    Obj.SetShader(&shader);
}

int main(int argc, char** argv)
{
    ISdlWindowParams params;
    params.vsync = true;
    
    OpenglWindow window(params);
    {
        Scene scene;
    
        window.SetBackgroundColor(glm::vec4(0.6, 0.6, 0.6, 1.0));
    
        window.SetScene(&scene);
        BaseInput(window);
        BaseObjects(scene);

        window.Show();
    }
    shader.Destroy();
    vShader.Destroy();
    fShader.Destroy();
    
    return 0;
}

