#include <pangolin/display/display.h>
#include <pangolin/gl/glvbo.h>

void sample()
{
    pangolin::CreateWindowAndBind("Pango GL Triangle with VBO", 500, 500);

    // Create an OpenGL Buffer "containing the vertices of a triangle"
    // 기존의 glGenBuffer, glBindBuffer, glBufferData 과정을 한번에 수행
    // 정점들을 갖고 있는 버퍼를 한 번에 생성
    pangolin::GlBuffer vbo(pangolin::GlArrayBuffer,
        std::vector<Eigen::Vector3f>{
           {-0.5f, -0.5f, 0.0f},
           { 0.5f, -0.5f, 0.0f },
           { 0.0f,  0.5f, 0.0f }
        }
    );

    glClearColor(0.64f, 0.5f, 0.81f, 0.0f); // 배경
    glColor3f(0.29f, 0.71f, 1.0f); // 전경

    while( !pangolin::ShouldQuit() )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //화면 지우기

        // Connect the first 3 vertices in the GL Buffer to form a triangle!
        // 정점 버퍼 객체(vbo)로 그리는 방법(GL_TRIANGLE)을 제공하고,
        // 버퍼 선택 -> 정점 구조 제공 -> 활성화 -> 그리기 -> 비활성화 -> 버퍼 해제까지 한 번에 수행
        pangolin::RenderVbo(vbo, GL_TRIANGLES);

        pangolin::FinishFrame(); // 이벤트 처리 및 버퍼 스왑
    }
}

int main( int /*argc*/, char** /*argv*/ )
{
    sample();
    return 0;
}
