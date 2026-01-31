#include <pangolin/display/display.h>
#include <pangolin/gl/gl.h>

void sample()
{
    // Create a 500x500 pixel application window with an OpenGL Context
    // By default the window is double buffered if available.
    // * double buffer: 한번에 완벽한 화면을 디스플레이하기 위한 기법
    //                  버퍼에서 처리해서 디스플레이에 한번에 뿌림
    // Load any available OpenGL Extensions (through GLEW)
    pangolin::CreateWindowAndBind("Classic GL Triangle", 500, 500);

    // List coordinates of a triangle
    // These vertices will be relative to the coordinates of the window
    // which default in OpenGL to +/- 1.0 in X and Y (first two vertex ordinates)
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,    // z축은 일정하니까, 좌하단
         0.5f, -0.5f, 0.0f,    // 우하단
         0.0f,  0.5f, 0.0f     // 상단
    };

    // We want the background to be purple
    glClearColor(0.64f, 0.5f, 0.81f, 0.0f); //RGBA, each element in [0, 1)?

    // We want our triangle to be a pleasant shade of blue!
    glColor3f(0.0f, 0.0f, 1.0f); // Just RGB? 함수다 함수.

    // We keep rendering in a loop so that the triangle will keep showing
    // and get adjusted as the window is resized. Press Escape or close the
    // window to exit the Pangolin loop.
    while( !pangolin::ShouldQuit() )
    {
        // Clear the window
        // * GL_COLOR_BUFFER_BIT: 배경 버퍼, RGBA를 담고 있는 버퍼
        // * GL_DEPTH_BUFFER_BIT: 화면에 그려지는 픽셀들의 카메라로부터 거리를 저장
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // This buffer contains floating point vertices with 3 dimensions.
        // They starts from the 0th element and are packed without padding.
        // 각 vertex의 위치를 OpenGL에게 알려준다.
        // 3: 각 정점이 3-vector로 이뤄짐
        // GL_FLOAT: 좌표값의 데이터 타입이 float임 
        // 0: stride, 데이터의 간격, 0이면 연속적으로 붙어있음
        // vertices: 실제 데이터가 담긴 배열의 주소
        glVertexPointer(3, GL_FLOAT, 0, vertices);

        // Use Them!
        // 정점 배열 사용을 활성화 한다. == 그리기 작업에 등록한 정점들을 실제로 사용해라.
        glEnableClientState(GL_VERTEX_ARRAY);

        // Connect the first 3 of these vertices to form a triangle!
        // 실제로 그리기
        // GL_TRIANGLES: 점 3개씩 묶어 삼각형을 그림
        // 0: 배열의 0번째 인덱스부터 시작
        // 3: 총 세 개의 점을 사용
        glDrawArrays(GL_TRIANGLES, 0, 3); 

        // Disable the stuff we enabled...
        // 정점 배열 사용을 비활성화 한다. == 등록한 정점들을 이제는 사용하지마라.
        glDisableClientState(GL_VERTEX_ARRAY); // why?

        // Process any windowing events and swap the back and front
        // OpenGL buffers if available.
        pangolin::FinishFrame();
    }
}

int main( int /*argc*/, char** /*argv*/ )
{
    sample();
    return 0;
}
