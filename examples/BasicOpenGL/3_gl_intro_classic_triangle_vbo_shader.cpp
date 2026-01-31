#include <pangolin/display/display.h>
/* 
 * 이 코드는 Pangolin을 사용하여 윈도우를 생성하고, 
 * **VBO(Vertex Buffer Object)**와 **GLSL 셰이더(Shader)**를 사용하여 
 * 애니메이션 효과가 들어간 삼각형을 그리는 예제입니다.

 * 기존의 고정 함수 파이프라인(Fixed-Function Pipeline) 대신
 * **프로그래밍 가능한 파이프라인(Programmable Pipeline)**의 기초를 보여줍니다.
*/

// GLSL Start
// 1. 정점 셰이더
// 각 정점의 위치를 계산 => Geometry
const char* vertex_shader = R"Shader(
#version 120
attribute vec3 a_position; // 입력: 정점의 위치
varying vec2 v_pos;        // 출력: 프래그먼트 셰이더로 전달할 좌표

void main() {
    // gl_Position은 정점의 최종 화면 위치를 결정하는 내장 변수다.
    gl_Position = vec4(a_position, 1.0);
    // x, y 좌표를 프래그먼트 셰이더에서 패턴을 만드는 데 쓰기 위해 넘겨준다.
    v_pos = a_position.xy;
}
)Shader";

// 2. 프래그먼트 셰이더
// 화면의 각 픽셀(프래그먼트)의 색상을 결정한다. => Color
const char* fragment_shader = R"Shader(
#version 120
varying vec2 v_pos; // 버텍스 셰이더로부터 받은 좌표
uniform float u_time; // 외부(C++)에서 넘겨주는 시간 값 (애니메이션용)

vec3 colorA = vec3(0.905,0.045,0.045); // 빨간색 계열
vec3 colorB = vec3(0.995,0.705,0.051); // 노란색 계열

void main() {
    // sin 함수와 시간 (u_time)을 이용해 움직이는 물결 패턴을 계산한다.
    float pattern = sin(10*v_pos.y + u_time) * sin(10*v_pos.x + u_time) * 0.5 + 0.5;
    // 두 색상을 패턴 값에 따라 섞는다.
    vec3 color = mix(colorA, colorB, pattern);
    // 최종 픽셀 색상을 출력
    gl_FragColor = vec4(color, 1.0);
}
)Shader";

// GLSL end

// Here is an example of Pangolin being used *just* for windowing.
// Influenced by https://learnopengl.com/Getting-started/Hello-Triangle
void sample()
{
    // 3. 윈도우 생성
    pangolin::CreateWindowAndBind("Classic GL Triangle With VBO and Shader", 500, 500);

    // 4. 셰이더 컴파일 및 프로그램 생성
    // ------------------------------------

    // 정점 셰이더 컴파일
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader); // compile

    // Check if the compilation was successfull, and print anyn errors
    // 컴파일 에러 체크
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 프래그먼트 셰이더 컴파일
    // Repeat for the Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    // 프래그먼트 셰이더 컴파일 에러 체크
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 셰이더 프로그램 링크 (두 프로그램을 하나로 합침)
    // Link the vertex and fragment shaders into one complete program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); // 먼저 vertexShader 추가
    glAttachShader(shaderProgram, fragmentShader); // 다음으로 fragmentShader 추가
    glLinkProgram(shaderProgram); // 링킹!

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete the now unused shader objects
    glDeleteShader(vertexShader); // 링킹 했으니까 제거~
    glDeleteShader(fragmentShader); // me too~

    // Setup the Triangle VBO
    // ------------------------------------

    // List coordinates of a triangle
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // 진부한 출력 과정
    // Create an OpenGL Buffer to store these coordinates
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Set that buffer as the current GL buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy our host data into the currently bound OpenGL buffer
    // GL_STATIC_DRAW is a hint about how we'll use the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enable use of the shader that we created for future rendering commands
    glUseProgram(shaderProgram); // 생성한 프로그램 사용 선언

    glClearColor(0.64f, 0.5f, 0.81f, 0.0f);

    // Setup a variable to progress a simple animation as a function of time
    float time = 2.06f;

    while( !pangolin::ShouldQuit() )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set our buffer as the current one for OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // This buffer contains floating point vertices with 3 dimensions.
        // They starts from the 0th element and are packed without padding.
        glVertexPointer(3, GL_FLOAT, 0, 0);

        // Use Them!
        glEnableClientState(GL_VERTEX_ARRAY);

        // [중요] C++의 time 변수 값을 셰이더 내부의 u_time 변수로 전달한다.
        GLint u_time = glGetUniformLocation(shaderProgram, "u_time");
        glUniform1f( u_time, time);

        // Connect the first 3 of these vertices to form a triangle!
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Disable the stuff we enabled...
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        time += 0.10;

        pangolin::FinishFrame();
    }

    // Deallocate the OpenGL buffer we made
    glDeleteBuffers(1, &VBO);

    // Deallocate the GlSl Shader program
    glDeleteProgram(shaderProgram);
}

int main( int /*argc*/, char** /*argv*/ )
{
    sample();
    return 0;
}
