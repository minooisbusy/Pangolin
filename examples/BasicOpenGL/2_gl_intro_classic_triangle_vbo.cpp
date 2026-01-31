#include <pangolin/display/display.h>

// Here is an example of Pangolin being used *just* for windowing.
// We're using
void sample()
{
    pangolin::CreateWindowAndBind("Classic GL Triangle with VBO", 500, 500);

    // List coordinates of a triangle
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Create an OpenGL Buffer to store these coordinates
    unsigned int VBO; //Vertex Buffer Object
    glGenBuffers(1, &VBO); // 버퍼 객체 생성

    // Set that buffer as the current GL buffer
    // 생성한 버퍼를 현재 작업 대상으로 선택(바인딩)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy our host data into the currently bound OpenGL buffer
    // GL_STATIC_DRAW is a hint about how we'll use the buffer
    // CPU에 있는 데이터를 현재 바인딩된 GPU 버퍼로 복사
    // GL_STATIC_DRAW: 데이터가 한 번 설정되면 거의 변경되지 않음을 의미함
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glClearColor(0.64f, 0.5f, 0.81f, 0.0f); // 배경
    glColor3f(0.29f, 0.71f, 1.0f);  // 전경

    while( !pangolin::ShouldQuit() )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 화면 지우기

        // Set our buffer as the current one for OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // 버퍼 바인딩(선택)

        // This buffer contains floating point vertices with 3 dimensions.
        // They starts from the 0th element and are packed without padding.
        glVertexPointer(3, GL_FLOAT, 0, 0); // 정점 데이터 해석 방법 설정

        // Use Them!
        glEnableClientState(GL_VERTEX_ARRAY); // 활성화

        // Connect the first 3 of these vertices to form a triangle!
        glDrawArrays(GL_TRIANGLES, 0, 3); // 그리기

        // Disable the stuff we enabled...
        glDisableClientState(GL_VERTEX_ARRAY); // 비활성화
        glBindBuffer(GL_ARRAY_BUFFER, 0); // 바인딩 해제

        pangolin::FinishFrame();
    }

    // Deallocate the OpenGL buffer we made
    glDeleteBuffers(1, &VBO);
}

int main( int /*argc*/, char** /*argv*/ )
{
    sample();
    return 0;
}
