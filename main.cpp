#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <cmath> // для sinf, cosf и других математических функций




// Комбинированная анимация (вращение + пульсация)

void DrawLoadingSpinner(ImVec2 center, float radius, ImU32 color) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    static float t = 0.0f;
    t += ImGui::GetIO().DeltaTime * 3.0f;
    
    const int segments = 12;
    const float thickness = radius * 0.2f;
    
    for (int i = 0; i < segments; ++i) {
        float angle = t + i * (2 * M_PI / segments);
        float alpha = 0.3f + 0.7f * (0.5f + 0.5f * sinf(angle * 2.0f));
        ImU32 seg_color = (color & ~IM_COL32_A_MASK) | ((int)(alpha * 255) << IM_COL32_A_SHIFT);
        
        draw_list->AddLine(
            ImVec2(center.x + cosf(angle) * radius,
            center.y + sinf(angle) * radius),
            ImVec2(center.x + cosf(angle) * (radius - thickness),
            center.y + sinf(angle) * (radius - thickness)),
            seg_color, thickness
        );
    }
}

// Интерполяция
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t) {
    return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}


// Анимированная галочка (progress animation)
void DrawAnimatedCheckmark(ImVec2 pos, ImU32 color, float progress) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    ImVec2 p1 = pos;
    ImVec2 p2 = ImVec2(pos.x + 5, pos.y + 10);
    ImVec2 p3 = ImVec2(pos.x + 15, pos.y);
    
    if (progress < 0.5f) {
        float t = progress * 2.0f;
        draw_list->AddLine(p1, ImLerp(p1, p2, t), color, 2.0f);
    } else {
        float t = (progress - 0.5f) * 2.0f;
        draw_list->AddLine(p1, p2, color, 2.0f);
        draw_list->AddLine(p2, ImLerp(p2, p3, t), color, 2.0f);
    }
}



static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    // Инициализация GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // Создаем окно GLFW
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Dynamic Tabs", NULL, NULL);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();


    // Инициализация рендерера (GLFW + OpenGL3)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Главный цикл
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Начинаем новый кадр ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

	
	// кастомные иконки
		ImGui::Begin("Progress Indicators");
		// Использование (progress от 0.0 до 1.0)
		static float check_progress = 0.0f;
		check_progress = fmodf(check_progress + ImGui::GetIO().DeltaTime * 0.5f, 1.0f);
		DrawAnimatedCheckmark(ImGui::GetCursorScreenPos(), IM_COL32(0, 255, 0, 255), check_progress);
        DrawLoadingSpinner(ImVec2(ImGui::GetCursorScreenPos().x+50,ImGui::GetCursorScreenPos().y+100), 10.0f, IM_COL32(0, 255, 0, 255));
		ImGui::End();


        // Рендеринг
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Очистка
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}





