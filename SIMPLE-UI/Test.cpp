#include "SIMPLE-UI.h" // ����SIMPLE-UI��
#include <windows.h>
using namespace std;

int main() {
    // ����һ�����ڣ��ߴ�Ϊ 800x600
    Swindow testWindow(800, 600,50,50, "Test Window");
    // ���ñ���ɫΪ��ɫ
    testWindow.setBackgroundColor(Scolor::BLACK);
    // ������Ⱦ����
    SrenderEngine renderengine(&testWindow);
    // ��ʼ����Ⱦ����
    if (!renderengine.init()) {
        cerr << "Failed to initialize render engine." << endl;
        system("pause");
        return -1;
    }
    // ��Ⱦѭ��
    while (!renderengine.shouldClose()) {
        renderengine.render();
    }
    return 0;
}
